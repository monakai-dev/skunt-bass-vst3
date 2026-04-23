#include "SkuntFilter.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void SkuntFilter::setCutoff(float freqHz)
{
    cutoff = juce::jlimit(20.0f, 18000.0f, freqHz);
    updateCoefficients();
}

void SkuntFilter::setResonance(float q)
{
    resonance = juce::jlimit(0.0f, 1.0f, q);
    updateCoefficients();
}

void SkuntFilter::reset()
{
    z1_1 = z2_1 = 0.0f;
    z1_2 = z2_2 = 0.0f;
}

float SkuntFilter::saturate(float x) const
{
    // Soft clipping with tanh
    if (drive > 0.001f)
    {
        float driveAmt = 1.0f + drive * 4.0f;
        return std::tanh(x * driveAmt) / driveAmt;
    }
    return x;
}

void SkuntFilter::updateCoefficients()
{
    // Normalize frequency
    float w0 = static_cast<float>(2.0 * M_PI * cutoff / sampleRate);
    float cosw0 = std::cos(w0);
    float sinw0 = std::sin(w0);

    // Q mapping: 0.5 to 8.5 for pronounced resonance
    float Q = 0.5f + resonance * 8.0f;

    float alpha = sinw0 / (2.0f * Q);

    if (type == FilterType::LP12 || type == FilterType::LP24)
    {
        // Lowpass biquad (RBJ)
        float a0 = 1.0f + alpha;
        b0_1 = (1.0f - cosw0) / (2.0f * a0);
        b1_1 = (1.0f - cosw0) / a0;
        b2_1 = b0_1;
        a1_1 = (-2.0f * cosw0) / a0;
        a2_1 = (1.0f - alpha) / a0;

        // For LP24, stage 2 uses same coeffs but independent state
        if (type == FilterType::LP24)
        {
            b0_2 = b0_1; b1_2 = b1_1; b2_2 = b2_1;
            a1_2 = a1_1; a2_2 = a2_1;
        }
    }
    else if (type == FilterType::BP6)
    {
        // Bandpass (constant 0 dB peak)
        float a0 = 1.0f + alpha;
        b0_1 = alpha / a0;
        b1_1 = 0.0f;
        b2_1 = -alpha / a0;
        a1_1 = (-2.0f * cosw0) / a0;
        a2_1 = (1.0f - alpha) / a0;
    }
    else if (type == FilterType::HP12)
    {
        // Highpass
        float a0 = 1.0f + alpha;
        b0_1 = (1.0f + cosw0) / (2.0f * a0);
        b1_1 = -(1.0f + cosw0) / a0;
        b2_1 = b0_1;
        a1_1 = (-2.0f * cosw0) / a0;
        a2_1 = (1.0f - alpha) / a0;
    }
}

// Tiny DC offset to prevent denormal issues in IIR filter
static constexpr float ANTI_DENORMAL = 1.0e-15f;

float SkuntFilter::processBiquad(float input, float a1, float a2, float b0, float b1, float b2, float& z1, float& z2)
{
    // Transposed Direct Form II with anti-denormal protection
    float output = b0 * input + z1;
    z1 = b1 * input + z2 - a1 * output + ANTI_DENORMAL;
    z2 = b2 * input - a2 * output + ANTI_DENORMAL;
    return output;
}

float SkuntFilter::process(float input)
{
    // Apply drive saturation to input (generates harmonics before filtering)
    float driven = saturate(input);

    float output;
    if (type == FilterType::LP24)
    {
        // Two cascaded biquads with independent state
        float stage1 = processBiquad(driven, a1_1, a2_1, b0_1, b1_1, b2_1, z1_1, z2_1);
        output = processBiquad(stage1, a1_2, a2_2, b0_2, b1_2, b2_2, z1_2, z2_2);
    }
    else
    {
        output = processBiquad(driven, a1_1, a2_1, b0_1, b1_1, b2_1, z1_1, z2_1);
    }

    // Additional saturation on output for character
    return std::tanh(output * 1.2f);
}
