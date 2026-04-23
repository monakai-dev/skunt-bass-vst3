#pragma once
#include <juce_core/juce_core.h>

class Envelope
{
public:
    Envelope() = default;

    void setSampleRate(double sr) { sampleRate = sr; }
    void setParameters(float attackMs, float decayMs, float sustainLevel, float releaseMs);

    void noteOn() { stage = Stage::Attack; }
    void noteOff() { if (stage != Stage::Idle) stage = Stage::Release; }

    float process();
    float getOutput() const { return currentLevel; }
    bool isActive() const { return stage != Stage::Idle; }
    bool isReleasing() const { return stage == Stage::Release; }
    void reset();

private:
    enum class Stage { Idle, Attack, Decay, Sustain, Release };
    Stage stage = Stage::Idle;
    double sampleRate = 48000.0;
    float currentLevel = 0.0f;
    float attackCoeff = 0.0f;
    float decayCoeff = 0.0f;
    float sustainLevel = 0.0f;
    float releaseCoeff = 0.0f;

    float msToCoeff(float ms) const;
};
