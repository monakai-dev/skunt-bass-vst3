#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ParameterIds.h"


PluginProcessor::PluginProcessor()
    : AudioProcessor(BusesProperties()
                        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, juce::Identifier("SKUNTBASS"), createParameterLayout())
{
    paramListener = std::make_unique<ParameterListener>(*this);

    // Add all parameters as listeners
    for (auto& param : apvts.processor.getParameters())
    {
        auto* p = dynamic_cast<juce::RangedAudioParameter*>(param);
        if (p) apvts.addParameterListener(p->paramID, paramListener.get());
    }

    // Setup synth
    for (int i = 0; i < 8; ++i)
        synthEngine.getSynth().addVoice(new SkuntVoice());
    synthEngine.getSynth().addSound(new SkuntSound());

    syncAllParameters();
}

PluginProcessor::~PluginProcessor()
{
    if (paramListener)
    {
        for (auto& param : apvts.processor.getParameters())
        {
            auto* p = dynamic_cast<juce::RangedAudioParameter*>(param);
            if (p) apvts.removeParameterListener(p->paramID, paramListener.get());
        }
    }
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32>(getTotalNumOutputChannels());

    synthEngine.prepare(spec);
    syncAllParameters();
}

void PluginProcessor::releaseResources()
{
}

bool PluginProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::mono()
        || layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Process MIDI through synth engine
    synthEngine.getSynth().renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    // Apply global effects per-sample
    for (int i = 0; i < buffer.getNumSamples(); ++i)
    {
        float left = buffer.getSample(0, i);
        float right = buffer.getNumChannels() > 1 ? buffer.getSample(1, i) : left;

        synthEngine.processEffects(left, right);

        buffer.setSample(0, i, left);
        if (buffer.getNumChannels() > 1)
            buffer.setSample(1, i, right);
    }
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor(*this);
}

int PluginProcessor::getCurrentProgram()
{
    return currentProgram;
}

void PluginProcessor::setCurrentProgram(int index)
{
    if (index >= 0 && index < 5)
    {
        currentProgram = index;
        loadPreset(index);
    }
}

const juce::String PluginProcessor::getProgramName(int index)
{
    const char* names[] = { "Init", "Reese Bass", "Neuro Growl", "Techstep Wub", "Sub Destroyer" };
    if (index >= 0 && index < 5) return names[index];
    return "Unknown";
}

void PluginProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void PluginProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr)
        if (xml->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xml));
}

void PluginProcessor::syncParameterToEngine(const juce::String& id, float value)
{
    // value is normalized 0-1; convert to real parameter value
    auto* param = dynamic_cast<juce::RangedAudioParameter*>(apvts.getParameter(id));
    float v = param ? param->getNormalisableRange().convertFrom0to1(value) : value;

    if (id == ParamID::oscAWave)      synthEngine.setOscAWave(static_cast<WaveA>(static_cast<int>(v)));
    else if (id == ParamID::oscAPitch)      synthEngine.setOscAPitch(static_cast<int>(v), 0.0f);
    else if (id == ParamID::oscAFine)       { /* fine is negligible for engine */ }
    else if (id == ParamID::oscALevel)      synthEngine.setOscALevel(v);
    else if (id == ParamID::oscBWave)       synthEngine.setOscBWave(static_cast<WaveB>(static_cast<int>(v)));
    else if (id == ParamID::oscBPitch)      synthEngine.setOscBPitch(static_cast<int>(v), 0.0f);
    else if (id == ParamID::oscBFine)       { }
    else if (id == ParamID::oscBLevel)      synthEngine.setOscBLevel(v);
    else if (id == ParamID::fmAmount)       synthEngine.setFMAmount(v);
    else if (id == ParamID::subLevel)       synthEngine.setSubLevel(v);
    else if (id == ParamID::subWaveform)    synthEngine.setSubWaveform(static_cast<SubWave>(static_cast<int>(v)));
    else if (id == ParamID::noiseLevel)     synthEngine.setNoiseLevel(v);
    else if (id == ParamID::filterType)     synthEngine.setFilterType(static_cast<FilterType>(static_cast<int>(v)));
    else if (id == ParamID::filterCutoff)   synthEngine.setFilterCutoff(v);
    else if (id == ParamID::filterResonance) synthEngine.setFilterResonance(v);
    else if (id == ParamID::filterDrive)    synthEngine.setFilterDrive(v);
    else if (id == ParamID::filterMix)      synthEngine.setFilterMix(v);
    else if (id == ParamID::filterKeyTrack) synthEngine.setFilterKeyTrack(v);
    else if (id == ParamID::filterEnvDepth) synthEngine.setFilterEnvDepth(v);
    else if (id == ParamID::lfo1Rate)       synthEngine.setLFO1Rate(v);
    else if (id == ParamID::lfo1Shape)      synthEngine.setLFO1Shape(static_cast<LFOShape>(static_cast<int>(v)));
    else if (id == ParamID::lfo1Depth)      synthEngine.setLFO1Depth(v);
    else if (id == ParamID::lfo1Dest)       synthEngine.setLFO1Dest(static_cast<ModDestination>(static_cast<int>(v)));
    else if (id == ParamID::lfo2Rate)       synthEngine.setLFO2Rate(v);
    else if (id == ParamID::lfo2Shape)      synthEngine.setLFO2Shape(static_cast<LFOShape>(static_cast<int>(v)));
    else if (id == ParamID::lfo2Depth)      synthEngine.setLFO2Depth(v);
    else if (id == ParamID::lfo2Dest)       synthEngine.setLFO2Dest(static_cast<ModDestination>(static_cast<int>(v)));
    else if (id == ParamID::seqRate)        synthEngine.setSeqRate(v);
    else if (id == ParamID::seqSmooth)      synthEngine.setSeqSmooth(v);
    else if (id == ParamID::seqDest)        synthEngine.setSeqDest(static_cast<ModDestination>(static_cast<int>(v)));
    else if (id == ParamID::seqStep1)       synthEngine.setSeqStep(0, v);
    else if (id == ParamID::seqStep2)       synthEngine.setSeqStep(1, v);
    else if (id == ParamID::seqStep3)       synthEngine.setSeqStep(2, v);
    else if (id == ParamID::seqStep4)       synthEngine.setSeqStep(3, v);
    else if (id == ParamID::seqStep5)       synthEngine.setSeqStep(4, v);
    else if (id == ParamID::seqStep6)       synthEngine.setSeqStep(5, v);
    else if (id == ParamID::seqStep7)       synthEngine.setSeqStep(6, v);
    else if (id == ParamID::seqStep8)       synthEngine.setSeqStep(7, v);
    else if (id == ParamID::distType)       synthEngine.setDistortionType(static_cast<DistortionType>(static_cast<int>(v)));
    else if (id == ParamID::distAmount)     synthEngine.setDistortionAmount(v);
    else if (id == ParamID::distMix)        synthEngine.setDistortionMix(v);
    else if (id == ParamID::xoverLow)       { /* handled with xoverHigh */ }
    else if (id == ParamID::xoverHigh)      { /* handled with xoverLow */ }
    else if (id == ParamID::compThresh)     synthEngine.setCompThreshold(v);
    else if (id == ParamID::compRatio)      synthEngine.setCompRatio(v / 20.0f);
    else if (id == ParamID::widthAmount)    synthEngine.setWidth(v);
    else if (id == ParamID::masterGain)     synthEngine.setMasterGain(v);
}

void PluginProcessor::syncAllParameters()
{
    for (auto& param : apvts.processor.getParameters())
    {
        auto* p = dynamic_cast<juce::RangedAudioParameter*>(param);
        if (p) syncParameterToEngine(p->paramID, p->getValue());
    }

    // Push all envelope params
    auto* a = apvts.getRawParameterValue(ParamID::ampAttack);
    auto* d = apvts.getRawParameterValue(ParamID::ampDecay);
    auto* s = apvts.getRawParameterValue(ParamID::ampSustain);
    auto* r = apvts.getRawParameterValue(ParamID::ampRelease);
    if (a && d && s && r) synthEngine.setAmpEnvelope(*a, *d, *s, *r);

    a = apvts.getRawParameterValue(ParamID::filterAttack);
    d = apvts.getRawParameterValue(ParamID::filterDecay);
    s = apvts.getRawParameterValue(ParamID::filterSustain);
    r = apvts.getRawParameterValue(ParamID::filterRelease);
    if (a && d && s && r) synthEngine.setFilterEnvelope(*a, *d, *s, *r);

    a = apvts.getRawParameterValue(ParamID::modAttack);
    d = apvts.getRawParameterValue(ParamID::modDecay);
    s = apvts.getRawParameterValue(ParamID::modSustain);
    r = apvts.getRawParameterValue(ParamID::modRelease);
    if (a && d && s && r) synthEngine.setModEnvelope(*a, *d, *s, *r);

    auto* xl = apvts.getRawParameterValue(ParamID::xoverLow);
    auto* xh = apvts.getRawParameterValue(ParamID::xoverHigh);
    if (xl && xh) synthEngine.setXoverFrequencies(*xl, *xh);
}

void PluginProcessor::loadPreset(int presetIndex)
{
    auto* tree = &apvts;

    auto setVal = [&](const char* id, float val)
    {
        if (auto* p = tree->getParameter(id))
            p->setValueNotifyingHost(p->getNormalisableRange().convertTo0to1(val));
    };

    auto setChoice = [&](const char* id, int val)
    {
        if (auto* p = tree->getParameter(id))
            p->setValueNotifyingHost(p->getNormalisableRange().convertTo0to1(static_cast<float>(val)));
    };

    // ================================================================
    // NEUROFUNK BASS PRESETS — Designed for proper frequency ranges:
    // Sub (<90 Hz):     Clean sine, Direct Out, 0.5-0.85 level
    // Low-Mid (90-300 Hz):  Warmth from osc fundamentals
    // Mid (300 Hz-2 kHz):   "Talking" character — shaped by filter
    // High (>2 kHz):        Tearing texture — from filter env sweep
    // ================================================================

    if (presetIndex == 0) // Init
    {
        // Clean starting point — moderate everything
        setChoice(ParamID::oscAWave, 1); setVal(ParamID::oscAPitch, 0); setVal(ParamID::oscAFine, 0); setVal(ParamID::oscALevel, 0.9f);
        setChoice(ParamID::oscBWave, 1); setVal(ParamID::oscBPitch, -12); setVal(ParamID::oscBFine, 0); setVal(ParamID::oscBLevel, 0.4f);
        setVal(ParamID::fmAmount, 0.1f); setVal(ParamID::subLevel, 0.5f); setChoice(ParamID::subWaveform, 0); setVal(ParamID::noiseLevel, 0.0f);
        // Filter: LP12, base 500 Hz (passes sub + low-mid, shapes midrange)
        setChoice(ParamID::filterType, 0); setVal(ParamID::filterCutoff, 500); setVal(ParamID::filterResonance, 0.4f);
        setVal(ParamID::filterDrive, 0.2f); setVal(ParamID::filterMix, 0.8f); setVal(ParamID::filterKeyTrack, 0.3f); setVal(ParamID::filterEnvDepth, 0.3f);
        setVal(ParamID::ampAttack, 0.5f); setVal(ParamID::ampDecay, 250); setVal(ParamID::ampSustain, 0.8f); setVal(ParamID::ampRelease, 350);
        setVal(ParamID::filterAttack, 3); setVal(ParamID::filterDecay, 300); setVal(ParamID::filterSustain, 0.3f); setVal(ParamID::filterRelease, 400);
        setVal(ParamID::lfo1Rate, 4); setChoice(ParamID::lfo1Shape, 0); setVal(ParamID::lfo1Depth, 0.0f); setChoice(ParamID::lfo1Dest, 0);
        setVal(ParamID::lfo2Rate, 0.25f); setChoice(ParamID::lfo2Shape, 3); setVal(ParamID::lfo2Depth, 0.0f); setChoice(ParamID::lfo2Dest, 2);
        setChoice(ParamID::distType, 1); setVal(ParamID::distAmount, 0.2f); setVal(ParamID::distMix, 0.15f);
        setVal(ParamID::compThresh, -12); setVal(ParamID::compRatio, 4);
        setVal(ParamID::masterGain, -3);
    }
    else if (presetIndex == 1) // Reese Bass
    {
        // ANALYZED from neurofunk-reese-bass_G_minor.wav:
        // Fundamental ~57 Hz, beating ~4 Hz, attack ~15ms
        // Steep LP24 filter, strong sub, detuned saws
        setChoice(ParamID::oscAWave, 1); setVal(ParamID::oscAPitch, 0); setVal(ParamID::oscAFine, 15);
        setChoice(ParamID::oscBWave, 1); setVal(ParamID::oscBPitch, 0); setVal(ParamID::oscBFine, -15);
        setVal(ParamID::fmAmount, 0.0f); setVal(ParamID::subLevel, 0.65f); setChoice(ParamID::subWaveform, 0); setVal(ParamID::noiseLevel, 0.0f);
        // LP24 at 240 Hz — reference shows steep rolloff above ~300 Hz
        setChoice(ParamID::filterType, 1); setVal(ParamID::filterCutoff, 240); setVal(ParamID::filterResonance, 0.38f);
        setVal(ParamID::filterDrive, 0.3f); setVal(ParamID::filterMix, 0.9f); setVal(ParamID::filterKeyTrack, 0.15f); setVal(ParamID::filterEnvDepth, 0.65f);
        // Attack ~15ms like reference
        setVal(ParamID::ampAttack, 15); setVal(ParamID::ampDecay, 400); setVal(ParamID::ampSustain, 0.92f); setVal(ParamID::ampRelease, 550);
        setVal(ParamID::filterAttack, 25); setVal(ParamID::filterDecay, 320); setVal(ParamID::filterSustain, 0.3f); setVal(ParamID::filterRelease, 480);
        // Subtle movement
        setVal(ParamID::lfo1Rate, 0.25f); setChoice(ParamID::lfo1Shape, 0); setVal(ParamID::lfo1Depth, 0.03f); setChoice(ParamID::lfo1Dest, 0);
        setChoice(ParamID::distType, 1); setVal(ParamID::distAmount, 0.28f); setVal(ParamID::distMix, 0.22f);
        setVal(ParamID::compThresh, -10); setVal(ParamID::compRatio, 4.0f);
        setVal(ParamID::masterGain, -1);
    }
    else if (presetIndex == 2) // Neuro Growl
    {
        // FM growl + resonant filter sweep = vocal "talking" character
        // Osc A at +12st adds upper harmonics for the "tearing" texture
        setChoice(ParamID::oscAWave, 1); setVal(ParamID::oscAPitch, 12); setVal(ParamID::oscAFine, 0);
        setChoice(ParamID::oscBWave, 1); setVal(ParamID::oscBPitch, 0); setVal(ParamID::oscBFine, 7);
        setVal(ParamID::fmAmount, 0.4f); setVal(ParamID::subLevel, 0.45f); setChoice(ParamID::subWaveform, 0); setVal(ParamID::noiseLevel, 0.03f);
        // Filter: base 400 Hz, res 0.6 (high resonance for vocal "talking")
        // Envelope depth 0.75 — sweeps to ~4 kHz for aggressive "tear"
        setChoice(ParamID::filterType, 0); setVal(ParamID::filterCutoff, 400); setVal(ParamID::filterResonance, 0.6f);
        setVal(ParamID::filterDrive, 0.45f); setVal(ParamID::filterMix, 0.85f); setVal(ParamID::filterKeyTrack, 0.3f); setVal(ParamID::filterEnvDepth, 0.75f);
        // Fast filter envelope for snappy attack
        setVal(ParamID::ampAttack, 1); setVal(ParamID::ampDecay, 300); setVal(ParamID::ampSustain, 0.65f); setVal(ParamID::ampRelease, 400);
        setVal(ParamID::filterAttack, 2); setVal(ParamID::filterDecay, 180); setVal(ParamID::filterSustain, 0.1f); setVal(ParamID::filterRelease, 280);
        // Mod env for extra movement
        setVal(ParamID::modAttack, 2); setVal(ParamID::modDecay, 120); setVal(ParamID::modSustain, 0.0f); setVal(ParamID::modRelease, 180);
        setVal(ParamID::modDepth, 0.5f);
        // LFO: 3 Hz sine → cutoff for evolving morph
        setVal(ParamID::lfo1Rate, 3.0f); setChoice(ParamID::lfo1Shape, 0); setVal(ParamID::lfo1Depth, 0.15f); setChoice(ParamID::lfo1Dest, 0);
        // Foldback distortion for the "bio-mechanical" grit
        setChoice(ParamID::distType, 2); setVal(ParamID::distAmount, 0.5f); setVal(ParamID::distMix, 0.35f);
        setVal(ParamID::compThresh, -16); setVal(ParamID::compRatio, 5);
        setVal(ParamID::masterGain, -3);
    }
    else if (presetIndex == 3) // Techstep Wub
    {
        // LFO-modulated filter for rhythmic "wub-wub"
        // Square sub for punchy low end
        setChoice(ParamID::oscAWave, 1); setVal(ParamID::oscAPitch, 0); setVal(ParamID::oscAFine, 0);
        setChoice(ParamID::oscBWave, 2); setVal(ParamID::oscBPitch, -12); setVal(ParamID::oscBFine, 0);
        setVal(ParamID::fmAmount, 0.15f); setVal(ParamID::subLevel, 0.55f); setChoice(ParamID::subWaveform, 1); setVal(ParamID::noiseLevel, 0.0f);
        // Filter base 350 Hz — lower for deeper wub
        setChoice(ParamID::filterType, 0); setVal(ParamID::filterCutoff, 350); setVal(ParamID::filterResonance, 0.5f);
        setVal(ParamID::filterDrive, 0.25f); setVal(ParamID::filterMix, 0.8f); setVal(ParamID::filterKeyTrack, 0.2f); setVal(ParamID::filterEnvDepth, 0.5f);
        setVal(ParamID::ampAttack, 3); setVal(ParamID::ampDecay, 400); setVal(ParamID::ampSustain, 0.8f); setVal(ParamID::ampRelease, 500);
        setVal(ParamID::filterAttack, 10); setVal(ParamID::filterDecay, 400); setVal(ParamID::filterSustain, 0.2f); setVal(ParamID::filterRelease, 500);
        // LFO 2: 2 Hz square → cutoff for the wobble
        setVal(ParamID::lfo2Rate, 2.0f); setChoice(ParamID::lfo2Shape, 3); setVal(ParamID::lfo2Depth, 0.65f); setChoice(ParamID::lfo2Dest, 0);
        // Step sequencer for rhythmic variation
        setVal(ParamID::seqRate, 3); setChoice(ParamID::seqDest, 0);
        setVal(ParamID::seqStep1, 1.0f); setVal(ParamID::seqStep2, 0.1f); setVal(ParamID::seqStep3, 0.85f); setVal(ParamID::seqStep4, 0.1f);
        setVal(ParamID::seqStep5, 0.6f); setVal(ParamID::seqStep6, 0.1f); setVal(ParamID::seqStep7, 0.9f); setVal(ParamID::seqStep8, 0.1f);
        setChoice(ParamID::distType, 1); setVal(ParamID::distAmount, 0.25f); setVal(ParamID::distMix, 0.15f);
        setVal(ParamID::compThresh, -14); setVal(ParamID::compRatio, 4);
        setVal(ParamID::masterGain, -4);
    }
    else if (presetIndex == 4) // Sub Destroyer
    {
        // Maximum sub weight — clean, punchy, minimal harmonics
        // Sine oscillators for pure tone, heavy sub via Direct Out
        setChoice(ParamID::oscAWave, 3); setVal(ParamID::oscAPitch, 0); setVal(ParamID::oscAFine, 0); setVal(ParamID::oscALevel, 0.4f);
        setChoice(ParamID::oscBWave, 3); setVal(ParamID::oscBPitch, -12); setVal(ParamID::oscBFine, 0); setVal(ParamID::oscBLevel, 0.3f);
        setVal(ParamID::fmAmount, 0.08f); setVal(ParamID::subLevel, 0.85f); setChoice(ParamID::subWaveform, 0); setVal(ParamID::noiseLevel, 0.0f);
        // Filter: higher cutoff (600 Hz) since sines have no harmonics to shape
        setChoice(ParamID::filterType, 0); setVal(ParamID::filterCutoff, 600); setVal(ParamID::filterResonance, 0.3f);
        setVal(ParamID::filterDrive, 0.1f); setVal(ParamID::filterMix, 0.6f); setVal(ParamID::filterKeyTrack, 0.5f); setVal(ParamID::filterEnvDepth, 0.0f);
        // Fast punchy envelopes
        setVal(ParamID::ampAttack, 0.5f); setVal(ParamID::ampDecay, 200); setVal(ParamID::ampSustain, 0.95f); setVal(ParamID::ampRelease, 300);
        setVal(ParamID::filterAttack, 2); setVal(ParamID::filterDecay, 200); setVal(ParamID::filterSustain, 0.2f); setVal(ParamID::filterRelease, 300);
        // Minimal distortion — let the pure sub do the work
        setChoice(ParamID::distType, 1); setVal(ParamID::distAmount, 0.1f); setVal(ParamID::distMix, 0.08f);
        setVal(ParamID::compThresh, -10); setVal(ParamID::compRatio, 8);
        setVal(ParamID::widthAmount, 0.0f);  // Mono sub
        setVal(ParamID::masterGain, -2);
    }

    syncAllParameters();
}

float PluginProcessor::getOutputLevelLeft() const
{
    return synthEngine.getOutputLevelLeft();
}

float PluginProcessor::getOutputLevelRight() const
{
    return synthEngine.getOutputLevelRight();
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}


void PluginProcessor::ParameterListener::parameterChanged(const juce::String& parameterID, float newValue)
{
    processor.syncParameterToEngine(parameterID, newValue);
}
