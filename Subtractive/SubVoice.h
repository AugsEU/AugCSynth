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
private:
    struct VoiceComponent
    {
        Oscillator mOsc;
        Envelope mEnv;

        void Init() { mOsc.Init(); mEnv.Init(); }
        float DoNextSample(float phaseInc, WaveType waveType, float waveShape);
    };

public:
    void Init();
    void VoiceOn(uint8_t note);
    void VoiceOnSteal(uint8_t note);
    void VoiceOff();

    void PrepSampleBlock();
    float GetSample(
        WaveType waveShape1, WaveType waveShape2,
        float tune1, float tune2,
        float shape1, float shape2,
        float lfoValue);

    float Eligibility(uint8_t note);

    float mFreq; // Note: Oscillator frequency may vary from this.
    uint8_t mPlayingNoteIdx;

    VoiceComponent mComp1;
    VoiceComponent mComp2;

    float mLfoDelta;
    float mLfoAmount;
};

}