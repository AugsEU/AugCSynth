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
	DelayMode delayMode = (DelayMode)GetIntParam(SubParameter::DelayMode);

	SubState& subState = State().mSubState;
	
	if(delayMode == DelayMode::Glitch)
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

	int32_t delayReadOffset = GetFloatParam(SubParameter::DelayTime) * DELAY_BUFFER_LEN + subState.mDelayReadOffsetOffset;
	if(delayMode == DelayMode::Off)
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
	int32_t delayFeedbackVol = (uint32_t)(GetFloatParam(SubParameter::DelayFeedback) * 32768.0f);
	uint16_t delayGlide = (uint16_t)(GetFloatParam(SubParameter::DelayShear) * 275.0f) + 2;

	uint32_t delayReadHead;

	// DCO
	WaveType waveType1 = (WaveType)GetIntParam(SubParameter::DcoWaveType1);
	WaveType waveType2 = (WaveType)GetIntParam(SubParameter::DcoWaveType2);
	float tune1 = GetFloatParam(SubParameter::DcoTune1);
	float tune2 = GetFloatParam(SubParameter::DcoTune2);
	float shape1;// = 1.5f * GetFloatParam(SubParameter::DCO_WS_1) - 0.25f;
	float shape2;// = 1.5f * GetFloatParam(SubParameter::DCO_WS_2) - 0.25f;
	float shape1Lfo = GetFloatParam(SubParameter::LfoOsc1Shape);
	float shape2Lfo = GetFloatParam(SubParameter::LfoOsc2Shape);

	// VCF
	subState.mFilter.SetFilterType((FilterMode)GetIntParam(SubParameter::VcfMode));
	float filterFreqMod, filterFreq = GetFloatParam(SubParameter::VcfCutoff);
	float filterRes = GetFloatParam(SubParameter::VcfRes);
	float filterFreqLfo = GetFloatParam(SubParameter::LfoVcfCutoff);
	float filterResLfo = GetFloatParam(SubParameter::LfoVcfRes);
	float filterFollow = GetFloatParam(SubParameter::VcfFollow);

	// LFO
	float lfoValue;
	WaveType lfoWaveSelect = (WaveType)GetIntParam(SubParameter::LfoWaveType);
	float lfoPhaseInc = GetFloatParam(SubParameter::LfoRate);
	float lfoWobblePhaseInc = lfoPhaseInc * 0.061804697157f;
	float lfoWobble = GetFloatParam(SubParameter::LfoWobble);

	// Drive & Gain
	float gain = GetFloatParam(SubParameter::Gain);
	float drive = GetFloatParam(SubParameter::Drive);

	for(int i = 0; i < VOICE_POLYPHONY; i++)
	{
		State().mVoices[i].mSubVoice.PrepSampleBlock();
	}

	for (pos = 0; pos < samples; pos++)
	{		
		/*--- LFO ---*/
		subState.mLFO.PhaseInc(lfoPhaseInc * ComputeLfoMult(SineQuadraic(subState.mLFOWobbler.mPhase), lfoWobble));
		subState.mLFOWobbler.PhaseInc(lfoWobblePhaseInc);
		lfoValue = GetWaveLFO(subState.mLFO.mPhase, (WaveType)lfoWaveSelect);

		/*--- Generate waveform ---*/
		float	y = 0.0f;
		shape1 = 1.5f * GetFloatParam(SubParameter::DcoWs1) * ComputeLfoMult(lfoValue, shape1Lfo) - 0.25f;
		shape2 = 1.5f * GetFloatParam(SubParameter::DcoWs2) * ComputeLfoMult(lfoValue, shape2Lfo) - 0.25f;

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
		if (delayMode == DelayMode::Slapback)
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