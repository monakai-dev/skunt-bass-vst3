#include "SkuntFilter.h"
#include <cmath>

void SkuntFilter::setCutoff(float freqHz)
{
    cutoff = juce::jlimit(20.0f, 20000.0f, freqHz);
    updateCoefficients();
}

void SkuntFilter::setResonance(float q)
{
    resonance = juce::jlimit(0.0f, 1.0f, q);
    updateCoefficients();
}

void SkuntFilter::reset()
{
    s1[0] = s1[1] = 0.0f;
    s2[0] = s2[1] = 0.0f;
}

void SkuntFilter::updateCoefficients()
{
    float g_tan = std::tan(juce::MathConstants<float>::pi * cutoff / static_cast<float>(sampleRate));
    g = g_tan;
    // Q range: 0.5 to 5.0
    float Q = 0.5f + resonance * 4.5f;
    R = 1.0f / (2.0f * Q);
    h = 1.0f / (1.0f + R * g + g * g);
}

float SkuntFilter::saturate(float x) const
{
    // Soft saturation using tanh
    float driveAmt = 1.0f + drive * 4.0f;
    return std::tanh(x * driveAmt) / (1.0f + drive * 0.5f);
}

float SkuntFilter::process(float input)
{
    // Apply drive before filtering
    float driven = input;
    if (drive > 0.001f)
        driven = saturate(input);

    // Process first SVF stage
    float HP1 = (driven - (R + g) * s1[0]) * h;
    float BP1 = g * HP1 + s1[0];
    float LP1 = g * BP1 + s1[1];
    s1[0] = 2.0f * BP1 - s1[0];
    s1[1] = 2.0f * LP1 - s1[1];

    float output = 0.0f;

    switch (type)
    {
        case FilterType::LP12:
            output = LP1;
            break;

        case FilterType::LP24:
        {
            // Cascade through second stage
            float HP2 = (LP1 - (R + g) * s2[0]) * h;
            float BP2 = g * HP2 + s2[0];
            float LP2 = g * BP2 + s2[1];
            s2[0] = 2.0f * BP2 - s2[0];
            s2[1] = 2.0f * LP2 - s2[1];
            output = LP2;
            break;
        }

        case FilterType::BP6:
            output = BP1;
            break;

        case FilterType::HP12:
            output = HP1;
            break;
    }

    return output;
}
