#include "Multiband.h"
#include <cmath>

void MultibandCrossover::setSampleRate(double sr)
{
    sampleRate = sr;
    updateCoefficients();
}

void MultibandCrossover::setCrossoverFrequencies(float lowFreq, float highFreq)
{
    xoverLow = juce::jlimit(20.0f, 500.0f, lowFreq);
    xoverHigh = juce::jlimit(1000.0f, 10000.0f, highFreq);
    updateCoefficients();
}

void MultibandCrossover::reset()
{
    lp1_s = lp2_s = 0.0f;
    hp1_s = hp2_s = 0.0f;
    ap1_s = ap2_s = 0.0f;
}

void MultibandCrossover::updateCoefficients()
{
    // 1-pole coefficients: c = 1 - exp(-2*pi*fc/fs)
    lp1_b0 = 1.0f - std::exp(-2.0f * juce::MathConstants<float>::pi * xoverLow / static_cast<float>(sampleRate));
    lp2_b0 = lp1_b0;
    hp1_b0 = 1.0f - std::exp(-2.0f * juce::MathConstants<float>::pi * xoverHigh / static_cast<float>(sampleRate));
    hp2_b0 = hp1_b0;
    lp1_a1 = 1.0f - lp1_b0;
    lp2_a1 = lp2_b0;
    hp1_a1 = 1.0f - hp1_b0;
    hp2_a1 = hp2_b0;
}

void MultibandCrossover::process(float input, float& lo, float& mid, float& hi)
{
    // Low band: 2 cascaded 1-pole LPFs at xoverLow
    float lp1 = lp1_s + lp1_b0 * (input - lp1_s);
    lp1_s = lp1;
    float lp_out = lp2_s + lp2_b0 * (lp1 - lp2_s);
    lp2_s = lp_out;

    // High band: 2 cascaded 1-pole HPFs at xoverHigh
    float hp1 = input - hp1_s;
    hp1_s = hp1_s + hp1_b0 * hp1;
    float hp_out = hp1 - hp2_s;
    hp2_s = hp2_s + hp2_b0 * hp_out;

    lo = lp_out;
    hi = hp_out;

    // Mid = input - low - high (approximate bandpass)
    mid = input - lo - hi;
    mid = juce::jlimit(-10.0f, 10.0f, mid);
}
