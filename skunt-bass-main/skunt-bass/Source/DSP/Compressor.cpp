#include "Compressor.h"
#include <cmath>

void Compressor::setSampleRate(double sr)
{
    sampleRate = sr;
    updateCoefficients();
}

void Compressor::setRatio(float ratio01)
{
    ratio = 1.0f + ratio01 * 19.0f;
    updateCoefficients();
}

void Compressor::updateCoefficients()
{
    // Attack: 2ms, Release: 100ms
    attackCoeff = std::exp(-1.0f / (sampleRate * 0.002f));
    releaseCoeff = std::exp(-1.0f / (sampleRate * 0.1f));
    // Makeup gain compensates 50% of max reduction
    float maxReduction = thresholdDb * (1.0f - 1.0f / ratio);
    makeupGain = dbToGain(-maxReduction * 0.5f);
}

void Compressor::reset()
{
    envLevel = 0.0f;
}

float Compressor::process(float input)
{
    float absInput = std::abs(input);

    // Level detection with different attack/release
    if (absInput > envLevel)
        envLevel = attackCoeff * envLevel + (1.0f - attackCoeff) * absInput;
    else
        envLevel = releaseCoeff * envLevel + (1.0f - releaseCoeff) * absInput;

    // Gain reduction in dB
    float envDb = gainToDb(envLevel);
    float grDb = 0.0f;
    if (envDb > thresholdDb)
        grDb = (thresholdDb - envDb) * (1.0f - 1.0f / ratio);

    // Convert to linear gain
    float gain = dbToGain(grDb);

    return input * gain * makeupGain;
}
