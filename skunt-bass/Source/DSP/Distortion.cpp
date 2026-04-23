#include "Distortion.h"
#include <cmath>

float Distortion::process(float input)
{
    float dry = input;
    float wet = 0.0f;
    float driveFactor = 1.0f + amount * 2.0f;
    float driven = input * driveFactor;

    switch (type)
    {
        case DistortionType::Tube:
        {
            float d = 1.0f + amount * 10.0f;
            wet = std::tanh(driven * d) / std::tanh(d);
            break;
        }

        case DistortionType::Foldback:
        {
            float threshold = 1.0f / (1.0f + amount * 3.0f);
            wet = driven;
            int folds = 1 + static_cast<int>(amount * 5.0f);
            for (int i = 0; i < folds; ++i)
            {
                if (wet > threshold)
                    wet = threshold - (wet - threshold);
                else if (wet < -threshold)
                    wet = -threshold + (std::abs(wet) - threshold);
            }
            wet = juce::jlimit(-1.0f, 1.0f, wet);
            break;
        }

        case DistortionType::Bitcrush:
        {
            // Sample rate reduction
            bitPhase += 1.0f;
            int holdSamples = 1 + static_cast<int>(amount * 50.0f);
            if (bitPhase >= holdSamples)
            {
                bitPhase = 0.0f;
                lastOutput = driven;
            }
            wet = lastOutput;

            // Bit depth reduction
            float bits = 1.0f + (1.0f - amount) * 7.0f;
            float levels = std::pow(2.0f, bits - 1.0f);
            wet = std::round(wet * levels) / levels;
            break;
        }
    }

    // Gain compensation
    wet /= driveFactor;

    // Dry/wet mix
    return dry * (1.0f - mix) + wet * mix;
}
