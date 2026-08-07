// ============================================================================
// Includes
// ============================================================================
#include "Delay.h"
#include <string.h>
#include "Utilities/QwertyMath.h"




// ============================================================================
// Constants
// ============================================================================
static constexpr int32_t kDelayGlitchSize = 2000;




namespace AugCSynth {

// ============================================================================
// Delay
// ============================================================================

void Delay::Init()
{
	memset(mDelayBuffer, 0, sizeof(mDelayBuffer));

	mWriteHead = 0;
	mReadOffset = 0;
	mTargetReadOffset = 0;
	mGlitchOffset = 0;

	mFeedbackVol = 0;
	mGlide = 1;
	mMode = DelayMode::Off;
}



void Delay::SetParams(DelayMode mode, uint32_t delaySamples,
						uint32_t feedback, uint16_t glide)
{
	mMode = mode;

	if (delaySamples >= kNumBufferSamples)
	{
		delaySamples = kNumBufferSamples - 1;
	}

	mTargetReadOffset = delaySamples;
	mFeedbackVol = feedback;
	mGlide = (glide == 0) ? 1 : glide;

	DoGlitch();
}

void Delay::DoGlitch()
{
	uint32_t rndValue = GetNextRand();
	if(mMode == DelayMode::Glitch)
	{
		if(rndValue % 13 == 0)
		{
			if(rndValue % 3 == 0 && mGlitchOffset < kDelayGlitchSize)
			{
				mGlitchOffset += kDelayGlitchSize / 3;
			}
			else if (rndValue % 3 == 1 && mGlitchOffset > -kDelayGlitchSize)
			{
				mGlitchOffset -= kDelayGlitchSize / 3;
			}

			if(mGlitchOffset + mTargetReadOffset > (int32_t)kNumBufferSamples)
			{
				mGlitchOffset = (int32_t)kNumBufferSamples - mTargetReadOffset;
			}
			else if(mGlitchOffset + mTargetReadOffset < 0)
			{
				mGlitchOffset = -mTargetReadOffset;
			}
		}
	}
	else
	{
		mGlitchOffset = 0;
	}
}

int16_t Delay::GetNextSample(int16_t currentSample)
{
	if (mMode == DelayMode::Off)
	{
		return currentSample;
	}

	// Smooth delay time
	int32_t targetOffset = mGlitchOffset + mTargetReadOffset;
	if ((mWriteHead % mGlide) == 0)
	{
		if (mReadOffset < targetOffset)
			++mReadOffset;
		else if (mReadOffset > targetOffset)
			--mReadOffset;
	}

	int32_t delaySample = 0;

	if (mReadOffset != 0)
	{
		uint32_t readHead = mWriteHead + ((int32_t)kNumBufferSamples) - mReadOffset;
		if (readHead >= kNumBufferSamples)
			readHead -= kNumBufferSamples;

		delaySample = (int16_t)mDelayBuffer[readHead];
	}

	int32_t output = currentSample + delaySample;

	if (output > 32767)
		output = 32767;
	else if (output < -32768)
		output = -32768;

	int32_t writeValue = currentSample;

	if (mMode == DelayMode::Feedback)
	{
		writeValue = output;
		writeValue *= mFeedbackVol;
		writeValue /= 32768;
	}

	mDelayBuffer[mWriteHead] = (uint16_t)(int16_t)writeValue;

	++mWriteHead;
	if (mWriteHead == kNumBufferSamples)
		mWriteHead = 0;

	return (int16_t)output;
}

}