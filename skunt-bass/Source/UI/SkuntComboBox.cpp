#include "SkuntComboBox.h"
#include "GothicCamoLookAndFeel.h"

SkuntComboBox::SkuntComboBox()
{
    setSize(100, 24);
}

void SkuntComboBox::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();

    g.setColour(juce::Colour(0xFF141414));
    g.fillRoundedRectangle(bounds.toFloat(), 4.0f);

    g.setColour(juce::Colour(0xFF2D5016));
    g.drawRoundedRectangle(bounds.toFloat(), 4.0f, 1.0f);

    g.setColour(juce::Colour(0xFFE8E0D4));
    g.setFont(juce::Font(juce::FontOptions(12.0f)));
    g.drawFittedText(getText(), bounds.reduced(8, 0).removeFromRight(bounds.getWidth() - 24),
                      juce::Justification::centredLeft, 1);

    // Chevron
    float cx = bounds.getRight() - 14.0f;
    float cy = bounds.getCentreY();
    juce::Path chevron;
    chevron.addTriangle(cx - 4.0f, cy - 2.0f, cx + 4.0f, cy - 2.0f, cx, cy + 3.0f);
    g.setColour(juce::Colour(0xFF8A8578));
    g.fillPath(chevron);
}

void SkuntComboBox::resized()
{
    juce::ComboBox::resized();
}
