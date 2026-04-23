#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "GothicCamoLookAndFeel.h"

// Custom rotary knob with blood-red arc and real parameter value display
class SkuntKnob : public juce::Slider
{
public:
    SkuntKnob();

    enum ColourIds
    {
        trackColourId = 0x3000100,
        fillColourId  = 0x3000101,
        textColourId  = 0x3000102
    };

    void setLabel(const juce::String& lbl) { labelText = lbl; }
    void setSuffix(const juce::String& sfx) { suffix = sfx; }
    
    // Set the REAL parameter range for display (not 0-1)
    void setRealRange(float minVal, float maxVal) { realMin = minVal; realMax = maxVal; }
    void setDisplayDecimals(int dec) { decimals = dec; }

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::String labelText;
    juce::String suffix;
    std::unique_ptr<juce::Label> valueLabel;
    float realMin = 0.0f;
    float realMax = 1.0f;
    int decimals = 1;

    float getStartAngle() const { return juce::MathConstants<float>::pi * 1.2f; }
    float getEndAngle() const   { return juce::MathConstants<float>::pi * 2.8f; }
};
