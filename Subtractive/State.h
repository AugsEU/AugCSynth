// ============================================================================
// Non-linear filter for subtractive synth
// ----------------------------------------------------------------------------
#pragma once
#include <pch.h>
// ============================================================================
// Includes
// ============================================================================
#include "Oscillator.h"
#include <Filter/NLFilter.h>

// ============================================================================
// Constants
// ============================================================================
#define DELAY_BUFFER_LEN 48000
#define DELAY_GLITCH_SIZE 2000
#define LOUDNESS_ALPHA (0.001f)

namespace AugCSynth::Subtractive
{

struct SubState
{
	uint16_t mDelayBuffer[DELAY_BUFFER_LEN];
	uint32_t mDelayWriteHead;
	int32_t mDelayReadOffset;
	int32_t mDelayReadOffsetOffset;

	Oscillator mLFO;
	Oscillator mLFOWobbler;
	NLFilter mFilter;
	float mCurrLoudness;
};

}