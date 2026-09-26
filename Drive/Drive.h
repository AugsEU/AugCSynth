// ============================================================================
// Voice for subtractive synth
// ----------------------------------------------------------------------------
#pragma once
#include <pch.h>

namespace AugCSynth {

// ============================================================================
// Types
// ============================================================================
enum class DriveMode
{
	Off,
	PolyDrive,
	RatioDrive,
	Count
};

// ============================================================================
// Public functions
// ============================================================================

/// @brief Apply overdrive to a sample
/// @param mode Drive 
/// @param input Input sample
/// @param drive Drive amount, 0 to 1, zero is no drive
/// @return Overdriven sample
float DriveSample(DriveMode mode, float input, float drive);

/// @brief Apply overdrive to a sample using a polynomial formula
/// @param input Input sample
/// @param drive Drive amout, 0 to 1
/// @return Overdriven sample
float PolynomialDrive(float input, float drive);

/// @brief Apply overdrive to a sample using a rational formula
/// @param input Input sample
/// @param drive Drive amout, 0 to 1
/// @return Overdriven sample
float RationalDrive(float input, float drive);


}