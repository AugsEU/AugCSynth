// ============================================================================
// Includes
// ============================================================================
#include "Envelope.h"
#include "Parameters.h"
#include "Subtractive/SubParams.h"

namespace AugCSynth::Subtractive
{

// ============================================================================
// Public functions
// ============================================================================
void Envelope::Init()
{
    mSection = ES_OFF; 
    mAttack = 0.01f;
    mSustain = 0.95f;
    mDecay = 0.01f;
    mRelease = 0.01f;
    mVolume = 0.0f;
}

void Envelope::NextSample()
{
    EnvelopeSection section = mSection;

    switch (section)
    {
    case ES_OFF:
        break;
    case ES_ATTACK:
        mVolume += mAttack;
        if(mVolume >= 1.0f)
        {
            mVolume = 1.0f;
            mSection = ES_DECAY;
        }
        break;
    case ES_DECAY:
        mVolume -= mDecay;
        if(mVolume <= mSustain)
        {
            mVolume = mSustain;
            PresetType soundType = (PresetType)GetIntParam(SubParameter::SoundType);
            if(soundType == PresetType::Poly || soundType == PresetType::Mono)
            {
                mSection = ES_SUSTAIN;
            }
            else
            {
                mSection = ES_RELEASE;
            }
        }
        break;
    case ES_SUSTAIN:
        if(mSustain == 0.0f)
        {
            mSection = ES_OFF;
        }
        break; // Something else has to set this release.
    case ES_RELEASE:
        mVolume -= mRelease;
        if(signbit(mVolume))
        {
            mVolume = 0.0f;
            mSection = ES_OFF;
        }
        break;
    }
}

}