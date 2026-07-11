// ============================================================================
// Non-linear filter
// ----------------------------------------------------------------------------
#pragma once
#include <pch.h>
// ============================================================================
// Includes
// ============================================================================
#include <stdint.h>



namespace AugCSynth
{

// ============================================================================
// NLFilter
// ============================================================================
class NLFilter
{
public:
	NLFilter();
	void SetFilterFreq(float val);
	void SetFilterRes(float val);
	void SetFilterType(uint8_t type);

	float NextSample(float smpl);

private:
	uint8_t 	mType;
	float	    mFreq;
	float		mGain;
	float		mQ;
	float		mSample0;
	float		mSample1;
};

} // namespace AugCSynth
