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
    float mFreq; // Note: Oscillator frequency may vary from this.
    uint8_t mPlayingNoteIdx;

    Oscillator mOsc1;
    Envelope mEnv1;

    Oscillator mOsc2;
    Envelope mEnv2;

    float mLfoDelta;
    float mLfoAmount;
};


// ============================================================================
// Public functions
// ============================================================================

void VoiceInit(SubVoice* pVoice);

void VoiceOn(SubVoice* pVoice, uint8_t playingNoteIdx);
void VoiceOnSteal(SubVoice* pVoice, uint8_t playingNoteIdx);
void VoiceOff(SubVoice* pVoice);

void VoicePrepSampleBlock(SubVoice* pVoice);
float VoiceGetSample(SubVoice* pVoice, 
    uint32_t waveShape1, uint32_t waveShape2, 
    float tune1, float tune2, 
    float shape1, float shape2,
    float lfoValue);

float VoiceEligibility(SubVoice* pVoice, uint8_t noteIdx);

}