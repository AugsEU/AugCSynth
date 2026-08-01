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
#include <Delay/Delay.h>

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
	Oscillator mLFO;
	Oscillator mLFOWobbler;
	NLFilter mFilter;
	Delay mDelay;
	
	float mCurrLoudness;
};

}