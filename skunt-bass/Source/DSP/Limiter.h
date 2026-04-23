#pragma once
#include <juce_core/juce_core.h>

// Simple lookahead brickwall limiter
class Limiter
{
public:
    Limiter() = default;

    void setSampleRate(double sr);
    void reset();
    float process(float input);

private:
    double sampleRate = 48000.0;
    static constexpr int lookaheadMs = 10;
    static constexpr int maxLookaheadSamples = 4800; // at 48k * 0.1s

    float delayLine[maxLookaheadSamples] = {};
    int writeIdx = 0;
    int lookaheadSamples = 480;
    float envelope = 1.0f;
    float releaseCoeff = 0.999f;
};
