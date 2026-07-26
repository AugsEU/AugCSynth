// ============================================================================
// Includes
// ============================================================================
#include "Parameters.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>


namespace AugCSynth
{

// ============================================================================
// Private Types
// ============================================================================



// ============================================================================
// Globals
// ============================================================================


// ============================================================================
// Public functions
// ============================================================================

void ZeroOutParams()
{
	memset(State().mParameters, 0x00, sizeof(State().mParameters));
}

float GetFloatParam(size_t idx)
{
	return State().mParameters[idx].mFloatValue;
}

void SetFloatParam(size_t idx, float param)
{
	State().mParameters[idx].mFloatValue = param;
}

uint32_t GetIntParam(size_t idx)
{
	return State().mParameters[idx].mIntValue;
}

void SetIntParam(size_t idx, uint32_t param)
{
	State().mParameters[idx].mIntValue = param;
}

SynthMode GetSynthMode()
{
	return State().mMode;
}

void SetSynthMode(SynthMode mode)
{
	State().mMode = mode;
}

} //namespace AugCSynth
