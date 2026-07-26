// ============================================================================
// LUT for tuning notes
// ----------------------------------------------------------------------------
#pragma once
#include <pch.h>
// ============================================================================
// Includes
// ============================================================================
#include <math.h>
#include <stdint.h>




namespace AugCSynth
{

// ============================================================================
// Public functions
// ============================================================================
enum class Tuning
{
	Equal12,
	Equal24,
	JustC,
	JustCs,
	JustD,
	JustDs,
	JustE,
	JustF,
	JustFs,
	JustG,
	JustGs,
	JustA,
	JustAs,
	JustB,
	CircleOfFifths,
	Wonky,
};

	
// ============================================================================
// Public functions
// ============================================================================

/// @brief Set tuning table
void SetTuning(Tuning tuning);

/// @brief Convert note number to freq
float NoteToFreq(uint8_t note);

} // namespace AugCSynth
