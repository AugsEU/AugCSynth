// ============================================================================
// Synth parameters
// ----------------------------------------------------------------------------
#pragma once
#include <pch.h>
// ============================================================================
// Includes
// ============================================================================
#include <math.h>
#include <stdint.h>

#include "AugCState.h"

namespace AugCSynth
{

// ============================================================================
// Public functions
// ============================================================================

/// @brief Set all parameters to zero
void ZeroOutParams();



/// @brief Get a floating point param
/// @param idx Index of param
float GetFloatParam(size_t idx);

/// @brief Set a floating point param
/// @param idx Index of param
/// @param param Value of param
void SetFloatParam(size_t idx, float param);



/// @brief Get an int param
/// @param idx Index of param
uint32_t GetIntParam(size_t idx);

/// @brief Set an int param
/// @param idx Index of param
/// @param param Value of param
void SetIntParam(size_t idx, uint32_t param);



/// @brief Get a float param from enum type
/// @tparam T Enum type
/// @param idx Index of param 
template <typename T>
inline float GetFloatParam(T idx) { return GetFloatParam((size_t)idx); }

/// @brief Set a floating point param from enum type#
/// @tparam T Enum type
/// @param idx Index of param
/// @param param Value of param
template <typename T>
inline void SetFloatParam(T idx, float param) { SetFloatParam((size_t)idx, param); }




/// @brief Get an int param from enum type
/// @tparam T Enum type
/// @param idx Index of param 
template <typename T>
inline int GetIntParam(T idx) { return GetIntParam((size_t)idx); }

/// @brief Set an int point param from enum type#
/// @tparam T Enum type
/// @param idx Index of param
/// @param param Value of param
template <typename T>
inline void SetIntParam(T idx, uint32_t param) { SetIntParam((size_t)idx, param); }



/// @brief Get current synth mode
SynthMode GetSynthMode();

/// @brief Set current synth mode
void SetSynthMode(SynthMode mode);

} // namespace AugCSynth
