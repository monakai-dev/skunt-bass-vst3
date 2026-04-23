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
    // Randomize phases to prevent DC click on note start
    // All oscillators start at different points for cleaner attacks
    phaseA = juce::Random::getSystemRandom().nextFloat();
    phaseB = juce::Random::getSystemRandom().nextFloat();
    phaseSub = juce::Random::getSystemRandom().nextFloat();
    lastA = 0.0f;
    lastB = 0.0f;
    noiseState = 0.0f;
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

float Oscillator::getSubSample(float ph, float inc)
{
    switch (subWave)
    {
        case SubWave::Sine:
            return std::sin(ph * juce::MathConstants<float>::twoPi);
        case SubWave::Square:
        {
            float s = ph < 0.5f ? 1.0f : -1.0f;
            // PolyBLEP anti-aliasing for square
            s += polyBLEP(ph, inc);
            s -= polyBLEP(std::fmod(ph + 0.5f, 1.0f), inc);
            return s;
        }
        case SubWave::Saw:
        {
            float s = ph * 2.0f - 1.0f;
            s -= polyBLEP(ph, inc);
            return s;
        }
        case SubWave::Triangle:
        {
            // Direct triangle — at sub frequencies aliasing is minimal
            // (triangle harmonics roll off as 1/n^2 vs 1/n for square/saw)
            float t = ph < 0.5f ? (ph * 4.0f - 1.0f) : ((1.0f - ph) * 4.0f - 1.0f);
            // Subtle polyBLEP on the discontinuity for extra smoothness
            if (ph < inc) t -= polyBLEP(ph, inc) * 0.5f;
            if (std::abs(ph - 0.5f) < inc) t += polyBLEP(std::fmod(ph + 0.5f, 1.0f), inc) * 0.5f;
            return t;
        }
    }
    return 0.0f;
}

OscOutput Oscillator::process()
{
    // Calculate frequencies from pitch
    float pitchAOffset = pitchASemitones + pitchAFine * 0.01f;
    float pitchBOffset = pitchBSemitones + pitchBFine * 0.01f;

    float baseFreqA = noteFrequency * std::pow(2.0f, pitchAOffset / 12.0f);
    float baseFreqB = noteFrequency * std::pow(2.0f, pitchBOffset / 12.0f);

    // Apply FM: Osc A modulates Osc B frequency
    float fmMod = 1.0f + externalFM * fmAmount * 5.0f;
    float freqB = baseFreqB * fmMod;

    // Sub is -1 octave (standard bass sub)
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
            float white = noiseGen.nextFloat() * 2.0f - 1.0f;
            // Pink-ish noise: gentle lowpass on white noise
            float noiseCoeff = 1.0f - std::exp(-2.0f * juce::MathConstants<float>::pi * 6000.0f / static_cast<float>(sampleRate));
            noiseState += noiseCoeff * (white - noiseState);
            sampleB = noiseState * 1.5f;  // Boost for better audibility
            break;
        }
    }

    // --- Generate Sub oscillator ---
    phaseSub += incSub;
    while (phaseSub >= 1.0f) phaseSub -= 1.0f;
    float sampleSub = getSubSample(phaseSub, incSub);

    // Store outputs for FM feedback
    lastA = sampleA;
    lastB = sampleB;

    // SERUM-STYLE ARCHITECTURE:
    // filtered path: oscA + oscB + noise → goes through filter
    // direct path:  sub → bypasses filter (like Serum "Direct Out")
    OscOutput out;
    out.filtered = sampleA * levelA
                 + sampleB * levelB;

    // Add noise to filtered path
    if (noiseLevel > 0.001f)
    {
        float white = noiseGen.nextFloat() * 2.0f - 1.0f;
        out.filtered += white * noiseLevel * 0.3f;
    }

    // Sub goes direct — bypasses filter completely
    out.direct = sampleSub * subLevel;

    return out;
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
