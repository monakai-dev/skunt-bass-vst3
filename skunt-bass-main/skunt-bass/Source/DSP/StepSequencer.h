#pragma once
#include <juce_core/juce_core.h>

class StepSequencer
{
public:
    StepSequencer() = default;

    void setSampleRate(double sr) { sampleRate = sr; }
    void setRate(float freqHz);
    void setSmoothing(float amount01) { smoothAmount = amount01; }
    void setStepValue(int stepIndex, float value01);

    void reset() { phase = 0.0f; currentValue = 0.0f; lastStepValue = 0.0f; }
    float process(); // returns [0, 1]
    float getOutput() const { return currentValue; }
    int getCurrentStep() const { return currentStep; }

private:
    double sampleRate = 48000.0;
    float phase = 0.0f;
    float phaseInc = 0.0f;
    float smoothAmount = 0.1f;
    float stepValues[8] = { 1.0f, 0.3f, 0.7f, 0.1f, 0.9f, 0.2f, 0.6f, 0.4f };
    float currentValue = 0.0f;
    float lastStepValue = 0.0f;
    int currentStep = 0;
};
