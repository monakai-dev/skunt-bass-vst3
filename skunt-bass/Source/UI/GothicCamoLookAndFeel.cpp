#include "GothicCamoLookAndFeel.h"

GothicCamoLookAndFeel::GothicCamoLookAndFeel()
{
    // Set default colours
    setColour(juce::ResizableWindow::backgroundColourId, GothicCamoColours::bgPrimary);
    setColour(juce::Slider::rotarySliderFillColourId, GothicCamoColours::knobFill);
    setColour(juce::Slider::rotarySliderOutlineColourId, GothicCamoColours::knobTrack);
    setColour(juce::Slider::thumbColourId, GothicCamoColours::accentGold);
    setColour(juce::Slider::trackColourId, GothicCamoColours::knobTrack);
    setColour(juce::ComboBox::backgroundColourId, GothicCamoColours::bgTertiary);
    setColour(juce::ComboBox::outlineColourId, GothicCamoColours::panelBorder);
    setColour(juce::ComboBox::textColourId, GothicCamoColours::textPrimary);
    setColour(juce::ComboBox::arrowColourId, GothicCamoColours::textSecondary);
    setColour(juce::PopupMenu::backgroundColourId, GothicCamoColours::bgSecondary);
    setColour(juce::PopupMenu::textColourId, GothicCamoColours::textPrimary);
    setColour(juce::PopupMenu::highlightedBackgroundColourId, GothicCamoColours::accentGreen.withAlpha(0.4f));
    setColour(juce::PopupMenu::highlightedTextColourId, GothicCamoColours::textPrimary);
    setColour(juce::Label::textColourId, GothicCamoColours::textPrimary);
    setColour(juce::Label::textWhenEditingColourId, GothicCamoColours::textPrimary);
    setColour(juce::TextButton::buttonColourId, GothicCamoColours::bgSecondary);
    setColour(juce::TextButton::buttonOnColourId, GothicCamoColours::accentBlood);
    setColour(juce::TextButton::textColourOffId, GothicCamoColours::textPrimary);
    setColour(juce::TextButton::textColourOnId, GothicCamoColours::textPrimary);
    setColour(juce::GroupComponent::outlineColourId, GothicCamoColours::panelBorder);
    setColour(juce::GroupComponent::textColourId, GothicCamoColours::textPrimary);
}

void GothicCamoLookAndFeel::drawCamoBackground(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    if (!camoGenerated)
        generateCamoPaths(bounds);

    g.setColour(GothicCamoColours::camo1.withAlpha(0.12f));
    g.fillPath(camoPath1);
    g.setColour(GothicCamoColours::camo2.withAlpha(0.10f));
    g.fillPath(camoPath2);
    g.setColour(GothicCamoColours::camo3.withAlpha(0.08f));
    g.fillPath(camoPath3);
}

void GothicCamoLookAndFeel::generateCamoPaths(juce::Rectangle<int> bounds)
{
    camoPath1.clear();
    camoPath2.clear();
    camoPath3.clear();

    int w = bounds.getWidth();
    int h = bounds.getHeight();

    // Generate deterministic camo blobs
    auto addBlob = [](juce::Path& p, float x, float y, float rx, float ry, float rot)
    {
        p.addEllipse(x - rx, y - ry, rx * 2.0f, ry * 2.0f);
        juce::AffineTransform t = juce::AffineTransform::rotation(rot, x, y);
        p.applyTransform(t);
    };

    // Path 1 - large blobs
    addBlob(camoPath1, w * 0.2f, h * 0.15f, w * 0.15f, h * 0.08f, 0.3f);
    addBlob(camoPath1, w * 0.7f, h * 0.25f, w * 0.12f, h * 0.10f, -0.4f);
    addBlob(camoPath1, w * 0.5f, h * 0.6f, w * 0.18f, h * 0.07f, 0.6f);
    addBlob(camoPath1, w * 0.85f, h * 0.75f, w * 0.10f, h * 0.12f, -0.2f);

    // Path 2 - medium blobs
    addBlob(camoPath2, w * 0.35f, h * 0.35f, w * 0.10f, h * 0.06f, 0.5f);
    addBlob(camoPath2, w * 0.6f, h * 0.1f, w * 0.08f, h * 0.08f, -0.3f);
    addBlob(camoPath2, w * 0.15f, h * 0.7f, w * 0.12f, h * 0.05f, 0.8f);
    addBlob(camoPath2, w * 0.8f, h * 0.5f, w * 0.09f, h * 0.07f, -0.6f);

    // Path 3 - small accent blobs
    addBlob(camoPath3, w * 0.45f, h * 0.2f, w * 0.06f, h * 0.04f, 0.2f);
    addBlob(camoPath3, w * 0.25f, h * 0.55f, w * 0.07f, h * 0.05f, -0.5f);
    addBlob(camoPath3, w * 0.75f, h * 0.85f, w * 0.05f, h * 0.06f, 0.4f);

    camoGenerated = true;
}

void GothicCamoLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                               float sliderPosProportional, float rotaryStartAngle,
                                               float rotaryEndAngle, juce::Slider&)
{
    float cx = x + width * 0.5f;
    float cy = y + height * 0.5f;
    float radius = juce::jmin(width, height) * 0.5f - 4.0f;

    // Background circle
    g.setColour(GothicCamoColours::bgSecondary);
    g.fillEllipse(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f);

    // Outer ring
    g.setColour(GothicCamoColours::panelBorder);
    g.drawEllipse(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f, 2.0f);

    // Track arc
    juce::Path trackPath;
    trackPath.addCentredArc(cx, cy, radius - 6.0f, radius - 6.0f,
                            0.0f, rotaryStartAngle, rotaryEndAngle, true);
    g.setColour(GothicCamoColours::knobTrack);
    g.strokePath(trackPath, juce::PathStrokeType(5.0f));

    // Fill arc
    float angle = rotaryStartAngle + (rotaryEndAngle - rotaryStartAngle) * sliderPosProportional;
    juce::Path fillPath;
    fillPath.addCentredArc(cx, cy, radius - 6.0f, radius - 6.0f,
                           0.0f, rotaryStartAngle, angle, true);
    g.setColour(GothicCamoColours::knobFill);
    g.strokePath(fillPath, juce::PathStrokeType(5.0f));

    // Indicator dot
    float dotRadius = 4.5f;
    float dotX = cx + std::cos(angle - juce::MathConstants<float>::halfPi) * (radius - 6.0f);
    float dotY = cy + std::sin(angle - juce::MathConstants<float>::halfPi) * (radius - 6.0f);
    g.setColour(GothicCamoColours::accentGold);
    g.fillEllipse(dotX - dotRadius, dotY - dotRadius, dotRadius * 2.0f, dotRadius * 2.0f);

    // Inner circle (subtle)
    float innerR = radius * 0.4f;
    g.setColour(GothicCamoColours::bgTertiary);
    g.fillEllipse(cx - innerR, cy - innerR, innerR * 2.0f, innerR * 2.0f);
}

void GothicCamoLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                                   const juce::Colour&, bool, bool isDown)
{
    juce::Rectangle<int> bounds = button.getLocalBounds();
    float cornerSize = 4.0f;

    juce::Colour fillColour;
    if (isDown)
        fillColour = GothicCamoColours::accentGreen;
    else if (button.getToggleState())
        fillColour = GothicCamoColours::accentBlood;
    else
        fillColour = GothicCamoColours::bgSecondary;

    g.setColour(fillColour);
    g.fillRoundedRectangle(bounds.toFloat(), cornerSize);

    g.setColour(GothicCamoColours::panelBorder);
    g.drawRoundedRectangle(bounds.toFloat(), cornerSize, 1.5f);
}

void GothicCamoLookAndFeel::drawButtonText(juce::Graphics& g, juce::TextButton& button,
                                            bool, bool)
{
    g.setColour(GothicCamoColours::textPrimary);
    g.setFont(juce::Font(juce::FontOptions(13.0f).withStyle("Bold")));
    g.drawFittedText(button.getButtonText(), button.getLocalBounds(),
                      juce::Justification::centred, 1);
}

void GothicCamoLookAndFeel::drawComboBox(juce::Graphics& g, int width, int, bool,
                                          int, int, int, int, juce::ComboBox& box)
{
    juce::Rectangle<int> bounds(0, 0, width, box.getHeight());

    g.setColour(GothicCamoColours::bgTertiary);
    g.fillRoundedRectangle(bounds.toFloat(), 4.0f);
    g.setColour(GothicCamoColours::panelBorder);
    g.drawRoundedRectangle(bounds.toFloat(), 4.0f, 1.0f);

    // Draw selected text
    g.setColour(GothicCamoColours::textPrimary);
    g.setFont(juce::Font(juce::FontOptions(12.0f)));
    g.drawFittedText(box.getText(), bounds.reduced(8, 0).removeFromRight(bounds.getWidth() - 20),
                      juce::Justification::centredLeft, 1);

    // Draw chevron
    float cx = width - 14.0f;
    float cy = box.getHeight() * 0.5f;
    juce::Path chevron;
    chevron.addTriangle(cx - 4.0f, cy - 2.0f, cx + 4.0f, cy - 2.0f, cx, cy + 3.0f);
    g.setColour(GothicCamoColours::textSecondary);
    g.fillPath(chevron);
}

void GothicCamoLookAndFeel::drawPopupMenuBackground(juce::Graphics& g, int width, int height)
{
    g.setColour(GothicCamoColours::bgSecondary);
    g.fillRect(0, 0, width, height);
}

void GothicCamoLookAndFeel::drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area,
                                               bool isSeparator, bool /*isActive*/, bool isHighlighted,
                                               bool /*isTicked*/, bool /*hasSubMenu*/,
                                               const juce::String& text, const juce::String&,
                                               const juce::Drawable*, const juce::Colour*)
{
    if (isSeparator)
    {
        g.setColour(GothicCamoColours::panelBorder);
        g.fillRect(area.reduced(8, 0).withHeight(1).withY(area.getCentreY()));
        return;
    }

    if (isHighlighted)
    {
        g.setColour(GothicCamoColours::accentGreen.withAlpha(0.4f));
        g.fillRect(area);
    }

    g.setColour(GothicCamoColours::textPrimary);
    g.setFont(juce::Font(juce::FontOptions(13.0f)));
    g.drawFittedText(text, area.reduced(12, 0), juce::Justification::centredLeft, 1);
}

void GothicCamoLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    g.setColour(label.findColour(juce::Label::textColourId));
    g.setFont(getLabelFont(label));
    g.drawFittedText(label.getText(), label.getLocalBounds(),
                      juce::Justification::centred, 1);
}

juce::Font GothicCamoLookAndFeel::getLabelFont(juce::Label&)
{
    return juce::Font(12.0f, juce::Font::plain);
}
