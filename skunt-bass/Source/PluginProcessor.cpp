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
    if (id == ParamID::oscAWave)      synthEngine.setOscAWave(static_cast<WaveA>(static_cast<int>(value)));
    else if (id == ParamID::oscAPitch)      { /* pitch handled by osc */ }
    else if (id == ParamID::oscAFine)       { /* fine handled by osc */ }
    else if (id == ParamID::oscALevel)      synthEngine.setOscALevel(value);
    else if (id == ParamID::oscBWave)       synthEngine.setOscBWave(static_cast<WaveB>(static_cast<int>(value)));
    else if (id == ParamID::oscBPitch)      { }
    else if (id == ParamID::oscBFine)       { }
    else if (id == ParamID::oscBLevel)      synthEngine.setOscBLevel(value);
    else if (id == ParamID::fmAmount)       synthEngine.setFMAmount(value);
    else if (id == ParamID::subLevel)       synthEngine.setSubLevel(value);
    else if (id == ParamID::noiseLevel)     synthEngine.setNoiseLevel(value);
    else if (id == ParamID::filterType)     synthEngine.setFilterType(static_cast<FilterType>(static_cast<int>(value)));
    else if (id == ParamID::filterCutoff)   synthEngine.setFilterCutoff(value);
    else if (id == ParamID::filterResonance) synthEngine.setFilterResonance(value);
    else if (id == ParamID::filterDrive)    synthEngine.setFilterDrive(value);
    else if (id == ParamID::filterKeyTrack) synthEngine.setFilterKeyTrack(value);
    else if (id == ParamID::filterEnvDepth) synthEngine.setFilterEnvDepth(value);
    else if (id == ParamID::lfo1Rate)       synthEngine.setLFO1Rate(value);
    else if (id == ParamID::lfo1Shape)      synthEngine.setLFO1Shape(static_cast<LFOShape>(static_cast<int>(value)));
    else if (id == ParamID::lfo1Depth)      synthEngine.setLFO1Depth(value);
    else if (id == ParamID::lfo1Dest)       synthEngine.setLFO1Dest(static_cast<ModDestination>(static_cast<int>(value)));
    else if (id == ParamID::lfo2Rate)       synthEngine.setLFO2Rate(value);
    else if (id == ParamID::lfo2Shape)      synthEngine.setLFO2Shape(static_cast<LFOShape>(static_cast<int>(value)));
    else if (id == ParamID::lfo2Depth)      synthEngine.setLFO2Depth(value);
    else if (id == ParamID::lfo2Dest)       synthEngine.setLFO2Dest(static_cast<ModDestination>(static_cast<int>(value)));
    else if (id == ParamID::seqRate)        synthEngine.setSeqRate(value);
    else if (id == ParamID::seqSmooth)      synthEngine.setSeqSmooth(value);
    else if (id == ParamID::seqDest)        synthEngine.setSeqDest(static_cast<ModDestination>(static_cast<int>(value)));
    else if (id == ParamID::seqStep1)       synthEngine.setSeqStep(0, value);
    else if (id == ParamID::seqStep2)       synthEngine.setSeqStep(1, value);
    else if (id == ParamID::seqStep3)       synthEngine.setSeqStep(2, value);
    else if (id == ParamID::seqStep4)       synthEngine.setSeqStep(3, value);
    else if (id == ParamID::seqStep5)       synthEngine.setSeqStep(4, value);
    else if (id == ParamID::seqStep6)       synthEngine.setSeqStep(5, value);
    else if (id == ParamID::seqStep7)       synthEngine.setSeqStep(6, value);
    else if (id == ParamID::seqStep8)       synthEngine.setSeqStep(7, value);
    else if (id == ParamID::distType)       synthEngine.setDistortionType(static_cast<DistortionType>(static_cast<int>(value)));
    else if (id == ParamID::distAmount)     synthEngine.setDistortionAmount(value);
    else if (id == ParamID::distMix)        synthEngine.setDistortionMix(value);
    else if (id == ParamID::xoverLow)       { /* handled with xoverHigh */ }
    else if (id == ParamID::xoverHigh)      { /* handled with xoverLow */ }
    else if (id == ParamID::compThresh)     synthEngine.setCompThreshold(value);
    else if (id == ParamID::compRatio)      synthEngine.setCompRatio(value / 20.0f);
    else if (id == ParamID::widthAmount)    synthEngine.setWidth(value);
    else if (id == ParamID::masterGain)     synthEngine.setMasterGain(value);
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

    if (presetIndex == 0) // Init
    {
        setChoice(ParamID::oscAWave, 1); setVal(ParamID::oscAPitch, 0); setVal(ParamID::oscAFine, 0); setVal(ParamID::oscALevel, 0.8f);
        setChoice(ParamID::oscBWave, 1); setVal(ParamID::oscBPitch, -12); setVal(ParamID::oscBFine, 0); setVal(ParamID::oscBLevel, 0.4f);
        setVal(ParamID::fmAmount, 0.3f); setVal(ParamID::subLevel, 0.5f); setVal(ParamID::noiseLevel, 0.05f);
        setChoice(ParamID::filterType, 0); setVal(ParamID::filterCutoff, 2000); setVal(ParamID::filterResonance, 0.3f);
        setVal(ParamID::filterDrive, 0.2f); setVal(ParamID::filterKeyTrack, 0.3f); setVal(ParamID::filterEnvDepth, 0.7f);
        setVal(ParamID::ampAttack, 1); setVal(ParamID::ampDecay, 200); setVal(ParamID::ampSustain, 0.8f); setVal(ParamID::ampRelease, 300);
        setVal(ParamID::filterAttack, 10); setVal(ParamID::filterDecay, 400); setVal(ParamID::filterSustain, 0.2f); setVal(ParamID::filterRelease, 500);
        setVal(ParamID::lfo1Rate, 4); setChoice(ParamID::lfo1Shape, 0); setVal(ParamID::lfo1Depth, 0.3f); setChoice(ParamID::lfo1Dest, 0);
        setVal(ParamID::lfo2Rate, 0.25f); setChoice(ParamID::lfo2Shape, 3); setVal(ParamID::lfo2Depth, 0.2f); setChoice(ParamID::lfo2Dest, 2);
        setChoice(ParamID::distType, 1); setVal(ParamID::distAmount, 0.4f); setVal(ParamID::distMix, 0.3f);
        setVal(ParamID::masterGain, -6);
    }
    else if (presetIndex == 1) // Reese Bass
    {
        setChoice(ParamID::oscAWave, 1); setVal(ParamID::oscAPitch, 0); setVal(ParamID::oscAFine, 7);
        setChoice(ParamID::oscBWave, 1); setVal(ParamID::oscBPitch, 0); setVal(ParamID::oscBFine, -7);
        setVal(ParamID::fmAmount, 0.1f); setVal(ParamID::subLevel, 0.6f); setVal(ParamID::noiseLevel, 0.0f);
        setChoice(ParamID::filterType, 1); setVal(ParamID::filterCutoff, 800); setVal(ParamID::filterResonance, 0.4f);
        setVal(ParamID::filterDrive, 0.1f); setVal(ParamID::filterKeyTrack, 0.2f); setVal(ParamID::filterEnvDepth, 0.3f);
        setVal(ParamID::ampAttack, 5); setVal(ParamID::ampDecay, 300); setVal(ParamID::ampSustain, 0.9f); setVal(ParamID::ampRelease, 400);
        setVal(ParamID::lfo1Rate, 2); setChoice(ParamID::lfo1Shape, 0); setVal(ParamID::lfo1Depth, 0.6f); setChoice(ParamID::lfo1Dest, 0);
        setChoice(ParamID::distType, 0); setVal(ParamID::distAmount, 0.2f); setVal(ParamID::distMix, 0.15f);
        setVal(ParamID::masterGain, -4);
    }
    else if (presetIndex == 2) // Neuro Growl
    {
        setChoice(ParamID::oscAWave, 0); setVal(ParamID::oscAPitch, 0); setVal(ParamID::oscAFine, 0);
        setChoice(ParamID::oscBWave, 1); setVal(ParamID::oscBPitch, -12); setVal(ParamID::oscBFine, 0);
        setVal(ParamID::fmAmount, 0.7f); setVal(ParamID::subLevel, 0.4f); setVal(ParamID::noiseLevel, 0.1f);
        setChoice(ParamID::filterType, 1); setVal(ParamID::filterCutoff, 1500); setVal(ParamID::filterResonance, 0.5f);
        setVal(ParamID::filterDrive, 0.4f); setVal(ParamID::filterKeyTrack, 0.4f); setVal(ParamID::filterEnvDepth, 0.8f);
        setVal(ParamID::ampAttack, 2); setVal(ParamID::ampDecay, 250); setVal(ParamID::ampSustain, 0.7f); setVal(ParamID::ampRelease, 350);
        setVal(ParamID::filterAttack, 5); setVal(ParamID::filterDecay, 200); setVal(ParamID::filterSustain, 0.1f); setVal(ParamID::filterRelease, 300);
        setVal(ParamID::modAttack, 2); setVal(ParamID::modDecay, 150); setVal(ParamID::modSustain, 0.0f); setVal(ParamID::modRelease, 200);
        setVal(ParamID::modDepth, 0.7f);
        setChoice(ParamID::distType, 1); setVal(ParamID::distAmount, 0.6f); setVal(ParamID::distMix, 0.5f);
        setVal(ParamID::masterGain, -3);
    }
    else if (presetIndex == 3) // Techstep Wub
    {
        setChoice(ParamID::oscAWave, 3); setVal(ParamID::oscAPitch, 0); setVal(ParamID::oscAFine, 0);
        setChoice(ParamID::oscBWave, 0); setVal(ParamID::oscBPitch, 0); setVal(ParamID::oscBFine, 0);
        setVal(ParamID::fmAmount, 0.5f); setVal(ParamID::subLevel, 0.5f); setVal(ParamID::noiseLevel, 0.0f);
        setChoice(ParamID::filterType, 0); setVal(ParamID::filterCutoff, 600); setVal(ParamID::filterResonance, 0.35f);
        setVal(ParamID::filterDrive, 0.15f); setVal(ParamID::filterKeyTrack, 0.3f); setVal(ParamID::filterEnvDepth, 0.4f);
        setVal(ParamID::lfo2Rate, 2.0f); setChoice(ParamID::lfo2Shape, 3); setVal(ParamID::lfo2Depth, 0.8f); setChoice(ParamID::lfo2Dest, 0);
        setVal(ParamID::seqRate, 3); setChoice(ParamID::seqDest, 0);
        setVal(ParamID::seqStep1, 1); setVal(ParamID::seqStep2, 0.1f); setVal(ParamID::seqStep3, 0.8f); setVal(ParamID::seqStep4, 0.1f);
        setVal(ParamID::seqStep5, 0.6f); setVal(ParamID::seqStep6, 0.1f); setVal(ParamID::seqStep7, 0.9f); setVal(ParamID::seqStep8, 0.1f);
        setChoice(ParamID::distType, 0); setVal(ParamID::distAmount, 0.3f); setVal(ParamID::distMix, 0.2f);
        setVal(ParamID::compRatio, 8); setVal(ParamID::compThresh, -18);
        setVal(ParamID::masterGain, -2);
    }
    else if (presetIndex == 4) // Sub Destroyer
    {
        setChoice(ParamID::oscAWave, 3); setVal(ParamID::oscAPitch, -12); setVal(ParamID::oscAFine, 0);
        setChoice(ParamID::oscBWave, 3); setVal(ParamID::oscBPitch, -24); setVal(ParamID::oscBFine, 0);
        setVal(ParamID::fmAmount, 0.0f); setVal(ParamID::subLevel, 1.0f); setVal(ParamID::noiseLevel, 0.0f);
        setChoice(ParamID::filterType, 1); setVal(ParamID::filterCutoff, 400); setVal(ParamID::filterResonance, 0.2f);
        setVal(ParamID::filterDrive, 0.0f); setVal(ParamID::filterKeyTrack, 0.5f); setVal(ParamID::filterEnvDepth, 0.9f);
        setVal(ParamID::ampAttack, 1); setVal(ParamID::ampDecay, 100); setVal(ParamID::ampSustain, 0.95f); setVal(ParamID::ampRelease, 200);
        setVal(ParamID::filterAttack, 1); setVal(ParamID::filterDecay, 150); setVal(ParamID::filterSustain, 0.0f); setVal(ParamID::filterRelease, 250);
        setChoice(ParamID::distType, 0); setVal(ParamID::distAmount, 0.0f); setVal(ParamID::distMix, 0.0f);
        setVal(ParamID::widthAmount, 0.0f);
        setVal(ParamID::masterGain, 0);
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
