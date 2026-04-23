#include "SkuntKnob.h"

SkuntKnob::SkuntKnob()
{
    setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
}

void SkuntKnob::paint(juce::Graphics& g)
{
    juce::Slider::paint(g);

    auto bounds = getLocalBounds();
    const int valFont = 15;
    const int labelFont = 12;
    const int textStrip = 28;

    // Reserve bottom strip for text
    auto textArea = bounds.removeFromBottom(textStrip);

    // VALUE TEXT — always show (this was the #1 bug: suffix.isEmpty() blocked it)
    float val = getValue();
    juce::String valueStr;
    if (decimals == 0)
        valueStr = juce::String(juce::roundToInt(val)) + suffix;
    else
        valueStr = juce::String(val, decimals) + suffix;

    g.setColour(juce::Colour(0xFFE8E0D4));
    g.setFont(juce::Font(juce::FontOptions((float)valFont)));
    g.drawText(valueStr, textArea.removeFromTop(15),
               juce::Justification::centred, false);

    // LABEL TEXT — always show below value
    if (!labelText.isEmpty())
    {
        g.setColour(juce::Colour(0xFF8A8578));
        g.setFont(juce::Font(juce::FontOptions((float)labelFont)));
        g.drawText(labelText, textArea,
                   juce::Justification::centred, false);
    }
}

void SkuntKnob::resized()
{
    juce::Slider::resized();
}
