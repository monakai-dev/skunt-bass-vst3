#include "AdsrDisplay.h"

AdsrDisplay::AdsrDisplay()
{
    setSize(120, 50);
}

void AdsrDisplay::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Background
    g.setColour(juce::Colour(0xFF141414));
    g.fillRoundedRectangle(bounds, 3.0f);

    // Border
    g.setColour(juce::Colour(0xFF2D5016));
    g.drawRoundedRectangle(bounds, 3.0f, 1.0f);

    drawEnvelopePath(g);
}

void AdsrDisplay::drawEnvelopePath(juce::Graphics& g)
{
    auto bounds = getLocalBounds().reduced(4).toFloat();
    float w = bounds.getWidth();
    float h = bounds.getHeight();
    float bottom = bounds.getBottom();

    // Normalize times (sum = 1 for proportional display)
    float totalTime = attackMs + decayMs + 300.0f + releaseMs;
    if (totalTime < 1.0f) totalTime = 1000.0f;

    float xA = bounds.getX() + (attackMs / totalTime) * w;
    float xD = xA + (decayMs / totalTime) * w;
    float xS = xD + 0.15f * w;
    float xR = xS + (releaseMs / totalTime) * w;

    float y0 = bottom;
    float yA = bounds.getY();
    float yS = bottom - sustain * h;
    float yR = bottom;

    juce::Path p;
    p.startNewSubPath(bounds.getX(), y0);
    p.lineTo(xA, yA);
    p.lineTo(xD, yS);
    p.lineTo(xS, yS);
    p.lineTo(juce::jmin(xR, bounds.getRight()), yR);

    // Fill
    juce::Path fillPath = p;
    fillPath.lineTo(bounds.getX(), y0);
    fillPath.closeSubPath();
    g.setColour(juce::Colour(0xFF8B0000).withAlpha(0.2f));
    g.fillPath(fillPath);

    // Stroke
    g.setColour(juce::Colour(0xFF8B0000));
    g.strokePath(p, juce::PathStrokeType(2.0f));

    // Gold dots at breakpoints
    g.setColour(juce::Colour(0xFF8B7355));
    float dotR = 2.5f;
    g.fillEllipse(xA - dotR, yA - dotR, dotR * 2.0f, dotR * 2.0f);
    g.fillEllipse(xD - dotR, yS - dotR, dotR * 2.0f, dotR * 2.0f);
    g.fillEllipse(xS - dotR, yS - dotR, dotR * 2.0f, dotR * 2.0f);
}
