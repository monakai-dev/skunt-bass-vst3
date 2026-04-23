#pragma once
#include <juce_core/juce_core.h>

// Filter types: LP12, LP24, BP, HP
enum class FilterType { LP12 = 0, LP24, BP6, HP12 };

class SkuntFilter
{
public:
    SkuntFilter() = default;

    void setSampleRate(double sr) { sampleRate = sr; updateCoefficients(); }
    void setType(FilterType t) { type = t; }
    void setCutoff(float freqHz);
    void setResonance(float q); // 0-1
    void setDrive(float driveAmount) { drive = driveAmount; }

    void reset();
    float process(float input);

private:
    double sampleRate = 48000.0;
    FilterType type = FilterType::LP12;
    float cutoff = 2000.0f;
    float resonance = 0.3f;
    float drive = 0.0f;

    // Two independent biquad stages for LP24 cascade
    // Stage 1
    float a1_1 = 0.0f, a2_1 = 0.0f;
    float b0_1 = 1.0f, b1_1 = 0.0f, b2_1 = 0.0f;
    float z1_1 = 0.0f, z2_1 = 0.0f;

    // Stage 2 (for LP24)
    float a1_2 = 0.0f, a2_2 = 0.0f;
    float b0_2 = 1.0f, b1_2 = 0.0f, b2_2 = 0.0f;
    float z1_2 = 0.0f, z2_2 = 0.0f;

    void updateCoefficients();
    float saturate(float x) const;
    float processBiquad(float input, float a1, float a2, float b0, float b1, float b2, float& z1, float& z2);
};
