#pragma once
#include <juce_core/juce_core.h>

class Compressor
{
public:
    Compressor() = default;

    void setSampleRate(double sr);
    void setThreshold(float threshDb) { thresholdDb = threshDb; }
    void setRatio(float ratio01); // mapped 1:1 to 20:1

    void reset();
    float process(float input);

private:
    double sampleRate = 48000.0;
    float thresholdDb = -20.0f;
    float ratio = 4.0f; // actual ratio (1-20)

    // Envelope followers
    float envLevel = 0.0f;
    float attackCoeff = 0.99f;
    float releaseCoeff = 0.995f;
    float makeupGain = 1.0f;

    void updateCoefficients();
    static float dbToGain(float db) { return std::pow(10.0f, db * 0.05f); }
    static float gainToDb(float g) { return 20.0f * log10f(g + 1e-10f); }
};
