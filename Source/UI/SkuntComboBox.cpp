#include "SkuntComboBox.h"

SkuntComboBox::SkuntComboBox()
{
    setSize(100, 24);
}

void SkuntComboBox::resized()
{
    juce::ComboBox::resized();
}
