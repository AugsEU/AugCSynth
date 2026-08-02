
# AugCSynth

Fairly portable synth library written in C++. Mainly designed for embedded projects. Fairly performant.

So far the library only supports one global instance. This may change in the future. The reason for this is performance (i.e. addresses can be statically determined instead of loaded at runtime).


# Usage

First initialise the library on launch:

```cpp
AugCSynth::Initialise();
```

Then to start a note playing call

```cpp
AugCSynth::BeginVoice(noteNumber);
```

to stop a note:

```cpp
AugCSynth::ReleaseVoice(noteNumber); // lets note play out
// or
AugCSynth::StopVoice(noteNumber); // stops note dead
```

Then call periodically to fill sample buffers:

```cpp
AugCSynth::FillSoundBuffer((int16_t*)out, (uint16_t)len);
```

To set parameters you can call:

```cpp
AugCSynth::SetFloatParam(AugCSynth::Subtractive::SubParameter::DcoTune1, 0.4f);
// or
AugCSynth::SetIntParam(AugCSynth::Subtractive::SubParameter::Tuning, (int)AugCSynth::Tuning::Equal24);
```

The exact nature, acceptable ranges, and values of each parameter are yet to be documented. Hopefully a cleaner API will come down the line.

Some parameters are continuously stored in a float. Other parameters, like the tuning, correspond to integer values which map on to an enum. Setting a parameters to the wrong type (e.g. float to an int) is UB.

# To do

This library is still work in progress. Right now only the "Subtrative synth" model exists, but in future maybe more will:

- Needs cleaner way of configuring than config.h
- Needs to support multiple instances
- Needs to support non 16-bit pcm formats.