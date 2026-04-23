#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "ParameterIds.h"
#include "DSP/SynthEngine.h"

class PluginEditor;

class PluginProcessor : public juce::AudioProcessor
{
public:
    PluginProcessor();
    ~PluginProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 2.0; }

    int getNumPrograms() override { return 5; }
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }
    float getOutputLevelLeft() const;
    float getOutputLevelRight() const;
    SynthEngine& getSynthEngine() { return synthEngine; }

private:
    juce::AudioProcessorValueTreeState apvts;
    std::atomic<float>* params[64] = {};
    int numParams = 0;

    SynthEngine synthEngine;

    // Parameter listener
    class ParameterListener : public juce::AudioProcessorValueTreeState::Listener
    {
    public:
        ParameterListener(PluginProcessor& p) : processor(p) {}
        void parameterChanged(const juce::String& parameterID, float newValue) override;
    private:
        PluginProcessor& processor;
    };

    std::unique_ptr<ParameterListener> paramListener;

    void syncParameterToEngine(const juce::String& id, float value);
    void syncAllParameters();
    void loadPreset(int presetIndex);
    int currentProgram = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};
