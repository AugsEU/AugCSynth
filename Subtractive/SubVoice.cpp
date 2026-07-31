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
void SubVoice::Init()
{
    mPlayingNoteIdx = 0;
    mFreq = 0.0f;

    mComp1.Init();

    mComp2.Init();
}

/// @brief Begin playing voice.
void SubVoice::VoiceOn(uint8_t note)
{
    mComp1.mEnv.mSection = ES_ATTACK;
    mComp2.mEnv.mSection = ES_ATTACK;

    bool sameNote = note == mPlayingNoteIdx;
    if (!sameNote)
    {
        mComp2.mEnv.mVolume = 0.0f;
        mComp1.mEnv.mVolume = 0.0f;
    }

    mLfoAmount = 0.0f;

    mPlayingNoteIdx = note;
    mFreq = NoteToFreq(mPlayingNoteIdx) * SAMPLE_PERIOD;
}

/// @brief Begin playing voice when stolen from another.
void SubVoice::VoiceOnSteal(uint8_t note)
{
    if(mComp1.mEnv.mSection == ES_RELEASE || mComp1.mEnv.mSection == ES_OFF)
    {
        mComp1.mEnv.mSection = ES_ATTACK;
    }
    
    if(mComp2.mEnv.mSection == ES_RELEASE || mComp2.mEnv.mSection == ES_OFF)
    {
        mComp2.mEnv.mSection = ES_ATTACK;
    }

    mPlayingNoteIdx = note;
    mFreq = NoteToFreq(mPlayingNoteIdx) * SAMPLE_PERIOD;
}

void SubVoice::VoiceOff()
{
    mComp1.mEnv.mSection = ES_RELEASE;
    mComp2.mEnv.mSection = ES_RELEASE;
}

void SubVoice::PrepSampleBlock()
{
    mComp1.mEnv.mAttack = GetFloatParam(ASP_ENV_ATTACK1);
    mComp1.mEnv.mDecay = GetFloatParam(ASP_ENV_DECAY1);
    mComp1.mEnv.mSustain = GetFloatParam(ASP_ENV_SUSTAIN1);
    mComp1.mEnv.mRelease = GetFloatParam(ASP_ENV_RELEASE1);
    
    mComp2.mEnv.mAttack = GetFloatParam(ASP_ENV_ATTACK2);
    mComp2.mEnv.mDecay =  GetFloatParam(ASP_ENV_DECAY2);
    mComp2.mEnv.mSustain = GetFloatParam(ASP_ENV_SUSTAIN2);
    mComp2.mEnv.mRelease = GetFloatParam(ASP_ENV_RELEASE2);

    mLfoDelta = GetFloatParam(ASP_LFO_ATTACK);
}

float SubVoice::GetSample(
                    WaveType waveType1, WaveType waveType2, 
                    float tune1, float tune2, 
                    float shape1, float shape2,
                    float lfoValue)
{
    if(mLfoAmount < 1.0f)
    {
        mLfoAmount += mLfoDelta;
    }
    lfoValue *= mLfoAmount;

    const float osc1TuneLFO = FastUnitExp(GetFloatParam(ASP_LFO_OSC1_TUNE) * lfoValue);
    const float osc2TuneLFO = FastUnitExp(GetFloatParam(ASP_LFO_OSC2_TUNE) * lfoValue);
    const float dt = mFreq;

    // Osc1
    float osc1 = mComp1.DoNextSample(dt * tune1 * osc1TuneLFO, waveType1, shape1);
    osc1 *= GetFloatParam(ASP_DCO_VOL_1);
    osc1 *= ComputeLfoMult(lfoValue, GetFloatParam(ASP_LFO_OSC1_VOLUME));

#if !MONO_OSC
    // Osc2
    float osc2 = mComp2.DoNextSample(dt * tune2 * osc2TuneLFO, waveType2, shape2);
    osc2 *= GetFloatParam(ASP_DCO_VOL_2);
    osc2 *= ComputeLfoMult(lfoValue, GetFloatParam(ASP_LFO_OSC2_VOLUME));

    return osc1 + osc2;
#else
    return osc1;
#endif
}

float SubVoice::Eligibility(uint8_t note)
{
    if(mComp1.mEnv.mSection == ES_OFF)
    {
        return 10.0f;
    }

    float value = mComp1.mEnv.mSection * (1.0f-mComp1.mEnv.mVolume);
    value += (float)mComp2.mEnv.mSection * (1.0f-mComp2.mEnv.mVolume);

    if (mPlayingNoteIdx == note)
    {
        value += 9.0f;
    }

    return value;
}

float SubVoice::VoiceComponent::DoNextSample(float phaseInc, WaveType waveType, float waveShape)
{
    mOsc.PhaseInc(phaseInc);
    float out = GetWaveSample(mOsc.mPhase, phaseInc, waveType, waveShape);
    
    mEnv.NextSample();
    const float env1Volume = mEnv.mVolume;
    out *= env1Volume * env1Volume;

    return out;
}

}