#include "StereoWidth.h"
#include <cmath>

void StereoWidth::setSampleRate(double sr)
{
    sampleRate = sr;
    updateCoefficients();
}

void StereoWidth::setWidth(float width01)
{
    width = juce::jlimit(0.0f, 1.0f, width01);
}

void StereoWidth::updateCoefficients()
{
    // 1-pole LP at 120Hz for mono bass protection on side channel
    lpCoeff = 1.0f - std::exp(-2.0f * juce::MathConstants<float>::pi * 120.0f / sampleRate);
}

void StereoWidth::reset()
{
    lpStateL = 0.0f;
    lpStateR = 0.0f;
}

void StereoWidth::process(float& left, float& right)
{
    // M/S encode
    float mid = (left + right) * 0.5f;
    float side = (left - right) * 0.5f;

    // Width control: side *= width * 2
    side *= width * 2.0f;

    // Mono bass protection: lowpass filter the side channel
    lpStateL += lpCoeff * (side - lpStateL);
    side = lpStateL;

    // M/S decode
    left  = mid + side;
    right = mid - side;
}
