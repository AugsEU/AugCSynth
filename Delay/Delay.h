// ============================================================================
// Delay effect object
// ----------------------------------------------------------------------------
#pragma once
#include <pch.h>
// ============================================================================
// Includes
// ============================================================================


namespace AugCSynth {

// ============================================================================
// Types
// ============================================================================

// Delay modes
enum class DelayMode
{
	Off,
	Feedback, // infinite repeats
	Slapback, // one repeat
	Glitch, // time varying
    Count
};

class Delay
{
public:
    void Init();

    void SetParams(DelayMode mode, uint32_t delaySamples,
        			uint32_t feedback, uint16_t glide);

    int16_t GetNextSample(int16_t currentSample);

private:
    void DoGlitch();

	static constexpr size_t kNumBufferSamples = 48000;

    uint16_t mDelayBuffer[kNumBufferSamples];

    int32_t mWriteHead;
    int32_t mReadOffset;
    int32_t mTargetReadOffset;
    int32_t mGlitchOffset;

    uint32_t mFeedbackVol;
    uint16_t mGlide;

    DelayMode mMode;
};

}