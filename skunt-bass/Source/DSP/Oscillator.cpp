#include "Oscillator.h"
#include <cmath>

void Oscillator::setSampleRate(double sr)
{
    sampleRate = sr;
    initWavetable();
}

void Oscillator::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    initWavetable();
    reset();
}

void Oscillator::reset()
{
    phaseA = 0.0f;
    phaseB = 0.0f;
    phaseSub = 0.0f;
    lastA = 0.0f;
    lastB = 0.0f;
}

void Oscillator::initWavetable()
{
    // Build a 64-sample wavetable with odd harmonics (rich, reese-like tone)
    for (int i = 0; i < wtSize; ++i)
    {
        float phase = static_cast<float>(i) / static_cast<float>(wtSize);
        float sample = 0.0f;
        // Add harmonics 1, 3, 5, 7 with decreasing amplitude
        sample += std::sin(phase * juce::MathConstants<float>::twoPi);
        sample += 0.5f  * std::sin(phase * juce::MathConstants<float>::twoPi * 3.0f);
        sample += 0.25f * std::sin(phase * juce::MathConstants<float>::twoPi * 5.0f);
        sample += 0.125f* std::sin(phase * juce::MathConstants<float>::twoPi * 7.0f);
        wavetable[i] = sample * 0.6f; // normalize
    }
}

void Oscillator::setPitchA(int semitones, float fineCents)
{
    pitchASemitones = static_cast<float>(semitones);
    pitchAFine = fineCents;
}

void Oscillator::setPitchB(int semitones, float fineCents)
{
    pitchBSemitones = static_cast<float>(semitones);
    pitchBFine = fineCents;
}

float Oscillator::process()
{
    // Calculate frequencies from pitch
    float pitchAOffset = pitchASemitones + pitchAFine * 0.01f;
    float pitchBOffset = pitchBSemitones + pitchBFine * 0.01f;

    float baseFreqA = 440.0f * std::pow(2.0f, (pitchAOffset) / 12.0f);
    float baseFreqB = 440.0f * std::pow(2.0f, (pitchBOffset) / 12.0f);

    // Apply FM: Osc A modulates Osc B frequency
    float fmMod = 1.0f + externalFM * fmAmount * 5.0f;
    float freqB = baseFreqB * fmMod;

    // Sub is -1 octave
    float freqSub = baseFreqA * 0.5f;

    // Clamp frequencies
    baseFreqA = juce::jlimit(10.0f, 20000.0f, baseFreqA);
    freqB = juce::jlimit(10.0f, 20000.0f, freqB);
    freqSub = juce::jlimit(10.0f, 10000.0f, freqSub);

    float incA = baseFreqA / static_cast<float>(sampleRate);
    float incB = freqB / static_cast<float>(sampleRate);
    float incSub = freqSub / static_cast<float>(sampleRate);

    // --- Generate Osc A ---
    float sampleA = 0.0f;
    phaseA += incA;
    while (phaseA >= 1.0f) phaseA -= 1.0f;

    switch (waveA)
    {
        case WaveA::Wavetable:
        {
            float pos = phaseA * static_cast<float>(wtSize);
            int idx0 = static_cast<int>(pos) & (wtSize - 1);
            int idx1 = (idx0 + 1) & (wtSize - 1);
            float frac = pos - static_cast<float>(idx0);
            sampleA = wavetable[idx0] + frac * (wavetable[idx1] - wavetable[idx0]);
            break;
        }
        case WaveA::Saw:
        {
            sampleA = phaseA * 2.0f - 1.0f;
            sampleA -= polyBLEP(phaseA, incA);
            break;
        }
        case WaveA::Square:
        {
            sampleA = phaseA < 0.5f ? 1.0f : -1.0f;
            sampleA += polyBLEP(phaseA, incA);
            sampleA -= polyBLEP(std::fmod(phaseA + 0.5f, 1.0f), incA);
            break;
        }
        case WaveA::Sine:
            sampleA = std::sin(phaseA * juce::MathConstants<float>::twoPi);
            break;
    }

    // --- Generate Osc B ---
    float sampleB = 0.0f;
    phaseB += incB;
    while (phaseB >= 1.0f) phaseB -= 1.0f;

    switch (waveB)
    {
        case WaveB::Saw:
            sampleB = phaseB * 2.0f - 1.0f;
            sampleB -= polyBLEP(phaseB, incB);
            break;
        case WaveB::Square:
            sampleB = phaseB < 0.5f ? 1.0f : -1.0f;
            sampleB += polyBLEP(phaseB, incB);
            sampleB -= polyBLEP(std::fmod(phaseB + 0.5f, 1.0f), incB);
            break;
        case WaveB::Sine:
            sampleB = std::sin(phaseB * juce::MathConstants<float>::twoPi);
            break;
        case WaveB::Noise:
        {
            static float noiseState = 0.0f;
            float white = noiseGen.nextFloat() * 2.0f - 1.0f;
            float noiseCoeff = 1.0f - std::exp(-2.0f * juce::MathConstants<float>::pi * 8000.0f / static_cast<float>(sampleRate));
            noiseState += noiseCoeff * (white - noiseState);
            sampleB = noiseState;
            break;
        }
    }

    // --- Generate Sub oscillator ---
    phaseSub += incSub;
    while (phaseSub >= 1.0f) phaseSub -= 1.0f;
    float sampleSub = phaseSub < 0.5f ? 1.0f : -1.0f;

    // Store outputs for FM feedback
    lastA = sampleA;
    lastB = sampleB;

    // Mix
    float output = sampleA * levelA
                 + sampleB * levelB
                 + sampleSub * subLevel;

    // Add noise (separate from osc B noise)
    if (noiseLevel > 0.001f)
    {
        float white = noiseGen.nextFloat() * 2.0f - 1.0f;
        output += white * noiseLevel * 0.3f;
    }

    return output;
}

float Oscillator::polyBLEP(float phase, float phaseInc)
{
    float result = 0.0f;

    if (phase < phaseInc)
    {
        float t = phase / phaseInc;
        result = t + t - t * t - 1.0f;
    }
    else if (phase > 1.0f - phaseInc)
    {
        float t = (phase - 1.0f) / phaseInc;
        result = t * t + t + t + 1.0f;
    }

    return result * 0.5f;
}
