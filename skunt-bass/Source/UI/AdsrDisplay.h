#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

// Small ADSR envelope visualization
class AdsrDisplay : public juce::Component
{
public:
    AdsrDisplay();

    void paint(juce::Graphics& g) override;

    void setAttack(float aMs)  { attackMs  = aMs;  repaint(); }
    void setDecay(float dMs)   { decayMs   = dMs;  repaint(); }
    void setSustain(float s)   { sustain   = s;    repaint(); }
    void setRelease(float rMs) { releaseMs = rMs;  repaint(); }

    void setActive(bool a) { isActive = a; repaint(); }
    void triggerAnimation();

private:
    float attackMs = 10.0f;
    float decayMs = 400.0f;
    float sustain = 0.5f;
    float releaseMs = 500.0f;
    bool isActive = false;
    float animationPhase = 0.0f;

    void drawEnvelopePath(juce::Graphics& g);
};
