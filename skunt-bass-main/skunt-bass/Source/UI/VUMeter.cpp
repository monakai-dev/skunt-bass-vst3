#include "VUMeter.h"
#include <cmath>

VUMeter::VUMeter()
{
    startTimerHz(30);
}

void VUMeter::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    float segmentHeight = (bounds.getHeight() - (numSegments - 1) * 2.0f) / numSegments;
    float segH = segmentHeight;

    // Background
    g.setColour(juce::Colour(0xFF1A1A1A));
    g.fillRoundedRectangle(bounds, 3.0f);

    float dbLevel = 20.0f * log10f(currentLevel + 1e-10f);
    float dbMax = 6.0f;
    float dbMin = -60.0f;
    float normLevel = (dbLevel - dbMin) / (dbMax - dbMin);
    normLevel = juce::jlimit(0.0f, 1.0f, normLevel);

    int activeSegments = static_cast<int>(normLevel * numSegments);

    for (int i = 0; i < numSegments; ++i)
    {
        float y = bounds.getBottom() - (i + 1) * (segH + 2.0f);
        auto segBounds = juce::Rectangle<float>(bounds.getX() + 2.0f, y, bounds.getWidth() - 4.0f, segH);

        if (i < activeSegments)
        {
            g.setColour(getSegmentColour(i, numSegments));
            g.fillRoundedRectangle(segBounds, 1.0f);
        }
        else
        {
            g.setColour(juce::Colour(0xFF2A2A2A));
            g.fillRoundedRectangle(segBounds, 1.0f);
        }
    }
}

void VUMeter::timerCallback()
{
    currentLevel *= 0.92f;
    if (currentLevel < 0.0001f) currentLevel = 0.0f;
    repaint();
}

void VUMeter::setLevel(float levelLinear)
{
    currentLevel = std::max(currentLevel, levelLinear);
}

juce::Colour VUMeter::getSegmentColour(int segment, int total) const
{
    float ratio = static_cast<float>(segment) / total;
    if (ratio < 0.5f)
        return juce::Colour(0xFF2D5016);
    else if (ratio < 0.75f)
        return juce::Colour(0xFF8B7355);
    else
        return juce::Colour(0xFF8B0000);
}
