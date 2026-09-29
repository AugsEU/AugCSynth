// ============================================================================
// Non-linear filter for subtractive synth
// ----------------------------------------------------------------------------
#pragma once
#include <pch.h>
// ============================================================================
// Includes
// ============================================================================
#include <math.h>
#include <stdint.h>
#include <stdbool.h>



namespace AugCSynth::Subtractive
{

// ============================================================================
// Public functions
// ============================================================================
#ifndef AUGCSYNTH_NO_STATE

void SynthInit(void);
void FillSoundBuffer(int16_t* buf, uint16_t samples);

#endif // !AUGCSYNTH_NO_STATE
}
