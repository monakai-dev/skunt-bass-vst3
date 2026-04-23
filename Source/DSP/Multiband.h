#pragma once
#include <juce_core/juce_core.h>

// Simple 3-band crossover using 12dB/octave filters
class MultibandCrossover
{
public:
    MultibandCrossover() = default;

    void setSampleRate(double sr);
    void setCrossoverFrequencies(float lowFreq, float highFreq);
    void reset();

    // Process one sample, outputs for lo, mid, hi bands
    void process(float input, float& lo, float& mid, float& hi);

private:
    double sampleRate = 48000.0;
    float xoverLow = 150.0f;
    float xoverHigh = 4000.0f;

    // LP coefficients for low band
    float lp1_a1 = 0.0f, lp1_b0 = 1.0f;
    float lp2_a1 = 0.0f, lp2_b0 = 1.0f;
    // HP coefficients for hi band  
    float hp1_a1 = 0.0f, hp1_b0 = 1.0f;
    float hp2_a1 = 0.0f, hp2_b0 = 1.0f;

    // Filter states
    float lp1_s = 0.0f, lp2_s = 0.0f;
    float hp1_s = 0.0f, hp2_s = 0.0f;
    float ap1_s = 0.0f, ap2_s = 0.0f;

    void updateCoefficients();
};
