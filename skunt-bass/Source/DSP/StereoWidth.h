#pragma once
#include <juce_core/juce_core.h>

// M/S stereo width processor with mono bass protection
class StereoWidth
{
public:
    StereoWidth() = default;

    void setSampleRate(double sr);
    void setWidth(float width01); // 0=mono, 1=full width
    void reset();

    // Process stereo pair
    void process(float& left, float& right);

private:
    double sampleRate = 48000.0;
    float width = 0.5f;
    float monoCrossoverFreq = 120.0f;

    // LP filter for mono bass path
    float lpCoeff = 1.0f;
    float lpStateL = 0.0f;
    float lpStateR = 0.0f;

    void updateCoefficients();
};
