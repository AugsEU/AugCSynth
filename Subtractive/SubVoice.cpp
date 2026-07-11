// ============================================================================
// Includes
// ============================================================================
#include <Voice.h>
#include <Tuning.h>

#include "Parameters.h"
#include "Subtractive/SubParams.h"
#include "Utilities/QwertyMath.h"
#include <Config.h>

#define MONO_OSC 0

namespace AugCSynth::Subtractive
{

// ============================================================================
// Public functions
// ============================================================================

/// @brief Initialise the voice at program start.
void VoiceInit(SubVoice* pVoice)
{
    pVoice->mPlayingNoteIdx = 0;
    pVoice->mFreq = 0.0f;

    OscInit(&pVoice->mOsc1);
    EnvInit(&pVoice->mEnv1);

    OscInit(&pVoice->mOsc2);
    EnvInit(&pVoice->mEnv2);
}

/// @brief Begin playing voice.
void VoiceOn(SubVoice* pVoice, uint8_t playingNoteIdx)
{
    pVoice->mEnv1.mSection = ES_ATTACK;
    pVoice->mEnv2.mSection = ES_ATTACK;

    bool sameNote = playingNoteIdx == pVoice->mPlayingNoteIdx;
    if (!sameNote)
    {
        pVoice->mEnv2.mVolume = 0.0f;
        pVoice->mEnv1.mVolume = 0.0f;
    }

    pVoice->mLfoAmount = 0.0f;

    pVoice->mPlayingNoteIdx = playingNoteIdx;
    pVoice->mFreq = NoteToFreq(pVoice->mPlayingNoteIdx) * SAMPLE_PERIOD;
}

/// @brief Begin playing voice when stolen from another.
void VoiceOnSteal(SubVoice* pVoice, uint8_t playingNoteIdx)
{
    if(pVoice->mEnv1.mSection == ES_RELEASE || pVoice->mEnv1.mSection == ES_OFF)
    {
        pVoice->mEnv1.mSection = ES_ATTACK;
    }
    
    if(pVoice->mEnv2.mSection == ES_RELEASE || pVoice->mEnv2.mSection == ES_OFF)
    {
        pVoice->mEnv2.mSection = ES_ATTACK;
    }

    pVoice->mPlayingNoteIdx = playingNoteIdx;
    pVoice->mFreq = NoteToFreq(pVoice->mPlayingNoteIdx) * SAMPLE_PERIOD;
}

void VoiceOff(SubVoice* pVoice)
{
    pVoice->mEnv1.mSection = ES_RELEASE;
    pVoice->mEnv2.mSection = ES_RELEASE;
}

void VoicePrepSampleBlock(SubVoice* pVoice)
{
    pVoice->mEnv1.mAttack = GetFloatParam(ASP_ENV_ATTACK1);
    pVoice->mEnv1.mDecay = GetFloatParam(ASP_ENV_DECAY1);
    pVoice->mEnv1.mSustain = GetFloatParam(ASP_ENV_SUSTAIN1);
    pVoice->mEnv1.mRelease = GetFloatParam(ASP_ENV_RELEASE1);
    
    pVoice->mEnv2.mAttack = GetFloatParam(ASP_ENV_ATTACK2);
    pVoice->mEnv2.mDecay =  GetFloatParam(ASP_ENV_DECAY2);
    pVoice->mEnv2.mSustain = GetFloatParam(ASP_ENV_SUSTAIN2);
    pVoice->mEnv2.mRelease = GetFloatParam(ASP_ENV_RELEASE2);

    pVoice->mLfoDelta = GetFloatParam(ASP_LFO_ATTACK);
}

float VoiceGetSample(SubVoice* pVoice, 
                        uint32_t waveType1, uint32_t waveType2, 
                        float tune1, float tune2, 
                        float shape1, float shape2,
                        float lfoValue)
{
    if(pVoice->mLfoAmount < 1.0f)
    {
        pVoice->mLfoAmount += pVoice->mLfoDelta;
    }
    lfoValue *= pVoice->mLfoAmount;

    float osc1TuneLFO = FastUnitExp(GetFloatParam(ASP_LFO_OSC1_TUNE) * lfoValue);
    float osc2TuneLFO = FastUnitExp(GetFloatParam(ASP_LFO_OSC2_TUNE) * lfoValue);
    float dt = pVoice->mFreq;

    // Osc1
    OscPhaseInc(&pVoice->mOsc1, dt * tune1 * osc1TuneLFO);
    float osc1;
    switch (waveType1)
    {
    default:
    case OSC_MODE_SINE:
        osc1 = SineQuadraic(pVoice->mOsc1.mPhase);
        osc1 = ShapeWave(osc1, shape1);
        break;
    case OSC_MODE_SQUARE:
        osc1 = SquareWaveBLEPShape(pVoice->mOsc1.mPhase, dt, shape1);
        break;
    case OSC_MODE_SAW:
        osc1 = SawWaveBLEP(pVoice->mOsc1.mPhase, dt);
        osc1 = ShapeWave(osc1, shape1);
        break;
    case OSC_MODE_ORGAN:
        osc1 = SquareWaveOrgan(pVoice->mOsc1.mPhase, shape1);
        break;
    }
    osc1 *= GetFloatParam(ASP_DCO_VOL_1);

    EnvNextSample(&pVoice->mEnv1);
    osc1TuneLFO = pVoice->mEnv1.mVolume;// reuse var
    osc1 *= osc1TuneLFO * osc1TuneLFO;
    osc1 *= ComputeLfoMult(lfoValue, GetFloatParam(ASP_LFO_OSC1_VOLUME));

#if !MONO_OSC
    // Osc2
    OscPhaseInc(&pVoice->mOsc2, dt * tune2 * osc2TuneLFO);
    float osc2;
    switch (waveType2)
    {
    default:
    case OSC_MODE_SINE:
        osc2 = SineQuadraic(pVoice->mOsc2.mPhase);
        osc2 = ShapeWave(osc2, shape2);
        break;
    case OSC_MODE_SQUARE:
        osc2 = SquareWaveBLEPShape(pVoice->mOsc2.mPhase, dt, shape2);
        break;
    case OSC_MODE_SAW:
        osc2 = SawWaveBLEP(pVoice->mOsc2.mPhase, dt);
        osc2 = ShapeWave(osc2, shape2);
        break;
    case OSC_MODE_ORGAN:
        osc2= SquareWaveOrgan(pVoice->mOsc2.mPhase, shape1);
        break;
    }
    osc2 *= GetFloatParam(ASP_DCO_VOL_2);

    EnvNextSample(&pVoice->mEnv2);
    osc2TuneLFO = pVoice->mEnv2.mVolume;// reuse var
    osc2 *= osc2TuneLFO * osc2TuneLFO;
    osc2 *= ComputeLfoMult(lfoValue, GetFloatParam(ASP_LFO_OSC2_VOLUME));

    return osc1 + osc2;
#else
    return osc1;
#endif
}

float VoiceEligibility(SubVoice* pVoice, uint8_t noteIdx)
{
    if(pVoice->mEnv1.mSection == ES_OFF)
    {
        return 10.0f;
    }

    float value = pVoice->mEnv1.mSection * (1.0f-pVoice->mEnv1.mVolume);
    value += (float)pVoice->mEnv2.mSection * (1.0f-pVoice->mEnv2.mVolume);

    if (pVoice->mPlayingNoteIdx == noteIdx)
    {
        value += 9.0f;
    }

    return value;
}

}