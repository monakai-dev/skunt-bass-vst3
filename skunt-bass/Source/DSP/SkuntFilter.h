#pragma once
#include <juce_core/juce_core.h>

// Filter types: LP12, LP24, BP, HP
enum class FilterType { LP12 = 0, LP24, BP6, HP12 };

class SkuntFilter
{
public:
    SkuntFilter() = default;

    void setSampleRate(double sr) { sampleRate = sr; }
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

    // State Variable Filter state (TPT)
    float s1[2] = { 0.0f, 0.0f }; // first stage
    float s2[2] = { 0.0f, 0.0f }; // second stage (for LP24)

    // Precomputed coefficients
    float g = 0.0f;     // tan(pi * fc / sr)
    float R = 1.0f;     // damping (1 / (2Q))
    float h = 0.0f;     // common factor

    void updateCoefficients();
    float saturate(float x) const;
    float processSVFStage(float input, float& s, float& outLP, float& outBP, float& outHP);
};
