#pragma once
#include <juce_core/juce_core.h>
#include <juce_dsp/juce_dsp.h>

// Waveform types for Osc A
enum class WaveA { Wavetable = 0, Saw, Square, Sine };
// Waveform types for Osc B
enum class WaveB { Saw = 0, Square, Sine, Noise };
// Sub oscillator waveforms
enum class SubWave { Sine = 0, Square, Saw, Triangle };

// Output structure: filtered path + direct sub path
struct OscOutput
{
    float filtered = 0.0f;  // oscA + oscB + noise → goes through filter
    float direct = 0.0f;    // sub → bypasses filter (Serum "Direct Out")
};

class Oscillator
{
public:
    Oscillator() = default;

    void setSampleRate(double sr);

    // Note frequency from MIDI
    void setNoteFrequency(float freq) { noteFrequency = freq; }

    // Osc A controls
    void setWaveA(WaveA wave) { waveA = wave; }
    void setPitchA(int semitones, float fineCents);
    void setLevelA(float level) { levelA = level; }

    // Osc B controls
    void setWaveB(WaveB wave) { waveB = wave; }
    void setPitchB(int semitones, float fineCents);
    void setLevelB(float level) { levelB = level; }

    // FM, Sub, Noise
    void setFMAmount(float amount) { fmAmount = amount; }
    void setSubLevel(float level) { subLevel = level; }
    void setSubWaveform(SubWave w) { subWave = w; }
    void setNoiseLevel(float level) { noiseLevel = level; }

    // FM input (from another oscillator or mod source)
    void setFMInput(float fmValue) { externalFM = fmValue; }

    void reset();
    void prepare(const juce::dsp::ProcessSpec& spec);

    // Process one sample — returns SEPARATE filtered path and direct sub path
    // filtered = oscA + oscB + noise (goes through filter)
    // direct   = sub (bypasses filter, like Serum "Direct Out")
    OscOutput process();

    // Get individual outputs for FM routing
    float getLastOscAOutput() const { return lastA; }
    float getLastOscBOutput() const { return lastB; }

private:
    double sampleRate = 48000.0;

    // Waveform selection
    WaveA waveA = WaveA::Saw;
    WaveB waveB = WaveB::Square;

    // Pitch
    float pitchASemitones = 0.0f;
    float pitchAFine = 0.0f;
    float pitchBSemitones = 0.0f;
    float pitchBFine = 0.0f;

    // Levels
    float levelA = 0.8f;
    float levelB = 0.4f;
    float subLevel = 0.5f;
    SubWave subWave = SubWave::Sine;
    float noiseLevel = 0.05f;
    float fmAmount = 0.3f;
    float externalFM = 0.0f;

    // Phase accumulators
    float phaseA = 0.0f;
    float phaseB = 0.0f;
    float phaseSub = 0.0f;

    // Base note frequency from MIDI
    float noteFrequency = 440.0f;

    // Last outputs for FM routing
    float lastA = 0.0f;
    float lastB = 0.0f;

    juce::Random noiseGen;

    // Noise state (per-instance, NOT static — each voice needs its own)
    float noiseState = 0.0f;

    // Wavetable (64 samples, sine-derived with harmonics)
    static constexpr int wtSize = 64;
    float wavetable[wtSize];
    void initWavetable();

    // Helper methods
    float incrementPhase(float& phase, float freq);
    float polyBLEP(float phase, float phaseInc);
    float getSampleA(float ph);
    float getSampleB(float ph);
    float getSubSample(float ph, float inc);  // Generate sub osc with polyBLEP anti-aliasing
};