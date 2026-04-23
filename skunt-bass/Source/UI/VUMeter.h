#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

// Vertical LED VU meter with peak hold
class VUMeter : public juce::Component, public juce::Timer
{
public:
    VUMeter();

    void paint(juce::Graphics& g) override;
    void timerCallback() override;

    void setLevel(float levelLinear); // 0-1+
    void setNumChannels(int ch) { numChannels = ch; repaint(); }

private:
    float currentLevel = 0.0f;
    float peakLevel = 0.0f;
    float holdLevel = 0.0f;
    int holdCounter = 0;
    int numChannels = 2;
    static constexpr int holdTimeMs = 1000;
    static constexpr int numSegments = 20;

    juce::Colour getSegmentColour(int segment, int total) const;
};
