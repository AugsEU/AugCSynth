// ============================================================================
// Aggregate of wall wave genration functions
// ----------------------------------------------------------------------------
#pragma once
#include <pch.h>
// ============================================================================
// Inline
// ============================================================================
#include "WaveSine.h"
#include "WaveSquare.h"
#include "WaveSaw.h"


namespace AugCSynth {
	
// ============================================================================
// Types
// ============================================================================
enum class WaveType
{
	Sine,
	Square,
	Saw,
	Organ,
	Count
};


// ============================================================================
// Public shared wave functions
// ============================================================================

/// @brief Get audio sample for a wave
/// @param phase Wave input from 0 to 1
/// @param dt Phase increment
/// @param waveType Wave type
/// @param shape Wave shape param
float GetWaveSample(float phase, float dt, WaveType waveType, float shape);

/// @brief Get LFO sample for a wave
/// @param phase Wave input from 0 to 1
/// @param dt Phase increment
/// @param waveType Wave type
float GetWaveLFO(float phase, WaveType waveType);


}