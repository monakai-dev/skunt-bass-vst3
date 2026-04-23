#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_dsp/juce_dsp.h>

// Small oscilloscope / waveform display
class ScopeComponent : public juce::Component, public juce::Timer
{
public:
    ScopeComponent();

    void paint(juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;

    void pushSample(float sample);
    void setColour(juce::Colour c) { waveColour = c; }

private:
    static constexpr int bufferSize = 512;
    float buffer[bufferSize] = {};
    int writePos = 0;
    juce::Colour waveColour = juce::Colour(0xFF8B0000);
};
