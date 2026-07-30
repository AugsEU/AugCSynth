// ============================================================================
// Inline
// ============================================================================
#include "WaveAll.h"
#include <Utilities/QwertyMath.h>

namespace AugCSynth {

// ============================================================================
// Public functions
// ============================================================================

float GetWaveSample(float phase, float dt, WaveType waveType, float shape)
{
	float out;
	switch (waveType)
    {
    default:
    case WaveType::Sine:
        out = SineQuadraic(phase);
        out = ShapeWave(out, shape);
        break;
    case WaveType::Square:
        out = SquareWaveBLEPShape(phase, dt, shape);
        break;
    case WaveType::Saw:
        out = SawWaveBLEP(phase, dt);
        out = ShapeWave(out, shape);
        break;
    case WaveType::Organ:
        out = SquareWaveOrgan(phase, shape);
        break;
    }

	return out;
}


float GetWaveLFO(float phase, WaveType waveType)
{
	float out;
	switch (waveType)
	{
		default:
		case WaveType::Sine:
			out = SineQuadraic(phase);
			break;
		case WaveType::Square:
			out = SquareWaveLFO(phase);
			break;
		case WaveType::Saw :
			out = SawWaveLFO(phase);
			break;
	}

	return out;
}

}