// ============================================================================
// State variable filter
// ----------------------------------------------------------------------------
#pragma once
#include <pch.h>
// ============================================================================
// Includes
// ============================================================================

namespace AugCSynth
{

// ============================================================================
// SVFilter
// ============================================================================
class SVFilter
{
	SVFilter();
	void SetFilterFreq(float val) { mFreq = val; }
	void SetFilterRes(float val) { mRes = val; }
	void SetFilterType(uint8_t type);

	float NextSample(float smpl);

private:
	float mFreq;
	float mRes;
	float mTypeMult;

    float mLp;             // Low-pass state
    float mBp;             // Band-pass state
    float mHp;             // High-pass state
};

} // namespace AugCSynth
