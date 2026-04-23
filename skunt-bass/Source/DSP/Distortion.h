#pragma once
#include <juce_core/juce_core.h>

// Distortion modes
enum class DistortionType { Tube = 0, Foldback, Bitcrush };

class Distortion
{
public:
    Distortion() = default;

    void setSampleRate(double sr) { sampleRate = sr; }
    void setType(DistortionType t) { type = t; }
    void setAmount(float amt) { amount = amt; }
    void setMix(float mix01) { mix = mix01; }
    void reset() { lastOutput = 0.0f; bitPhase = 0.0f; }

    float process(float input);

private:
    double sampleRate = 48000.0;
    DistortionType type = DistortionType::Tube;
    float amount = 0.5f;
    float mix = 1.0f;
    float lastOutput = 0.0f;
    float bitPhase = 0.0f;

    float processTube(float x);
    float processFoldback(float x);
    float processBitcrush(float x);
};
