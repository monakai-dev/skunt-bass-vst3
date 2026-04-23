#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

// Dark styled combo box for wave/filter/mode selection
class SkuntComboBox : public juce::ComboBox
{
public:
    SkuntComboBox();
    void resized() override;

    void setLabel(const juce::String& lbl) { labelText = lbl; }

private:
    juce::String labelText;
};
