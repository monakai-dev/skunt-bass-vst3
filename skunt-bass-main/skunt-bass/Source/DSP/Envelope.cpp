#include "Envelope.h"
#include <cmath>

void Envelope::setParameters(float attackMs, float decayMs, float sustainLevel, float releaseMs)
{
    attackCoeff  = msToCoeff(attackMs);
    decayCoeff   = msToCoeff(decayMs);
    this->sustainLevel = juce::jlimit(0.0f, 1.0f, sustainLevel);
    releaseCoeff = msToCoeff(releaseMs);
}

float Envelope::process()
{
    switch (stage)
    {
        case Stage::Idle:
            currentLevel = 0.0f;
            break;

        case Stage::Attack:
            currentLevel += (1.0f - currentLevel) * attackCoeff;
            if (currentLevel > 0.999f)
            {
                currentLevel = 1.0f;
                stage = Stage::Decay;
            }
            break;

        case Stage::Decay:
            currentLevel += (sustainLevel - currentLevel) * decayCoeff;
            if (std::abs(currentLevel - sustainLevel) < 0.001f)
                stage = Stage::Sustain;
            break;

        case Stage::Sustain:
            currentLevel = sustainLevel;
            break;

        case Stage::Release:
            currentLevel += (0.0f - currentLevel) * releaseCoeff;
            if (currentLevel < 0.0001f)
            {
                currentLevel = 0.0f;
                stage = Stage::Idle;
            }
            break;
    }

    return currentLevel;
}

void Envelope::reset()
{
    stage = Stage::Idle;
    currentLevel = 0.0f;
}

float Envelope::msToCoeff(float ms) const
{
    if (ms < 1.0f) ms = 1.0f;
    return 1.0f - std::exp(-1.0f / (sampleRate * ms * 0.001f));
}
