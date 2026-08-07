// ============================================================================
// Voice for subtractive synth
// ----------------------------------------------------------------------------
#pragma once
#include <pch.h>
// ============================================================================
// Include
// ============================================================================
#include "Tuning.h"
#include "Wave\WaveAll.h"
#include "Utilities/QwertyMath.h"

#include <Parameters.h>

// ============================================================================
// Types
// ============================================================================

namespace AugCSynth::Subtractive {

// Sound types
enum class PresetType
{
	Poly,
	Mono,
	Piano,
	Bass,
	Count
};

// X-macro defining each parameter
#define SUB_PARAMETER_TABLE(PARAM) \
	/*    Name              Type    Integer bounds      Unit float formula*/\
	PARAM(Tuning,           int,    Tuning::Count,      x)\
	PARAM(Drive,            float,  50,                 x*x)\
	PARAM(Gain,             float,  50,                 x*x)\
	/*Delay*/\
	PARAM(DelayTime,        float,  50,                 x)\
	PARAM(DelayFeedback,    float,  50,                 x)\
	PARAM(DelayShear,       float,  50,                 x)\
	PARAM(DelayMode,        int,    DelayMode::Count,   x)\
	/*DCO 1*/\
	PARAM(DcoWaveType1,     int,    WaveType::Count,    x)\
	PARAM(DcoTune1,         float,  -50,                ScaleFreqParam(x))\
	PARAM(DcoVol1,          float,  50,                 x*x)\
	PARAM(DcoWs1,           float,  20,                 x)\
	/*DCO 2*/\
	PARAM(DcoWaveType2,     int,    WaveType::Count,    x)\
	PARAM(DcoTune2,         float,  -50,                ScaleFreqParam(x))\
	PARAM(DcoVol2,          float,  50,                 x*x)\
	PARAM(DcoWs2,           float,  20,                 x)\
	/*ENV 1*/\
	PARAM(EnvAttack1,       float,  99,                 ScaleEnvParam(x))\
	PARAM(EnvDecay1,        float,  99,                 ScaleEnvParam(x))\
	PARAM(EnvSustain1,      float,  50,                 x)\
	PARAM(EnvRelease1,      float,  99,                 ScaleEnvParam(x))\
	/*ENV 2*/\
	PARAM(EnvAttack2,       float,  99,                 ScaleEnvParam(x))\
	PARAM(EnvDecay2,        float,  99,                 ScaleEnvParam(x))\
	PARAM(EnvSustain2,      float,  50,                 x)\
	PARAM(EnvRelease2,      float,  50,                 ScaleEnvParam(x))\
	/*VCF*/\
	PARAM(VcfCutoff,        float,  50,                 x*x)\
	PARAM(VcfRes,           float,  20,                 ScaleWeightHigh(x))\
	PARAM(VcfMode,          int,    FilterMode::Count,  x)\
	PARAM(VcfFollow,        float,  20,                 ScaleWeightHigh(x))\
	/*LFO*/\
	PARAM(LfoRate,          float,  99,                 ScaleLfoRate(x))\
	PARAM(LfoWaveType,      int,    WaveType::Count-1,  x)\
	PARAM(LfoAttack,        float,  99,                 ScaleEnvParam(x))\
	PARAM(LfoWobble,        float,  -20,                x*0.5f)\
	PARAM(LfoOsc1Tune,      float,  -20,                x*fabsf(x))\
	PARAM(LfoOsc1Volume,    float,  -20,                x*0.5f)\
	PARAM(LfoOsc1Shape,     float,  -20,                x*0.5f)\
	PARAM(LfoOsc2Tune,      float,  -20,                x*fabsf(x))\
	PARAM(LfoOsc2Volume,    float,  -20,                x*0.5f)\
	PARAM(LfoOsc2Shape,     float,  -20,                x*0.5f)\
	PARAM(LfoVcfCutoff,     float,  -20,                x*0.5f)\
	PARAM(LfoVcfRes,        float,  -20,                x*0.5f)\
	/*Sound type*/\
	PARAM(SoundType,        int,  PresetType::Count,    x)\
	




// ============================================================================
// Enum
// ============================================================================
#define DEFINE_ENUM(name, type, intMax, scaleFormula) name,
enum class SubParameter
{
	SUB_PARAMETER_TABLE(DEFINE_ENUM)
	Count
};
#undef DEFINE_ENUM





// ============================================================================
// Int bounds
// ============================================================================
#define BOUNDS_CASE(name, type, intMax, scaleFormula) \
	case SubParameter::name: return SynthParamBounds((int8_t)intMax);

inline SynthParamBounds GetSubParamBounds(SubParameter param)
{
	switch (param)
	{
	SUB_PARAMETER_TABLE(BOUNDS_CASE)
	default:
		break;
	}

	return SynthParamBounds(0);
}

#undef BOUNDS_CASE





// ============================================================================
// Float scaling
// ============================================================================
#define SCALE_CASE(name, type, intMax, scaleFormula) \
	case SubParameter::name: x = scaleFormula; break;

inline float ScaleFloatForSubParam(SubParameter paramNum, float x)
{
	switch(paramNum)
	{
	SUB_PARAMETER_TABLE(SCALE_CASE)
	default:
		break;
	}

	return x;
}

#undef SCALE_CASE





// ============================================================================
// Is this an int param?
// ============================================================================
#define IS_INT_CASE(name, type, intMax, scaleFormula) \
	case SubParameter::name: \
	{ \
		constexpr bool isInt_##name = (type)(0.5f) == 0; \
		return isInt_##name; \
	}

constexpr inline bool SubParamIsInt(SubParameter paramNum)
{
	switch(paramNum)
	{
	SUB_PARAMETER_TABLE(IS_INT_CASE)
	default:
		break;
	}

	return false;
}

#undef IS_INT_CASE

}
