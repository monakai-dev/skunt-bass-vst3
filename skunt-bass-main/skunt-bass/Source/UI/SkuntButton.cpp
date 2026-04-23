#include "SkuntButton.h"
#include "GothicCamoLookAndFeel.h"

SkuntButton::SkuntButton()
{
    setClickingTogglesState(false);
}

void SkuntButton::paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    auto bounds = getLocalBounds().toFloat();
    float cornerSize = 4.0f;

    juce::Colour fillColour;
    if (shouldDrawButtonAsDown)
        fillColour = juce::Colour(0xFF2D5016);
    else if (getToggleState())
        fillColour = juce::Colour(0xFF8B0000);
    else
        fillColour = juce::Colour(0xFF1A1A1A);

    g.setColour(fillColour);
    g.fillRoundedRectangle(bounds, cornerSize);

    g.setColour(juce::Colour(0xFF2D5016));
    g.drawRoundedRectangle(bounds, cornerSize, 1.5f);

    // LED indicator
    if (showLED)
    {
        float ledR = 3.5f;
        float ledX = bounds.getRight() - 8.0f;
        float ledY = bounds.getY() + 8.0f;

        if (getToggleState())
        {
            g.setColour(ledColour);
            g.fillEllipse(ledX - ledR, ledY - ledR, ledR * 2.0f, ledR * 2.0f);
            g.setColour(ledColour.withAlpha(0.5f));
            g.fillEllipse(ledX - ledR - 2.0f, ledY - ledR - 2.0f, (ledR + 2.0f) * 2.0f, (ledR + 2.0f) * 2.0f);
        }
        else
        {
            g.setColour(juce::Colour(0xFF333333));
            g.fillEllipse(ledX - ledR, ledY - ledR, ledR * 2.0f, ledR * 2.0f);
        }
    }

    // Text
    g.setColour(juce::Colour(0xFFE8E0D4));
    g.setFont(juce::Font(juce::FontOptions(13.0f).withStyle("Bold")));
    g.drawFittedText(getButtonText(), getLocalBounds(), juce::Justification::centred, 1);
}
