// ============================================================================
// Synth parameters
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
// Public constant
// ============================================================================
constexpr size_t NUM_PARAMETERS = 40; // TO DO fix this?

// ============================================================================
// Public types
// ============================================================================

struct SynthParameter
{
	union
	{
		float mFloatValue;
		uint32_t mIntValue;
	};
};

enum class SynthMode : uint8_t
{
	Subtractive = 0x00
};

// ============================================================================
// Public functions
// ============================================================================

void ZeroOutParams();

float GetFloatParam(size_t idx);
void SetFloatParam(size_t idx, float param);

uint32_t GetIntParam(size_t idx);
void SetIntParam(size_t idx, uint32_t param);

SynthMode GetSynthMode();
void SetSynthMode(SynthMode mode);

} // namespace AugCSynth
