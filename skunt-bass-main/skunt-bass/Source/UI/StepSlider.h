#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

// Vertical step slider for the 8-step sequencer
class StepSlider : public juce::Slider
{
public:
    StepSlider(int stepIndex = 0);

    void paint(juce::Graphics& g) override;
    void resized() override;

    void setIsActiveStep(bool active) { isActiveStep = active; repaint(); }

private:
    int stepNumber = 0;
    bool isActiveStep = false;
};
