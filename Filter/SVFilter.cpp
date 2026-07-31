// ============================================================================
// Includes
// ============================================================================
#include "SVFilter.h"
#include "Subtractive/SubParams.h"


namespace AugCSynth
{

// ============================================================================
// Public functions
// ============================================================================

SVFilter::SVFilter()
{
    mLp = 0.0f;
    mBp = 0.0f;
    mHp = 0.0f;

	mFreq = 0.0f;
	mRes = 0.0f;
	SetFilterType(FilterMode::HighPass);
}

void SVFilter::SetFilterType(FilterMode type)
{
	mTypeMult = (float)(type == FilterMode::LowPass);
}

float SVFilter::NextSample(float sample)
{
	float res = mRes;
    res *= res;
    res *= 5.0f;
    res += 1.0f;
    res = 1.0f / res;

	float freq = mFreq;
    freq *= 0.98f;
    freq *= freq;
    freq += 0.01f;
    freq = 0.01f + 0.99f * freq * freq;;

    // Filter processing
    mHp = sample - mLp - res * mBp;
    mBp += mHp * freq;
    mLp += mBp; 
    
    return (mLp * mTypeMult) + (mHp * (1.0f - mTypeMult));
}


} // namespace AugCSynth
