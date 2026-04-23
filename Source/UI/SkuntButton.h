#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

// Gothic styled button with LED indicator
class SkuntButton : public juce::TextButton
{
public:
    SkuntButton();

    void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted,
                     bool shouldDrawButtonAsDown) override;

    void setLEDColour(juce::Colour c) { ledColour = c; }
    void setShowLED(bool show) { showLED = show; }

private:
    juce::Colour ledColour = juce::Colours::red;
    bool showLED = true;
};
