// ============================================================================
// Voice for subtractive synth
// ----------------------------------------------------------------------------
#pragma once
#include <pch.h>
// ============================================================================
// Includes
// ============================================================================
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "Oscillator.h"
#include "Envelope.h"



namespace AugCSynth::Subtractive
{

// ============================================================================
// Public types
// ============================================================================
struct SubVoice
{
    void Init();
    void VoiceOn(uint8_t note);
    void VoiceOnSteal(uint8_t note);
    void VoiceOff();

    void PrepSampleBlock();
    float GetSample(
        uint32_t waveShape1, uint32_t waveShape2,
        float tune1, float tune2,
        float shape1, float shape2,
        float lfoValue);

    float Eligibility(uint8_t note);

    float mFreq; // Note: Oscillator frequency may vary from this.
    uint8_t mPlayingNoteIdx;

    Oscillator mOsc1;
    Envelope mEnv1;

    Oscillator mOsc2;
    Envelope mEnv2;

    float mLfoDelta;
    float mLfoAmount;
};

}