#include "StepSlider.h"
#include "GothicCamoLookAndFeel.h"

StepSlider::StepSlider(int stepIndex) : stepNumber(stepIndex)
{
    setSliderStyle(juce::Slider::LinearVertical);
    setRange(0.0, 1.0, 0.01);
    setValue(0.5 + (stepIndex % 2 == 0 ? 0.2 : -0.2));
    setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
}

void StepSlider::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    float cornerSize = 3.0f;

    // Background
    g.setColour(juce::Colour(0xFF1A1A1A));
    g.fillRoundedRectangle(bounds, cornerSize);

    // Border
    g.setColour(isActiveStep ? juce::Colour(0xFF8B7355) : juce::Colour(0xFF2D5016));
    g.drawRoundedRectangle(bounds, cornerSize, isActiveStep ? 2.0f : 1.0f);

    // Fill
    float val = (float)getValue();
    float fillHeight = bounds.getHeight() * val;
    auto fillBounds = bounds.removeFromBottom(fillHeight);

    juce::ColourGradient grad(juce::Colour(0xFF8B0000), fillBounds.getX(), fillBounds.getBottom(),
                               juce::Colour(0xFFCC5500), fillBounds.getX(), fillBounds.getY(), false);
    g.setGradientFill(grad);
    g.fillRoundedRectangle(fillBounds, cornerSize);

    // Step number
    g.setColour(isActiveStep ? juce::Colour(0xFF8B7355) : juce::Colour(0xFF8A8578));
    g.setFont(juce::Font(juce::FontOptions(9.0f).withStyle("Bold")));
    g.drawFittedText(juce::String(stepNumber + 1), bounds.removeFromTop(14).toNearestInt(),
                      juce::Justification::centred, 1);
}

void StepSlider::resized()
{
    juce::Slider::resized();
}
