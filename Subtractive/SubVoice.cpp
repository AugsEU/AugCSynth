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

    mOsc1.Init();
    mEnv1.Init();

    mOsc2.Init();
    mEnv2.Init();
}

/// @brief Begin playing voice.
void SubVoice::VoiceOn(uint8_t note)
{
    mEnv1.mSection = ES_ATTACK;
    mEnv2.mSection = ES_ATTACK;

    bool sameNote = note == mPlayingNoteIdx;
    if (!sameNote)
    {
        mEnv2.mVolume = 0.0f;
        mEnv1.mVolume = 0.0f;
    }

    mLfoAmount = 0.0f;

    mPlayingNoteIdx = note;
    mFreq = NoteToFreq(mPlayingNoteIdx) * SAMPLE_PERIOD;
}

/// @brief Begin playing voice when stolen from another.
void SubVoice::VoiceOnSteal(uint8_t note)
{
    if(mEnv1.mSection == ES_RELEASE || mEnv1.mSection == ES_OFF)
    {
        mEnv1.mSection = ES_ATTACK;
    }
    
    if(mEnv2.mSection == ES_RELEASE || mEnv2.mSection == ES_OFF)
    {
        mEnv2.mSection = ES_ATTACK;
    }

    mPlayingNoteIdx = note;
    mFreq = NoteToFreq(mPlayingNoteIdx) * SAMPLE_PERIOD;
}

void SubVoice::VoiceOff()
{
    mEnv1.mSection = ES_RELEASE;
    mEnv2.mSection = ES_RELEASE;
}

void SubVoice::PrepSampleBlock()
{
    mEnv1.mAttack = GetFloatParam(ASP_ENV_ATTACK1);
    mEnv1.mDecay = GetFloatParam(ASP_ENV_DECAY1);
    mEnv1.mSustain = GetFloatParam(ASP_ENV_SUSTAIN1);
    mEnv1.mRelease = GetFloatParam(ASP_ENV_RELEASE1);
    
    mEnv2.mAttack = GetFloatParam(ASP_ENV_ATTACK2);
    mEnv2.mDecay =  GetFloatParam(ASP_ENV_DECAY2);
    mEnv2.mSustain = GetFloatParam(ASP_ENV_SUSTAIN2);
    mEnv2.mRelease = GetFloatParam(ASP_ENV_RELEASE2);

    mLfoDelta = GetFloatParam(ASP_LFO_ATTACK);
}

float SubVoice::GetSample(
                    uint32_t waveType1, uint32_t waveType2, 
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
    mOsc1.PhaseInc(dt * tune1 * osc1TuneLFO);
    float osc1 = GetWaveSample(mOsc1.mPhase, dt, (WaveType)waveType1, shape1);
    osc1 *= GetFloatParam(ASP_DCO_VOL_1);

    mEnv1.NextSample();
    const float env1Volume = mEnv1.mVolume;
    osc1 *= env1Volume * env1Volume;
    osc1 *= ComputeLfoMult(lfoValue, GetFloatParam(ASP_LFO_OSC1_VOLUME));

#if !MONO_OSC
    // Osc2
    mOsc2.PhaseInc(dt * tune2 * osc2TuneLFO);
    float osc2 = GetWaveSample(mOsc2.mPhase, dt, (WaveType)waveType2, shape2);
    osc2 *= GetFloatParam(ASP_DCO_VOL_2);

    mEnv2.NextSample();
    const float env2Volume = mEnv2.mVolume;
    osc2 *= env2Volume * env2Volume;
    osc2 *= ComputeLfoMult(lfoValue, GetFloatParam(ASP_LFO_OSC2_VOLUME));

    return osc1 + osc2;
#else
    return osc1;
#endif
}

float SubVoice::Eligibility(uint8_t note)
{
    if(mEnv1.mSection == ES_OFF)
    {
        return 10.0f;
    }

    float value = mEnv1.mSection * (1.0f-mEnv1.mVolume);
    value += (float)mEnv2.mSection * (1.0f-mEnv2.mVolume);

    if (mPlayingNoteIdx == note)
    {
        value += 9.0f;
    }

    return value;
}

}