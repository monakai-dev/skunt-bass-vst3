#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>
#include "SkuntVoice.h"
#include "SkuntSound.h"
#include "LFO.h"
#include "StepSequencer.h"
#include "Distortion.h"
#include "Multiband.h"
#include "Compressor.h"
#include "StereoWidth.h"
#include "Limiter.h"

class SynthEngine
{
public:
    SynthEngine();
    ~SynthEngine() = default;

    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();

    // MIDI - pass through to internal synth
    void processMidi(const juce::MidiBuffer& midiMessages) { midiBuffer = midiMessages; }

    // Audio render
    void renderVoices(juce::AudioBuffer<float>& output, int startSample, int numSamples);

    // Apply global effects (per-sample)
    void processEffects(float& left, float& right);

    juce::Synthesiser& getSynth() { return synth; }

    // Parameter setters
    void setOscAWave(WaveA w);
    void setOscAPitch(int semis, float fine);
    void setOscALevel(float lvl);
    void setOscBWave(WaveB w);
    void setOscBPitch(int semis, float fine);
    void setOscBLevel(float lvl);
    void setFMAmount(float amt);
    void setSubLevel(float lvl);
    void setNoiseLevel(float lvl);
    void setFilterType(FilterType t);
    void setFilterCutoff(float freq);
    void setFilterResonance(float q);
    void setFilterDrive(float d);
    void setFilterKeyTrack(float kt);
    void setFilterEnvDepth(float d);
    void setAmpEnvelope(float a, float d, float s, float r);
    void setFilterEnvelope(float a, float d, float s, float r);
    void setModEnvelope(float a, float d, float s, float r);

    // Modulation
    void setLFO1Rate(float rate) { lfo1.setFrequency(rate); }
    void setLFO1Shape(LFOShape s) { lfo1.setShape(s); }
    void setLFO1Depth(float d) { lfo1Depth = d; }
    void setLFO1Dest(ModDestination dest);
    void setLFO2Rate(float rate) { lfo2.setFrequency(rate); }
    void setLFO2Shape(LFOShape s) { lfo2.setShape(s); }
    void setLFO2Depth(float d) { lfo2Depth = d; }
    void setLFO2Dest(ModDestination dest);
    void setSeqRate(float rate) { sequencer.setRate(rate); }
    void setSeqSmooth(float s) { sequencer.setSmoothing(s); }
    void setSeqStep(int idx, float val) { sequencer.setStepValue(idx, val); }
    void setSeqDest(ModDestination dest);
    void setModEnvParams(float a, float d, float s, float r);
    void setModEnvDepth(float d) { modEnvDepth = d; }
    void setModEnvDest(ModDestination dest);

    // Effects
    void setDistortionType(DistortionType t) { distortion.setType(t); }
    void setDistortionAmount(float amt) { distortion.setAmount(amt); }
    void setDistortionMix(float mix) { distortion.setMix(mix); }
    void setXoverFrequencies(float low, float high);
    void setCompThreshold(float thresh) { compressor.setThreshold(thresh); }
    void setCompRatio(float ratio) { compressor.setRatio(ratio); }
    void setWidth(float w) { stereoWidth.setWidth(w); }

    // Master
    void setMasterGain(float gainDb) { masterGain = juce::Decibels::decibelsToGain(gainDb); }
    float getMasterGain() const { return masterGain; }

    // Output metering
    float getOutputLevelLeft() const { return outputLevelLeft; }
    float getOutputLevelRight() const { return outputLevelRight; }

private:
    juce::Synthesiser synth;
    SkuntSound* sound = nullptr;

    // Components
    Oscillator osc;
    SkuntFilter filter;
    Envelope ampEnv, filterEnv, modEnv;
    LFO lfo1;
    LFO lfo2;
    StepSequencer sequencer;
    float lfo1Depth = 0.3f;
    float lfo2Depth = 0.2f;
    float modEnvDepth = 0.5f;

    ModDestination lfo1Dest = ModDestination::FilterCutoff;
    ModDestination lfo2Dest = ModDestination::FMAmount;
    ModDestination seqDest = ModDestination::FilterCutoff;
    ModDestination modEnvDest = ModDestination::Pitch;

    // Effects
    Distortion distortion;
    MultibandCrossover crossover;
    Compressor compressor;
    StereoWidth stereoWidth;
    Limiter limiter;

    // Cached parameters
    int pitchASemitones = 0; float pitchAFine = 0.0f;
    float oscALevel = 0.8f;
    int pitchBSemitones = -12; float pitchBFine = 0.0f;
    float oscBLevel = 0.4f;
    float fmAmount = 0.3f;
    float subLevel = 0.5f;
    float noiseLevel = 0.05f;
    FilterType filterType = FilterType::LP12;
    float baseCutoff = 2000.0f;
    float filterResonance = 0.3f;
    float filterDrive = 0.2f;
    float keyTrackAmount = 0.3f;
    float filterEnvDepth = 0.7f;
    float ampAttack = 1.0f, ampDecay = 200.0f, ampSustain = 0.8f, ampRelease = 300.0f;
    float filtAttack = 10.0f, filtDecay = 400.0f, filtSustain = 0.2f, filtRelease = 500.0f;
    float modAttack = 5.0f, modDecay = 300.0f, modSustain = 0.0f, modRelease = 200.0f;

    // Master
    float masterGain = 0.5f;
    float outputLevelLeft = 0.0f;
    float outputLevelRight = 0.0f;

    double sampleRate = 48000.0;
    int blockSize = 256;
    juce::AudioBuffer<float> voiceBuffer;
    juce::MidiBuffer midiBuffer;

    void updateAllVoices();
    SkuntVoice* getVoice(int index);
};
