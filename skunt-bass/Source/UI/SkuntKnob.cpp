#include "SkuntKnob.h"

SkuntKnob::SkuntKnob()
{
    setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    setRange(0.0, 1.0, 0.001);
}

void SkuntKnob::paint(juce::Graphics& g)
{
    juce::Slider::paint(g);

    auto bounds = getLocalBounds();
    float cx = bounds.getCentreX();
    float labelY = bounds.getBottom() - 12;

    // Value text
    if (!suffix.isEmpty())
    {
        g.setColour(juce::Colour(0xFFE8E0D4));
        g.setFont(juce::Font(juce::FontOptions(11.0f)));
        juce::String valText = juce::String(getValue(), 1) + suffix;
        g.drawFittedText(valText, bounds.withBottom(labelY + 10).withTop(labelY - 10),
                          juce::Justification::centred, 1);
    }

    // Label text
    if (!labelText.isEmpty())
    {
        g.setColour(juce::Colour(0xFF8A8578));
        g.setFont(juce::Font(juce::FontOptions(11.0f)));
        g.drawFittedText(labelText, bounds.withBottom(14).withTop(0),
                          juce::Justification::centred, 1);
    }
}

void SkuntKnob::resized()
{
    juce::Slider::resized();
}
