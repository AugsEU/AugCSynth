// ============================================================================
// Envelope for subtractive synth
// ----------------------------------------------------------------------------
#pragma once
#include <pch.h>
// ============================================================================
// Includes
// ============================================================================
#include <stdint.h>
#include <stdbool.h>
#include <math.h>


namespace AugCSynth::Subtractive
{
    
// ============================================================================
// Types
// ============================================================================

enum EnvelopeSection
{
    ES_ATTACK,
    ES_DECAY,
    ES_SUSTAIN,
    ES_RELEASE,
    ES_OFF
};

struct Envelope
{
    void Init();
    void NextSample();

    EnvelopeSection mSection;
    float mAttack;
    float mDecay;
    float mSustain;
    float mRelease;
    
    float mVolume;
};
    
}
