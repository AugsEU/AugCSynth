// ============================================================================
// Utilities to receive from the front end
// ----------------------------------------------------------------------------
#pragma once
#include <pch.h>
// ============================================================================
// Includes
// ============================================================================
#include <stdint.h>
#include <math.h>


namespace AugCSynth
{

// ============================================================================
// Public functions
// ============================================================================
uint32_t GetNextRand();
float FastUnitExp(float x);
float ComputeLfoMult(float lfoValue, float lfoAmount);
float ComputeLoudnessMult(float loudness, float amount);
float ShapeWave(float waveValue, float waveShape);
float DrivenSample(float sample);

} // namespace AugCSynth

