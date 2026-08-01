// ============================================================================
// Voice for subtractive synth
// ----------------------------------------------------------------------------
#pragma once
#include <pch.h>
// ============================================================================
// Types
// ============================================================================

namespace AugCSynth::Subtractive {

enum class SubParameter
{
	// General
	Tuning,
	Drive,
	Gain,
	// Delay
	DelayTime,
	DelayFeedback,
	DelayShear,
	DelayMode,
	// DCO
	DcoWaveType1,
	DcoTune1,
	DcoVol1,
	DcoWs1,
	DcoWaveType2,
	DcoTune2,
	DcoVol2,
	DcoWs2,
	// Env
	EnvAttack1,
	EnvDecay1,
	EnvSustain1,
	EnvRelease1,
	EnvAttack2,
	EnvDecay2,
	EnvSustain2,
	EnvRelease2,
	// Filter
	VcfCutoff,
	VcfRes,
	VcfMode,
	VcfFollow,
	// LFO
	LfoRate,
	LfoWaveType,
	LfoAttack,
	LfoWobble,
	LfoOsc1Tune,
	LfoOsc1Volume,
	LfoOsc1Shape,
	LfoOsc2Tune,
	LfoOsc2Volume,
	LfoOsc2Shape,
	LfoVcfCutoff,
	LfoVcfRes,
	SoundType,
	Count
};

// Sound types
enum class PresetType
{
	Poly,
	Mono,
	Piano,
	Bass,
};

}
