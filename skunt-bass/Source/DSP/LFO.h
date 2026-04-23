#pragma once
#include <juce_core/juce_core.h>

// LFO shapes: Sine, Triangle, Saw, Square, S&H, Smoothed Noise
enum class LFOShape { Sine = 0, Triangle, Saw, Square, SampleAndHold, SmoothedNoise };

class LFO
{
public:
    LFO() = default;

    void setSampleRate(double sr) { sampleRate = sr; }
    void setFrequency(float freqHz);
    void setShape(LFOShape shape) { currentShape = shape; }
    void setPhase(float phase01) { phase = phase01; }

    void reset() { phase = 0.0f; lastOutput = 0.0f; holdValue = 0.0f; }
    float process(); // returns [-1, 1]

private:
    double sampleRate = 48000.0;
    float phase = 0.0f;
    float phaseInc = 0.0f;
    LFOShape currentShape = LFOShape::Sine;
    float lastOutput = 0.0f;
    float holdValue = 0.0f;
    juce::Random rng;

    float sineShape() const;
    float triangleShape() const;
    float sawShape() const;
    float squareShape() const;
    float sampleAndHoldShape();
    float smoothedNoiseShape();
};
