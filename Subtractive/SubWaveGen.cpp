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
#include "Utilities/SynthMath.h"
#include <Config.h>
#include <AugCState.h>
#include <Delay/Delay.h>
#include <Drive/Drive.h>
#include <Arduino.h>


namespace AugCSynth::Subtractive
{
#ifndef AUGCSYNTH_NO_STATE
// ============================================================================
// Constants
// ============================================================================
#define DELAY_BUFFER_LEN 48000

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

	subState.mDelay.Init();

	subState.mLFO.Init();
	subState.mLFOWobbler.Init();

	ZeroOutParams();
}



/// @brief Fill sound buffer with number of samples.
/// Note: Buffer has to be twice as long as number of samples.
void FillSoundBuffer(int16_t* buf, uint16_t samples)
{
	uint16_t pos;
	int16_t* outp = buf;
	int32_t value;

	SubState& subState = State().mSubState;

	// Delay
	DelayMode delayMode = (DelayMode)GetIntParam(SubParameter::DelayMode);
	int32_t delayReadOffset = GetFloatParam(SubParameter::DelayTime) * DELAY_BUFFER_LEN;
	uint32_t delayFeedbackVol = (uint32_t)(GetFloatParam(SubParameter::DelayFeedback) * 32768.0f);
	uint16_t delayGlide = (uint16_t)(GetFloatParam(SubParameter::DelayShear) * 275.0f) + 2;

	subState.mDelay.SetParams(delayMode, delayReadOffset, delayFeedbackVol, delayGlide);

	// DCO
	WaveType waveType1 = (WaveType)GetIntParam(SubParameter::DcoWaveType1);
	WaveType waveType2 = (WaveType)GetIntParam(SubParameter::DcoWaveType2);
	float tune1 = GetFloatParam(SubParameter::DcoTune1);
	float tune2 = GetFloatParam(SubParameter::DcoTune2);
	float shape1;// = 1.5f * GetFloatParam(SubParameter::DCO_WS_1) - 0.25f;
	float shape2;// = 1.5f * GetFloatParam(SubParameter::DCO_WS_2) - 0.25f;
	float shape1Lfo = GetFloatParam(SubParameter::LfoOsc1Shape);
	float shape2Lfo = GetFloatParam(SubParameter::LfoOsc2Shape);

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
		y *= (1.0f / (VOICE_POLYPHONY + 1.0f));

		/*--- Drive & Gain ---*/
		y = DriveSample(DriveMode::RatioDrive, y, drive);
		y *= gain;
		
		/*--- Delay ---*/
		value = (int32_t)((32767.0f) * y);

		/*--- Write to buffer ---*/
		if (value < -32768)
		{
			value = -32768;
		}
		else if (value > 32767)
		{
			value = 32767;
		}

		int16_t value16 = (int16_t)value;
		printf("V: %d\n", value16);
		value16 = subState.mDelay.GetNextSample(value16);

		*outp++ = value16;
		*outp++ = value16;
	}
}
#endif // !AUGCSYNTH_NO_STATE
}