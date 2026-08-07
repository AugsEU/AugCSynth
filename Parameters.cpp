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


SynthParamBounds::SynthParamBounds(int8_t maxValue)
{
    mBound = maxValue;
}

int8_t SynthParamBounds::ApplyDelta(int8_t value, int8_t delta)
{
    value += delta;
    int8_t max = GetMaxValue();
    int8_t min = GetMinValue();
    if(value > max)
    {
        value = max;
    }
    else if(value < min)
    {
        value = min;
    }

	return value;
}

float SynthParamBounds::GetNormFloatValue(int8_t value)
{
    int8_t max = GetMaxValue();
    int8_t min = GetMinValue();
    if(min < 0 && value < 0)
    {
        return -(float)value / (float)min;
    }

    return (float)value / (float)max;
}

int8_t SynthParamBounds::GetMaxValue()
{
    if(mBound > 0)
    {
        return mBound;
    }

    return -mBound;
}

int8_t SynthParamBounds::GetMinValue()
{
    if(mBound < 0)
    {
        return mBound;
    }

    return 0;
}

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
