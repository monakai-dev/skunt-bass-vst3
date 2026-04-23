#include "Limiter.h"
#include <cmath>

void Limiter::setSampleRate(double sr)
{
    sampleRate = sr;
    lookaheadSamples = static_cast<int>(sr * 0.01); // 10ms
    if (lookaheadSamples > maxLookaheadSamples)
        lookaheadSamples = maxLookaheadSamples;
    releaseCoeff = std::exp(-1.0f / (sr * 0.05)); // 50ms release
}

void Limiter::reset()
{
    for (int i = 0; i < maxLookaheadSamples; ++i)
        delayLine[i] = 0.0f;
    writeIdx = 0;
    envelope = 0.0f;
}

float Limiter::process(float input)
{
    // Write to delay line
    delayLine[writeIdx] = input;
    int readIdx = writeIdx - lookaheadSamples;
    if (readIdx < 0) readIdx += maxLookaheadSamples;
    float delayed = delayLine[readIdx];
    writeIdx++;
    if (writeIdx >= maxLookaheadSamples) writeIdx = 0;

    // Envelope follower on input (lookahead)
    float absInput = std::abs(input);
    envelope = std::max(absInput, releaseCoeff * envelope);

    // Gain reduction (soft limiter)
    float gr = 1.0f;
    if (envelope > 1.0f)
        gr = 1.0f / envelope;

    return delayed * gr;
}
