// ============================================================================
// Non-linear filter
// ----------------------------------------------------------------------------
#pragma once
#include <pch.h>
// ============================================================================
// Includes
// ============================================================================
#include <stdint.h>
#include "Filter.h"


namespace AugCSynth
{

// ============================================================================
// NLFilter
// ============================================================================
class NLFilter
{
public:
	void Init();
	void SetFilterFreq(float val);
	void SetFilterRes(float val);
	void SetFilterType(FilterMode type);

	float NextSample(float smpl);

private:
	FilterMode 	mType;
	float	    mFreq;
	float		mGain;
	float		mQ;
	float		mSample0;
	float		mSample1;
};

} // namespace AugCSynth
