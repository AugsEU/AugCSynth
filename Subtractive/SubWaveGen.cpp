// ============================================================================
// Includes
// ============================================================================
#include "SubWaveGen.h"
#include "Oscillator.h"
#include "Tuning.h"
#include "Subtractive/SubParams.h"
#include "SubVoice.h"
#include "Voice.h"
#include "Parameters.h"
#include <Filter/NLFilter.h>
#include "Utilities/QwertyMath.h"
#include <Config.h>
#include <AugCState.h>


namespace AugCSynth::Subtractive
{

// ============================================================================
// Constants
// ============================================================================
#define DELAY_BUFFER_LEN 48000
#define DELAY_GLITCH_SIZE 2000
#define LOUDNESS_ALPHA (0.001f)


// ============================================================================
// Public functions
// ============================================================================

/// @brief Initialise synth.
void SynthInit(void)
{
	for(int i = 0; i < VOICE_POLYPHONY; i++)
	{
		State().mVoices[i].mSubVoice.Init();
	}

	SubState& subState = State().mSubState;

	subState.mDelayWriteHead = 0;
	subState.mDelayReadOffset = 0;
	subState.mDelayReadOffsetOffset = 0;
	for(int i = 0; i < DELAY_BUFFER_LEN; i++)
	{
		subState.mDelayBuffer[i] = 0;
	}

	subState.mLFO.Init();
	subState.mLFOWobbler.Init();

	subState.mCurrLoudness = 0.0f;

	ZeroOutParams();
	subState.mFilter.Init();

	// Default test preset
	SetFloatParam(ASP_GAIN, 0.7f);
	SetIntParam(ASP_DCO_WAVE_TYPE_1, OSC_MODE_SAW);
	SetFloatParam(ASP_DCO_TUNE_1, 1.0f);
	SetFloatParam(ASP_DCO_VOL_1, 0.75f);
	SetFloatParam(ASP_DCO_WS_1, 0.5f);

	SetIntParam(ASP_DCO_WAVE_TYPE_2, OSC_MODE_SQUARE);
	SetFloatParam(ASP_DCO_TUNE_2, 1.5f);
	SetFloatParam(ASP_DCO_VOL_2, 0.8f);
	SetFloatParam(ASP_DCO_WS_2, 0.2f);
	
	SetFloatParam(ASP_ENV_ATTACK1, 0.002);
	SetFloatParam(ASP_ENV_DECAY1, 0.000005f);
	SetFloatParam(ASP_ENV_SUSTAIN1, 0.76f);
	SetFloatParam(ASP_ENV_RELEASE1, 0.00005f);

	SetFloatParam(ASP_ENV_ATTACK2, 0.0002);
	SetFloatParam(ASP_ENV_DECAY2, 0.00005f);
	SetFloatParam(ASP_ENV_SUSTAIN2, 0.96f);
	SetFloatParam(ASP_ENV_RELEASE2, 0.0005f);

	SetFloatParam(ASP_LFO_RATE, 4.0f * (1.0f / (float)SAMPLE_RATE));
	SetFloatParam(ASP_LFO_WOBBLE, 0.2f * (1.0f / (float)SAMPLE_RATE));
	SetFloatParam(ASP_LFO_OSC1_VOLUME, 0.25f);
	SetFloatParam(ASP_LFO_VCF_CUTOFF, -0.2f);

	SetIntParam(ASP_VCF_MODE, FILTER_MODE_LP);
	SetFloatParam(ASP_VCF_RES, 0.4f);
	SetFloatParam(ASP_VCF_CUTOFF, 0.5f);

	SetIntParam(ASP_DELAY_MODE, DELAY_MODE_NORMAL);

	SetFloatParam(ASP_DELAY_TIME, 0.2f);
	SetFloatParam(ASP_DELAY_FEEDBACK, 0.2f);
}



/// @brief Fill sound buffer with number of samples.
/// Note: Buffer has to be twice as long as number of samples.
void FillSoundBuffer(int16_t* buf, uint16_t samples)
{
	uint16_t pos;
	int16_t* outp = buf;
	int32_t value, noDelayValue;
	int32_t delayValue;
	uint32_t rndValue = GetNextRand();

	// Delay
	uint32_t delayMode = GetIntParam(ASP_DELAY_MODE);

	SubState& subState = State().mSubState;
	
	if(delayMode == DELAY_MODE_GLITCH)
	{
		if(rndValue % 13 == 0)
		{
			if(rndValue % 3 == 0 && subState.mDelayReadOffsetOffset < DELAY_GLITCH_SIZE)
			{
				subState.mDelayReadOffsetOffset += DELAY_GLITCH_SIZE / 3;
			}
			else if (rndValue % 3 == 1 && subState.mDelayReadOffsetOffset > -DELAY_GLITCH_SIZE)
			{
				subState.mDelayReadOffsetOffset -= DELAY_GLITCH_SIZE / 3;
			}
		}
	}
	else
	{
		subState.mDelayReadOffsetOffset = 0;
	}

	int32_t delayReadOffset = GetFloatParam(ASP_DELAY_TIME) * DELAY_BUFFER_LEN + subState.mDelayReadOffsetOffset;
	if(delayMode == DELAY_MODE_OFF)
	{
		subState.mDelayReadOffset = 0;
		delayReadOffset = 0;
	}
	else
	{
		if(delayReadOffset < 0) 
		{
			delayReadOffset = 0;
			subState.mDelayReadOffsetOffset = 0;
		}
		else if(delayReadOffset >= DELAY_BUFFER_LEN)
		{
			delayReadOffset =  DELAY_BUFFER_LEN - 1;
			subState.mDelayReadOffsetOffset = 0;
		}
	}
	int32_t delayFeedbackVol = (uint32_t)(GetFloatParam(ASP_DELAY_FEEDBACK) * 32768.0f);
	uint16_t delayGlide = (uint16_t)(GetFloatParam(ASP_DELAY_SHEAR) * 275.0f) + 2;

	uint32_t delayReadHead;

	// DCO
	uint32_t waveType1 = GetIntParam(ASP_DCO_WAVE_TYPE_1);
	uint32_t waveType2 = GetIntParam(ASP_DCO_WAVE_TYPE_2);
	float tune1 = GetFloatParam(ASP_DCO_TUNE_1);
	float tune2 = GetFloatParam(ASP_DCO_TUNE_2);
	float shape1;// = 1.5f * GetFloatParam(ASP_DCO_WS_1) - 0.25f;
	float shape2;// = 1.5f * GetFloatParam(ASP_DCO_WS_2) - 0.25f;
	float shape1Lfo = GetFloatParam(ASP_LFO_OSC1_SHAPE);
	float shape2Lfo = GetFloatParam(ASP_LFO_OSC2_SHAPE);

	// VCF
	subState.mFilter.SetFilterType(GetIntParam(ASP_VCF_MODE));
	float filterFreqMod, filterFreq = GetFloatParam(ASP_VCF_CUTOFF);
	float filterRes = GetFloatParam(ASP_VCF_RES);
	float filterFreqLfo = GetFloatParam(ASP_LFO_VCF_CUTOFF);
	float filterResLfo = GetFloatParam(ASP_LFO_VCF_RES);
	float filterFollow = GetFloatParam(ASP_VCF_FOLLOW);

	// LFO
	float lfoValue;
	uint32_t lfoWaveSelect = GetIntParam(ASP_LFO_WAVE_TYPE);
	float lfoPhaseInc = GetFloatParam(ASP_LFO_RATE);
	float lfoWobblePhaseInc = lfoPhaseInc * 0.061804697157f;
	float lfoWobble = GetFloatParam(ASP_LFO_WOBBLE);

	// Drive & Gain
	float gain = GetFloatParam(ASP_GAIN);
	float drive = GetFloatParam(ASP_DRIVE);

	for(int i = 0; i < VOICE_POLYPHONY; i++)
	{
		State().mVoices[i].mSubVoice.PrepSampleBlock();
	}

	for (pos = 0; pos < samples; pos++)
	{		
		/*--- LFO ---*/
		subState.mLFO.PhaseInc(lfoPhaseInc * ComputeLfoMult(SineQuadraic(subState.mLFOWobbler.mPhase), lfoWobble));
		subState.mLFOWobbler.PhaseInc(lfoWobblePhaseInc);
		switch (lfoWaveSelect)
		{
		default:
		case OSC_MODE_SINE:
			lfoValue = SineQuadraic(subState.mLFO.mPhase);
			break;
		case OSC_MODE_SQUARE:
			lfoValue = SquareWaveLFO(subState.mLFO.mPhase);
			break;
		case OSC_MODE_SAW:
			lfoValue = SawWaveBLEP(subState.mLFO.mPhase, lfoPhaseInc);
			break;
		}

		/*--- Generate waveform ---*/
		float	y = 0.0f;
		shape1 = 1.5f * GetFloatParam(ASP_DCO_WS_1) * ComputeLfoMult(lfoValue, shape1Lfo) - 0.25f;
		shape2 = 1.5f * GetFloatParam(ASP_DCO_WS_2) * ComputeLfoMult(lfoValue, shape2Lfo) - 0.25f;

		for(int i = 0; i < VOICE_POLYPHONY; i++)
		{
			y += State().mVoices[i].mSubVoice.GetSample(
					waveType1, waveType2, 
					tune1, tune2, 
					shape1, shape2, 
					lfoValue);
		}

		/*--- Measure loudness ---*/
		float sampLoud = fabsf(y) * 6.0f;
		if(sampLoud > 1.0f) sampLoud = 1.0f;
		subState.mCurrLoudness = LOUDNESS_ALPHA * sampLoud + (1.0f - LOUDNESS_ALPHA) * subState.mCurrLoudness;

		/*--- Filter ---*/
		y *= (1.0f / (VOICE_POLYPHONY + 1.0f)); // Normalise
		filterFreqMod = ComputeLfoMult(lfoValue, filterFreqLfo);
		filterFreqMod *=  ComputeLoudnessMult(subState.mCurrLoudness, filterFollow);
		subState.mFilter.SetFilterFreq(filterFreq * filterFreqMod);
		subState.mFilter.SetFilterRes(filterRes * ComputeLfoMult(lfoValue, filterResLfo));
		y = subState.mFilter.NextSample(y);

		/*--- Drive & Gain ---*/
		y = drive * DrivenSample(y) + (1.0f - drive) * y;
		y *= gain;
		
		/*--- Delay ---*/
		value = (int32_t)((32767.0f) * y);

		// Delay read
		if ((pos % delayGlide) == 0)
		{
			if(subState.mDelayReadOffset < delayReadOffset)
			{
				subState.mDelayReadOffset += 1;
			}
			else if (subState.mDelayReadOffset > delayReadOffset)
			{
				subState.mDelayReadOffset -= 1;
			}
		}

		noDelayValue = value;
		if(subState.mDelayReadOffset > 0)
		{
			delayReadHead = (subState.mDelayWriteHead + DELAY_BUFFER_LEN - subState.mDelayReadOffset) % DELAY_BUFFER_LEN;
			delayValue = (int16_t)subState.mDelayBuffer[delayReadHead];
			value += delayValue;
		}

		/*--- Write to buffer ---*/
		if (value < -32768)
		{
			value = -32768;
		}
		else if (value > 32767)
		{
			value = 32767;
		}

		*outp++ = (int16_t)value;
		*outp++ = (int16_t)value;

		/*--- Delay write ---*/
		if (delayMode == DELAY_MODE_SLAPBACK)
		{
			value = noDelayValue; // Just write delay without feedback
		}

		value *= delayFeedbackVol;
		value /= 32768;

		subState.mDelayBuffer[subState.mDelayWriteHead] = (uint16_t)(int16_t)value;
		subState.mDelayWriteHead = (subState.mDelayWriteHead + 1) % DELAY_BUFFER_LEN;
	}
}

}