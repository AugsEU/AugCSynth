// ============================================================================
// Include
// ============================================================================
#include "Drive.h"


namespace AugCSynth {

// ============================================================================
// Private functions
// ============================================================================

#define DRIVE_K (0.9f)
#define DRIVE_M (4.0f*DRIVE_K + 1.0f)

#define DRIVE_A (DRIVE_K*DRIVE_M - DRIVE_K)
#define DRIVE_B ((DRIVE_K+1.0f)*(1.0f-DRIVE_M))
#define DRIVE_C (DRIVE_M)

/// @brief Positive part of polynomial drive
float PolynomialDrivePositve(float input, float drive)
{
	const float k = drive;
	const float m = 4*k;

	const float a = k*m;
	const float b = -(k+1.0f)*m;
	const float c = m+1.0f;

	float x = input;
	
	return ((a*x+b)*x+c)*x;
}

// ============================================================================
// Public functions
// ============================================================================

float DriveSample(DriveMode mode, float input, float drive)
{
	switch (mode)
	{
	case DriveMode::PolyDrive:
		return PolynomialDrive(input, drive);
	case DriveMode::RatioDrive:
		return RationalDrive(input, drive);
	case DriveMode::Off:
	default:
		break;
	}

	return input;
}



float PolynomialDrive(float input, float drive)
{
    if(signbit(input))
    {
        return -PolynomialDrivePositve(-input, drive);
    }

    return PolynomialDrivePositve(input, drive);;
}



float RationalDrive(float input, float drive)
{
	constexpr float RATIONAL_DRIVE_SCALE_FACTOR = 20.0f;

	drive *= RATIONAL_DRIVE_SCALE_FACTOR;

	float a = input*drive;
	
	input += a;
	a = fabs(a) + 1.0f;
	input /= a;

	return input;
}

}