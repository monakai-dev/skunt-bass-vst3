#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

// Gothic + Camo color constants
namespace GothicCamoColours
{
    inline const juce::Colour bgPrimary      (0xFF0D0D0D);
    inline const juce::Colour bgSecondary    (0xFF1A1A1A);
    inline const juce::Colour bgTertiary     (0xFF141414);
    inline const juce::Colour accentGreen    (0xFF2D5016);
    inline const juce::Colour accentGreenLight(0xFF4A7C2E);
    inline const juce::Colour accentBlood    (0xFF8B0000);
    inline const juce::Colour accentGold     (0xFF8B7355);
    inline const juce::Colour textPrimary    (0xFFE8E0D4);
    inline const juce::Colour textSecondary  (0xFF8A8578);
    inline const juce::Colour knobTrack      (0xFF2D5016);
    inline const juce::Colour knobFill       (0xFF8B0000);
    inline const juce::Colour panelBorder    (0xFF2D5016);
    inline const juce::Colour camo1          (0xFF1A1A1A);
    inline const juce::Colour camo2          (0xFF1F2F15);
    inline const juce::Colour camo3          (0xFF2D5016);
}

class GothicCamoLookAndFeel : public juce::LookAndFeel_V4
{
public:
    GothicCamoLookAndFeel();

    // Draw camo background pattern
    void drawCamoBackground(juce::Graphics& g, juce::Rectangle<int> bounds);

    // Override slider drawing (rotary)
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPosProportional, float rotaryStartAngle,
                          float rotaryEndAngle, juce::Slider&) override;

    // Override button drawing
    void drawButtonBackground(juce::Graphics& g, juce::Button& button,
                               const juce::Colour& backgroundColour,
                               bool shouldDrawButtonAsHighlighted,
                               bool shouldDrawButtonAsDown) override;

    void drawButtonText(juce::Graphics& g, juce::TextButton& button,
                         bool shouldDrawButtonAsHighlighted,
                         bool shouldDrawButtonAsDown) override;

    // Override combo box
    void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       juce::ComboBox& box) override;

    void drawPopupMenuBackground(juce::Graphics& g, int width, int height) override;
    void drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area,
                            bool isSeparator, bool isActive, bool isHighlighted, bool isTicked,
                            bool hasSubMenu, const juce::String& text,
                            const juce::String& shortcutKeyText,
                            const juce::Drawable* icon,
                            const juce::Colour* textColour) override;

    // Override label
    void drawLabel(juce::Graphics& g, juce::Label& label) override;

    // Font helpers
    juce::Font getLabelFont(juce::Label& label) override;

    // Custom drawing helpers
    static juce::Colour getBloodRed() { return GothicCamoColours::accentBlood; }
    static juce::Colour getCamoGreen() { return GothicCamoColours::accentGreen; }

private:
    juce::Font titleFont;
    juce::Font labelFont;
    juce::Font smallFont;

    void generateCamoPaths(juce::Rectangle<int> bounds);
    juce::Path camoPath1, camoPath2, camoPath3;
    bool camoGenerated = false;
};
