#include "ScopeComponent.h"
#include <cmath>

ScopeComponent::ScopeComponent()
{
    setSize(150, 60);
    startTimerHz(20);
    for (int i = 0; i < bufferSize; ++i)
        buffer[i] = 0.0f;
}

void ScopeComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Background - semi-transparent
    g.setColour(juce::Colour(0x88141414));
    g.fillRoundedRectangle(bounds, 3.0f);

    g.setColour(juce::Colour(0xFF2D5016));
    g.drawRoundedRectangle(bounds, 3.0f, 1.0f);

    // Center line
    float cy = bounds.getCentreY();
    g.setColour(juce::Colour(0xFF8A8578).withAlpha(0.3f));
    g.drawHorizontalLine(static_cast<int>(cy), bounds.getX(), bounds.getRight());

    // Waveform
    juce::Path p;
    float xStep = bounds.getWidth() / bufferSize;
    bool started = false;

    for (int i = 0; i < bufferSize; ++i)
    {
        float x = bounds.getX() + i * xStep;
        float y = cy - buffer[i] * bounds.getHeight() * 0.4f;
        if (!started)
        {
            p.startNewSubPath(x, y);
            started = true;
        }
        else
        {
            p.lineTo(x, y);
        }
    }

    g.setColour(waveColour);
    g.strokePath(p, juce::PathStrokeType(1.5f));

    // Glow
    g.setColour(waveColour.withAlpha(0.3f));
    g.strokePath(p, juce::PathStrokeType(3.5f));
}

void ScopeComponent::resized()
{
}

void ScopeComponent::timerCallback()
{
    repaint();
}

void ScopeComponent::pushSample(float sample)
{
    buffer[writePos] = sample;
    writePos = (writePos + 1) & (bufferSize - 1);
}
