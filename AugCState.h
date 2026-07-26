// ============================================================================
// Struct which contains entire state of synth
// ----------------------------------------------------------------------------
#pragma once
#include <pch.h>
// ============================================================================
// Include
// ============================================================================
#include <Voice.h>
#include <Config.h>

#include <Subtractive/State.h>

namespace AugCSynth
{

// ============================================================================
// Public constant
// ============================================================================
constexpr size_t NUM_PARAMETERS = 40; // TO DO fix this?





// ============================================================================
// Public types
// ============================================================================

struct SynthParameter
{
	union
	{
		float mFloatValue;
		uint32_t mIntValue;
	};
};

enum class SynthMode : uint8_t
{
	Subtractive = 0x00
};





// ============================================================================
// State struct
// ============================================================================
struct AugCState
{
	// Individual modes
	union
	{
		Subtractive::SubState mSubState;
	};	

	// Voices
	Voice mVoices[VOICE_POLYPHONY];
	uint8_t mFreeNoteSearchStart;

	// Tuning
	const float* mTuningTable;

	// Parameters
	SynthParameter mParameters[NUM_PARAMETERS];
	SynthMode mMode;

	void Init();
};

// Assert this is plain-old-data
static_assert(std::is_trivially_constructible_v<AugCState>);
static_assert(std::is_trivially_copyable_v<AugCState>);
static_assert(std::is_pod_v<AugCState>);





// ============================================================================
// Globals
// ============================================================================

// We have one state but at some point we want more than 1?
inline AugCState gSingleState;

// ============================================================================
// Public functions
// ============================================================================

// Inline for optimisation reasons(i.e. hopefully gets elided into 1 address?)
[[gnu::always_inline]]
inline AugCState& State() noexcept
{
	return gSingleState;
}

}