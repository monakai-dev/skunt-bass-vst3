#include "LFO.h"
#include <cmath>

void LFO::setFrequency(float freqHz)
{
    freqHz = juce::jlimit(0.01f, 100.0f, freqHz);
    phaseInc = freqHz / static_cast<float>(sampleRate);
}

float LFO::process()
{
    phase += phaseInc;
    while (phase >= 1.0f) phase -= 1.0f;

    float output = 0.0f;

    switch (currentShape)
    {
        case LFOShape::Sine:
            output = sineShape();
            break;
        case LFOShape::Triangle:
            output = triangleShape();
            break;
        case LFOShape::Saw:
            output = sawShape();
            break;
        case LFOShape::Square:
            output = squareShape();
            break;
        case LFOShape::SampleAndHold:
            output = sampleAndHoldShape();
            break;
        case LFOShape::SmoothedNoise:
            output = smoothedNoiseShape();
            break;
    }

    lastOutput = output;
    return output;
}

float LFO::sineShape() const
{
    return std::sin(phase * juce::MathConstants<float>::twoPi);
}

float LFO::triangleShape() const
{
    float t = phase;
    if (t < 0.25f)
        return t * 4.0f;
    else if (t < 0.75f)
        return 2.0f - t * 4.0f;
    else
        return t * 4.0f - 4.0f;
}

float LFO::sawShape() const
{
    return phase * 2.0f - 1.0f;
}

float LFO::squareShape() const
{
    return phase < 0.5f ? 1.0f : -1.0f;
}

float LFO::sampleAndHoldShape()
{
    if (phase < phaseInc) // just wrapped around
    {
        holdValue = rng.nextFloat() * 2.0f - 1.0f;
    }
    return holdValue;
}

float LFO::smoothedNoiseShape()
{
    if (phase < phaseInc)
    {
        lastOutput = holdValue;
        holdValue = rng.nextFloat() * 2.0f - 1.0f;
    }
    // linear interpolation
    float t = phase;
    return lastOutput + t * (holdValue - lastOutput);
}
