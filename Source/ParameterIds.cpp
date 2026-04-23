#include "ParameterIds.h"

using namespace juce;

AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    // --- OSCILLATOR A ---
    params.push_back(std::make_unique<AudioParameterChoice>(juce::ParameterID(ParamID::oscAWave, 1), "Osc A Wave",
        juce::StringArray{"Wavetable", "Saw", "Square", "Sine"}, 1));

    // Pitch A: -20 to +20 semitones, step 1 (USER REQUEST)
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::oscAPitch, 1), "Osc A Pitch",
        NormalisableRange<float>(-20.0f, 20.0f, 1.0f), 0.0f,
        AudioParameterFloatAttributes().withLabel("st")));

    // Fine A: -100 to +100 cents, step 1 (USER REQUEST)
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::oscAFine, 1), "Osc A Fine",
        NormalisableRange<float>(-100.0f, 100.0f, 1.0f), 0.0f,
        AudioParameterFloatAttributes().withLabel("ct")));

    // Level A: 0-1, step 0.01 (Serum/Operator standard mix level)
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::oscALevel, 1), "Osc A Level",
        NormalisableRange<float>(0.0f, 1.0f, 0.01f), 1.0f));

    // --- OSCILLATOR B ---
    params.push_back(std::make_unique<AudioParameterChoice>(juce::ParameterID(ParamID::oscBWave, 1), "Osc B Wave",
        juce::StringArray{"Saw", "Square", "Sine", "Noise"}, 0));

    // Pitch B: -20 to +20 semitones, step 1 (Serum standard)
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::oscBPitch, 1), "Osc B Pitch",
        NormalisableRange<float>(-20.0f, 20.0f, 1.0f), -12.0f,
        AudioParameterFloatAttributes().withLabel("st")));

    // Fine B: -100 to +100 cents, step 1 (Serum standard)
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::oscBFine, 1), "Osc B Fine",
        NormalisableRange<float>(-100.0f, 100.0f, 1.0f), 0.0f,
        AudioParameterFloatAttributes().withLabel("ct")));

    // Level B: 0-1, step 0.01
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::oscBLevel, 1), "Osc B Level",
        NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));

    // --- FM & SUB ---
    // FM Amount: 0-1, step 0.01 (FM8/Operator standard)
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::fmAmount, 1), "FM Amount",
        NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.1f));

    // Sub Level: 0-1, step 0.01 (Serum standard)
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::subLevel, 1), "Sub Level",
        NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));

    params.push_back(std::make_unique<AudioParameterChoice>(juce::ParameterID(ParamID::subWaveform, 1), "Sub Wave",
        juce::StringArray{"Sine", "Square", "Saw", "Triangle"}, 0));

    // Noise: 0-1, step 0.01 (Serum standard)
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::noiseLevel, 1), "Noise Level",
        NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));

    // --- FILTER ---
    params.push_back(std::make_unique<AudioParameterChoice>(juce::ParameterID(ParamID::filterType, 1), "Filter Type",
        juce::StringArray{"LP12", "LP24", "BP6", "HP12"}, 0));

    // Cutoff: 40 Hz to 10 kHz, step 10 Hz (USER REQUEST)
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::filterCutoff, 1), "Cutoff",
        NormalisableRange<float>(40.0f, 10000.0f, 10.0f), 500.0f,
        AudioParameterFloatAttributes().withLabel("Hz")));

    // Resonance: 0-1, step 0.01 (Serum/Operator standard)
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::filterResonance, 1), "Resonance",
        NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.35f));

    // Drive: 0-1, step 0.01 (Operator filter drive standard)
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::filterDrive, 1), "Drive",
        NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.3f));

    // Filter Mix: 0-1, step 0.01 (standard blend control)
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::filterMix, 1), "Filter Mix",
        NormalisableRange<float>(0.0f, 1.0f, 0.01f), 1.0f));

    // Key Track: 0-1, step 0.01 (Serum standard)
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::filterKeyTrack, 1), "Key Track",
        NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));

    // Filter Env Depth: 0-1, step 0.01 (standard)
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::filterEnvDepth, 1), "Env Depth",
        NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.5f));

    // --- AMP ENVELOPE ---
    // Attack: 0.1 ms to 10000 ms, step 0.1 (standard synth)
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::ampAttack, 1), "Amp Attack",
        NormalisableRange<float>(0.1f, 10000.0f, 0.1f),
        1.0f,
        AudioParameterFloatAttributes().withLabel("ms")));

    // Decay: 1 ms to 10000 ms, step 0.1
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::ampDecay, 1), "Amp Decay",
        NormalisableRange<float>(1.0f, 10000.0f, 0.1f),
        250.0f,
        AudioParameterFloatAttributes().withLabel("ms")));

    // Sustain: 0-1, step 0.01 (level, standard)
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::ampSustain, 1), "Amp Sustain",
        NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.8f));

    // Release: 1 ms to 10000 ms, step 0.1
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::ampRelease, 1), "Amp Release",
        NormalisableRange<float>(1.0f, 10000.0f, 0.1f),
        350.0f,
        AudioParameterFloatAttributes().withLabel("ms")));

    // --- FILTER ENVELOPE ---
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::filterAttack, 1), "Filter Attack",
        NormalisableRange<float>(0.1f, 10000.0f, 0.1f),
        3.0f,
        AudioParameterFloatAttributes().withLabel("ms")));

    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::filterDecay, 1), "Filter Decay",
        NormalisableRange<float>(1.0f, 10000.0f, 0.1f),
        300.0f,
        AudioParameterFloatAttributes().withLabel("ms")));

    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::filterSustain, 1), "Filter Sustain",
        NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.3f));

    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::filterRelease, 1), "Filter Release",
        NormalisableRange<float>(1.0f, 10000.0f, 0.1f),
        450.0f,
        AudioParameterFloatAttributes().withLabel("ms")));

    // --- MOD ENVELOPE ---
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::modAttack, 1), "Mod Attack",
        NormalisableRange<float>(0.1f, 10000.0f, 0.1f),
        2.0f,
        AudioParameterFloatAttributes().withLabel("ms")));

    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::modDecay, 1), "Mod Decay",
        NormalisableRange<float>(1.0f, 10000.0f, 0.1f),
        150.0f,
        AudioParameterFloatAttributes().withLabel("ms")));

    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::modSustain, 1), "Mod Sustain",
        NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));

    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::modRelease, 1), "Mod Release",
        NormalisableRange<float>(1.0f, 10000.0f, 0.1f),
        200.0f,
        AudioParameterFloatAttributes().withLabel("ms")));

    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::modDepth, 1), "Mod Depth",
        NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));

    // --- LFO 1 ---
    // Rate: 0.01 Hz to 100 Hz, step 0.01 (standard synth LFO)
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::lfo1Rate, 1), "LFO1 Rate",
        NormalisableRange<float>(0.01f, 100.0f, 0.01f),
        4.0f,
        AudioParameterFloatAttributes().withLabel("Hz")));

    params.push_back(std::make_unique<AudioParameterChoice>(juce::ParameterID(ParamID::lfo1Shape, 1), "LFO1 Shape",
        juce::StringArray{"Sine", "Triangle", "Saw", "Square", "Random"}, 0));

    // Depth: 0-1, step 0.01
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::lfo1Depth, 1), "LFO1 Depth",
        NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));

    params.push_back(std::make_unique<AudioParameterChoice>(juce::ParameterID(ParamID::lfo1Dest, 1), "LFO1 Dest",
        juce::StringArray{"Filter Cutoff", "Osc Pitch", "Osc Level", "Pan", "Resonance"}, 0));

    // --- LFO 2 ---
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::lfo2Rate, 1), "LFO2 Rate",
        NormalisableRange<float>(0.01f, 100.0f, 0.01f),
        0.25f,
        AudioParameterFloatAttributes().withLabel("Hz")));

    params.push_back(std::make_unique<AudioParameterChoice>(juce::ParameterID(ParamID::lfo2Shape, 1), "LFO2 Shape",
        juce::StringArray{"Sine", "Triangle", "Saw", "Square", "Random"}, 0));

    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::lfo2Depth, 1), "LFO2 Depth",
        NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));

    params.push_back(std::make_unique<AudioParameterChoice>(juce::ParameterID(ParamID::lfo2Dest, 1), "LFO2 Dest",
        juce::StringArray{"Filter Cutoff", "Osc Pitch", "Osc Level", "Pan", "Resonance"}, 0));

    // --- STEP SEQUENCER ---
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::seqRate, 1), "Seq Rate",
        NormalisableRange<float>(0.1f, 50.0f, 0.1f),
        4.0f,
        AudioParameterFloatAttributes().withLabel("Hz")));

    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::seqSmooth, 1), "Seq Smooth",
        NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f));

    params.push_back(std::make_unique<AudioParameterChoice>(juce::ParameterID(ParamID::seqDest, 1), "Seq Dest",
        juce::StringArray{"Filter Cutoff", "Osc Pitch", "Osc Level", "Pan", "Resonance"}, 0));

    for (int i = 0; i < 8; ++i)
    {
        params.push_back(std::make_unique<AudioParameterFloat>(
            juce::ParameterID("seqStep" + juce::String(i + 1), 1),
            "Step " + juce::String(i + 1),
            NormalisableRange<float>(0.0f, 1.0f, 0.01f),
            0.5f));
    }

    // --- DISTORTION ---
    params.push_back(std::make_unique<AudioParameterChoice>(juce::ParameterID(ParamID::distType, 1), "Distortion Type",
        juce::StringArray{"Tube", "Foldback", "Bitcrush"}, 0));

    // Amount: 0-1, step 0.01
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::distAmount, 1), "Distortion",
        NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.25f));

    // Mix: 0-1, step 0.01 (standard dry/wet)
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::distMix, 1), "Distortion Mix",
        NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.2f));

    // --- CROSSOVER ---
    // Low: 20 Hz to 2000 Hz, step 1 Hz
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::xoverLow, 1), "X-Over Low",
        NormalisableRange<float>(20.0f, 2000.0f, 1.0f),
        200.0f,
        AudioParameterFloatAttributes().withLabel("Hz")));

    // High: 200 Hz to 8000 Hz, step 1 Hz
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::xoverHigh, 1), "X-Over High",
        NormalisableRange<float>(200.0f, 8000.0f, 1.0f),
        2000.0f,
        AudioParameterFloatAttributes().withLabel("Hz")));

    // --- COMPRESSOR ---
    // Threshold: -60 dB to 0 dB, step 0.1 dB (standard)
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::compThresh, 1), "Comp Threshold",
        NormalisableRange<float>(-60.0f, 0.0f, 0.1f), -12.0f,
        AudioParameterFloatAttributes().withLabel("dB")));

    // Ratio: 1:1 to 20:1, step 0.1 (standard)
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::compRatio, 1), "Comp Ratio",
        NormalisableRange<float>(1.0f, 20.0f, 0.1f), 4.0f,
        AudioParameterFloatAttributes().withLabel(":1")));

    // --- STEREO WIDTH ---
    // 0-2 (0=mono, 1=normal, 2=wide), step 0.01
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::widthAmount, 1), "Stereo Width",
        NormalisableRange<float>(0.0f, 2.0f, 0.01f), 1.0f));

    // --- MASTER GAIN ---
    // -60 dB to +12 dB, step 1 dB (USER REQUEST)
    params.push_back(std::make_unique<AudioParameterFloat>(juce::ParameterID(ParamID::masterGain, 1), "Master Gain",
        NormalisableRange<float>(-60.0f, 12.0f, 1.0f), 0.0f,
        AudioParameterFloatAttributes().withLabel("dB")));

    return { params.begin(), params.end() };
}
