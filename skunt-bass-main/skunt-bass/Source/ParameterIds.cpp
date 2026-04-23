#include "ParameterIds.h"

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Oscillator A
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID(ParamID::oscAWave, 1), "Osc A Wave",
        juce::StringArray{"Wavetable", "Saw", "Square", "Sine"}, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::oscAPitch, 1), "Osc A Pitch",
        juce::NormalisableRange<float>(-24.0f, 24.0f, 1.0f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::oscAFine, 1), "Osc A Fine",
        juce::NormalisableRange<float>(-100.0f, 100.0f, 0.1f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::oscALevel, 1), "Osc A Level",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.8f));

    // Oscillator B
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID(ParamID::oscBWave, 1), "Osc B Wave",
        juce::StringArray{"Saw", "Square", "Sine", "Noise"}, 1));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::oscBPitch, 1), "Osc B Pitch",
        juce::NormalisableRange<float>(-24.0f, 24.0f, 1.0f), -12.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::oscBFine, 1), "Osc B Fine",
        juce::NormalisableRange<float>(-100.0f, 100.0f, 0.1f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::oscBLevel, 1), "Osc B Level",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.4f));

    // FM & Sub & Noise
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::fmAmount, 1), "FM Amount",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.3f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::subLevel, 1), "Sub Level",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::noiseLevel, 1), "Noise Level",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.05f));

    // Filter
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID(ParamID::filterType, 1), "Filter Type",
        juce::StringArray{"LP12", "LP24", "BP", "HP"}, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::filterCutoff, 1), "Cutoff",
        juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.3f), 2000.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::filterResonance, 1), "Resonance",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.3f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::filterDrive, 1), "Drive",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.2f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::filterKeyTrack, 1), "Key Track",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.3f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::filterEnvDepth, 1), "Env Depth",
        juce::NormalisableRange<float>(-1.0f, 1.0f), 0.7f));

    // Amp Envelope
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::ampAttack, 1), "Amp Attack",
        juce::NormalisableRange<float>(0.0f, 5000.0f, 0.1f, 0.4f), 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::ampDecay, 1), "Amp Decay",
        juce::NormalisableRange<float>(0.0f, 5000.0f, 0.1f, 0.4f), 200.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::ampSustain, 1), "Amp Sustain",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.8f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::ampRelease, 1), "Amp Release",
        juce::NormalisableRange<float>(0.0f, 5000.0f, 0.1f, 0.4f), 300.0f));

    // Filter Envelope
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::filterAttack, 1), "Filter Attack",
        juce::NormalisableRange<float>(0.0f, 5000.0f, 0.1f, 0.4f), 10.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::filterDecay, 1), "Filter Decay",
        juce::NormalisableRange<float>(0.0f, 5000.0f, 0.1f, 0.4f), 400.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::filterSustain, 1), "Filter Sustain",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.2f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::filterRelease, 1), "Filter Release",
        juce::NormalisableRange<float>(0.0f, 5000.0f, 0.1f, 0.4f), 500.0f));

    // LFO 1
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID(ParamID::lfo1Shape, 1), "LFO1 Shape",
        juce::StringArray{"Sine", "Triangle", "Saw", "Square", "S&H", "Noise"}, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::lfo1Rate, 1), "LFO1 Rate",
        juce::NormalisableRange<float>(0.01f, 20.0f, 0.01f, 0.5f), 4.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::lfo1Depth, 1), "LFO1 Depth",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.3f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID(ParamID::lfo1Dest, 1), "LFO1 Dest",
        juce::StringArray{"Cutoff", "Pitch", "FM", "Resonance"}, 0));

    // LFO 2
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID(ParamID::lfo2Shape, 1), "LFO2 Shape",
        juce::StringArray{"Sine", "Triangle", "Saw", "Square", "S&H", "Noise"}, 3));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::lfo2Rate, 1), "LFO2 Rate",
        juce::NormalisableRange<float>(0.01f, 20.0f, 0.01f, 0.5f), 0.25f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::lfo2Depth, 1), "LFO2 Depth",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.2f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID(ParamID::lfo2Dest, 1), "LFO2 Dest",
        juce::StringArray{"Cutoff", "Pitch", "FM", "Resonance"}, 2));

    // Step Sequencer
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::seqRate, 1), "Seq Rate",
        juce::NormalisableRange<float>(0.01f, 10.0f, 0.01f, 0.5f), 2.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::seqStep1, 1), "Seq Step 1",
        juce::NormalisableRange<float>(0.0f, 1.0f), 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::seqStep2, 1), "Seq Step 2",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.3f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::seqStep3, 1), "Seq Step 3",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.7f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::seqStep4, 1), "Seq Step 4",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::seqStep5, 1), "Seq Step 5",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.9f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::seqStep6, 1), "Seq Step 6",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.2f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::seqStep7, 1), "Seq Step 7",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.6f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::seqStep8, 1), "Seq Step 8",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.4f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::seqSmooth, 1), "Seq Smooth",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.1f));
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID(ParamID::seqDest, 1), "Seq Dest",
        juce::StringArray{"Cutoff", "Pitch", "FM", "Resonance"}, 0));

    // Mod Envelope
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::modAttack, 1), "Mod Attack",
        juce::NormalisableRange<float>(0.0f, 5000.0f, 0.1f, 0.4f), 5.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::modDecay, 1), "Mod Decay",
        juce::NormalisableRange<float>(0.0f, 5000.0f, 0.1f, 0.4f), 300.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::modSustain, 1), "Mod Sustain",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::modRelease, 1), "Mod Release",
        juce::NormalisableRange<float>(0.0f, 5000.0f, 0.1f, 0.4f), 200.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::modDepth, 1), "Mod Depth",
        juce::NormalisableRange<float>(-1.0f, 1.0f), 0.5f));

    // Effects
    params.push_back(std::make_unique<juce::AudioParameterChoice>(juce::ParameterID(ParamID::distType, 1), "Dist Type",
        juce::StringArray{"Tube", "Foldback", "Bitcrush"}, 1));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::distAmount, 1), "Dist Amount",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.4f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::distMix, 1), "Dist Mix",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.3f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::xoverLow, 1), "Xover Low",
        juce::NormalisableRange<float>(50.0f, 500.0f, 1.0f, 0.5f), 150.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::xoverHigh, 1), "Xover High",
        juce::NormalisableRange<float>(1000.0f, 10000.0f, 1.0f, 0.5f), 4000.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::compThresh, 1), "Comp Thresh",
        juce::NormalisableRange<float>(-60.0f, 0.0f), -20.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::compRatio, 1), "Comp Ratio",
        juce::NormalisableRange<float>(1.0f, 20.0f, 0.1f), 4.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::widthAmount, 1), "Width",
        juce::NormalisableRange<float>(0.0f, 1.0f), 0.3f));

    // Master
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID(ParamID::masterGain, 1), "Master Gain",
        juce::NormalisableRange<float>(-60.0f, 12.0f), -6.0f));

    return { params.begin(), params.end() };
}
