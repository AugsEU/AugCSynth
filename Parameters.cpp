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
SynthParameter gParameters[NUM_PARAMETERS];
SynthMode gMode;

// ============================================================================
// Public functions
// ============================================================================

void ZeroOutParams()
{
	memset(gParameters, 0x00, sizeof(gParameters));
}

float GetFloatParam(size_t idx)
{
	return gParameters[idx].mFloatValue;
}

void SetFloatParam(size_t idx, float param)
{
	gParameters[idx].mFloatValue = param;
}

uint32_t GetIntParam(size_t idx)
{
	return gParameters[idx].mIntValue;
}

void SetIntParam(size_t idx, uint32_t param)
{
	gParameters[idx].mIntValue = param;
}

SynthMode GetSynthMode()
{
	return gMode;
}

void SetSynthMode(SynthMode mode)
{
	gMode = mode;
}

} //namespace AugCSynth
