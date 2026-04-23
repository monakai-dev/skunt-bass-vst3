#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>
#include "Oscillator.h"
#include "SkuntFilter.h"
#include "Envelope.h"

// Modulation destinations
enum class ModDestination { FilterCutoff = 0, Pitch, FMAmount, Resonance };

// Per-voice modulation structure
struct VoiceMods
{
    float lfo1 = 0.0f;
    float lfo2 = 0.0f;
    float seq = 0.0f;
    float modEnv = 0.0f;
};

class SkuntVoice : public juce::SynthesiserVoice
{
public:
    SkuntVoice();

    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity,
                   juce::SynthesiserSound*, int pitchWheel) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void pitchWheelMoved(int newValue) override;
    void controllerMoved(int controllerNumber, int newValue) override;
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                         int startSample, int numSamples) override;

    void setSampleRate(double sr);
    void setOscAWave(WaveA w) { osc.setWaveA(w); }
    void setOscAPitch(int semis, float fine) { osc.setPitchA(semis, fine); pitchASemitones = semis; pitchAFine = fine; }
    void setOscALevel(float lvl) { osc.setLevelA(lvl); }
    void setOscBWave(WaveB w) { osc.setWaveB(w); }
    void setOscBPitch(int semis, float fine) { osc.setPitchB(semis, fine); pitchBSemitones = semis; pitchBFine = fine; }
    void setOscBLevel(float lvl) { osc.setLevelB(lvl); }
    void setFMAmount(float amt) { osc.setFMAmount(amt); fmAmount = amt; }
    void setSubLevel(float lvl) { osc.setSubLevel(lvl); }
    void setNoiseLevel(float lvl) { osc.setNoiseLevel(lvl); }
    void setFilterType(FilterType t) { filter.setType(t); }
    void setFilterCutoff(float freq) { baseCutoff = freq; filter.setCutoff(freq); }
    void setFilterResonance(float q) { filter.setResonance(q); filterResonance = q; }
    void setFilterDrive(float d) { filter.setDrive(d); }
    void setFilterMix(float mix) { filterMix = juce::jlimit(0.0f, 1.0f, mix); }
    void setSubWaveform(SubWave w) { osc.setSubWaveform(w); }
    void setFilterKeyTrack(float kt) { keyTrackAmount = kt; }
    void setFilterEnvDepth(float d) { filterEnvDepth = d; }
    void setAmpEnvelope(float a, float d, float s, float r);
    void setFilterEnvelope(float a, float d, float s, float r);
    void setModEnvelope(float a, float d, float s, float r);

    void applyModulation(const VoiceMods& mods);
    void setLFO1Dest(ModDestination dest) { lfo1Dest = dest; }
    void setLFO2Dest(ModDestination dest) { lfo2Dest = dest; }
    void setSeqDest(ModDestination dest) { seqDest = dest; }
    void setModEnvDest(ModDestination dest) { modEnvDest = dest; }

private:
    Oscillator osc;
    SkuntFilter filter;
    Envelope ampEnv;
    Envelope filterEnv;
    Envelope modEnv;

    double currentSampleRate = 48000.0f;
    float baseCutoff = 2000.0f;
    float keyTrackAmount = 0.3f;
    float filterEnvDepth = 0.7f;
    float filterMix = 1.0f;  // 0 = dry, 1 = fully filtered (Serum-style)
    float pitchBendSemitones = 0.0f;
    float noteVelocity = 1.0f;
    float currentCutoff = 2000.0f;
    float currentFM = 0.3f;
    float currentResonance = 0.3f;
    float currentPitch = 0.0f;
    int currentNote = 60;
    float fmAmount = 0.3f;
    float filterResonance = 0.3f;
    float pitchASemitones = 0.0f;
    float pitchAFine = 0.0f;
    float pitchBSemitones = -12.0f;
    float pitchBFine = 0.0f;

    ModDestination lfo1Dest = ModDestination::FilterCutoff;
    ModDestination lfo2Dest = ModDestination::FMAmount;
    ModDestination seqDest = ModDestination::FilterCutoff;
    ModDestination modEnvDest = ModDestination::Pitch;
};
