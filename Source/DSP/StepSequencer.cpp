#include "StepSequencer.h"
#include <cmath>

void StepSequencer::setRate(float freqHz)
{
    freqHz = juce::jlimit(0.01f, 50.0f, freqHz);
    phaseInc = freqHz / static_cast<float>(sampleRate);
}

void StepSequencer::setStepValue(int stepIndex, float value01)
{
    if (stepIndex >= 0 && stepIndex < 8)
        stepValues[stepIndex] = juce::jlimit(0.0f, 1.0f, value01);
}

float StepSequencer::process()
{
    phase += phaseInc;
    while (phase >= 1.0f) phase -= 1.0f;

    int newStep = static_cast<int>(phase * 8.0f) & 7;

    if (newStep != currentStep)
    {
        currentStep = newStep;
        lastStepValue = currentValue;
    }

    float target = stepValues[currentStep];

    // Smooth interpolation between steps
    float stepPhase = (phase * 8.0f) - static_cast<float>(currentStep);
    float interp = lastStepValue + (target - lastStepValue) * stepPhase;
    currentValue += (interp - currentValue) * (0.01f + smoothAmount * 0.3f);

    return currentValue;
}
