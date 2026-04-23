#include "SynthEngine.h"
#include <cmath>

SynthEngine::SynthEngine()
{
    sound = new SkuntSound();
    synth.addSound(sound);
}

void SynthEngine::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate;
    blockSize = static_cast<int>(spec.maximumBlockSize);

    voiceBuffer.setSize(2, blockSize);

    osc.setSampleRate(sampleRate);
    filter.setSampleRate(sampleRate);
    ampEnv.setSampleRate(sampleRate);
    filterEnv.setSampleRate(sampleRate);
    modEnv.setSampleRate(sampleRate);
    lfo1.setSampleRate(sampleRate);
    lfo2.setSampleRate(sampleRate);
    sequencer.setSampleRate(sampleRate);
    distortion.setSampleRate(sampleRate);
    crossover.setSampleRate(sampleRate);
    compressor.setSampleRate(sampleRate);
    stereoWidth.setSampleRate(sampleRate);
    limiter.setSampleRate(sampleRate);

    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* v = getVoice(i))
            v->setSampleRate(sampleRate);
    }

    reset();
}

void SynthEngine::reset()
{
    voiceBuffer.clear();
    lfo1.reset();
    lfo2.reset();
    sequencer.reset();
    distortion.reset();
    crossover.reset();
    compressor.reset();
    stereoWidth.reset();
    limiter.reset();
    outputLevelLeft = 0.0f;
    outputLevelRight = 0.0f;
}

SkuntVoice* SynthEngine::getVoice(int index)
{
    return dynamic_cast<SkuntVoice*>(synth.getVoice(index));
}

void SynthEngine::updateAllVoices()
{
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto* v = getVoice(i))
        {
            v->setFilterCutoff(baseCutoff);
            v->setFilterResonance(filterResonance);
            v->setFilterDrive(filterDrive);
            v->setFilterMix(filterMix);
            v->setFilterKeyTrack(keyTrackAmount);
            v->setFilterEnvDepth(filterEnvDepth);
            v->setSubWaveform(subWaveform);
        }
    }
}


void SynthEngine::renderVoices(juce::AudioBuffer<float>& output, int startSample, int numSamples)
{
    // Clear and prepare temp buffer
    voiceBuffer.clear(startSample, numSamples);

    // Render synth voices
    synth.renderNextBlock(voiceBuffer, midiBuffer, startSample, numSamples);

    // Apply global effects per-sample
    for (int i = startSample; i < startSample + numSamples; ++i)
    {
        float left = voiceBuffer.getSample(0, i);
        float right = voiceBuffer.getNumChannels() > 1 ? voiceBuffer.getSample(1, i) : left;

        processEffects(left, right);

        output.setSample(0, i, left);
        if (output.getNumChannels() > 1 && voiceBuffer.getNumChannels() > 1)
            output.setSample(1, i, right);
    }
}

void SynthEngine::processEffects(float& left, float& right)
{
    // Mono distortion (sum to mono for bass, distort, then back to stereo)
    float mono = (left + right) * 0.5f;
    float distMono = distortion.process(mono);

    // Split to L/R with distortion mix applied
    left = left + (distMono - mono);
    right = right + (distMono - mono);

    // Stereo width
    stereoWidth.process(left, right);

    // Compressor (mono detection, applied to both channels)
    left = compressor.process(left);
    right = compressor.process(right);

    // Master gain
    left *= masterGain;
    right *= masterGain;

    // Limiter
    left = limiter.process(left);
    right = limiter.process(right);

    // Metering with decay
    float absL = std::abs(left);
    float absR = std::abs(right);
    outputLevelLeft = std::max(absL, outputLevelLeft * 0.995f);
    outputLevelRight = std::max(absR, outputLevelRight * 0.995f);
}

// Parameter forwarding methods
void SynthEngine::setOscAWave(WaveA w)
{
    osc.setWaveA(w);
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* v = getVoice(i)) v->setOscAWave(w);
}

void SynthEngine::setOscAPitch(int semis, float fine)
{
    pitchASemitones = semis; pitchAFine = fine;
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* v = getVoice(i)) v->setOscAPitch(semis, fine);
}

void SynthEngine::setOscALevel(float lvl)
{
    oscALevel = lvl;
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* v = getVoice(i)) v->setOscALevel(lvl);
}

void SynthEngine::setOscBWave(WaveB w)
{
    osc.setWaveB(w);
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* v = getVoice(i)) v->setOscBWave(w);
}

void SynthEngine::setOscBPitch(int semis, float fine)
{
    pitchBSemitones = semis; pitchBFine = fine;
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* v = getVoice(i)) v->setOscBPitch(semis, fine);
}

void SynthEngine::setOscBLevel(float lvl)
{
    oscBLevel = lvl;
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* v = getVoice(i)) v->setOscBLevel(lvl);
}

void SynthEngine::setFMAmount(float amt)
{
    fmAmount = amt;
    osc.setFMAmount(amt);
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* v = getVoice(i)) { v->setFMAmount(amt); }
}

void SynthEngine::setSubLevel(float lvl)
{
    subLevel = lvl;
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* v = getVoice(i)) v->setSubLevel(lvl);
}

void SynthEngine::setSubWaveform(SubWave w)
{
    subWaveform = w;
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* v = getVoice(i)) v->setSubWaveform(w);
}

void SynthEngine::setNoiseLevel(float lvl)
{
    noiseLevel = lvl;
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* v = getVoice(i)) v->setNoiseLevel(lvl);
}

void SynthEngine::setFilterType(FilterType t)
{
    filterType = t;
    filter.setType(t);
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* v = getVoice(i)) v->setFilterType(t);
}

void SynthEngine::setFilterCutoff(float freq)
{
    baseCutoff = freq;
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* v = getVoice(i)) v->setFilterCutoff(freq);
}

void SynthEngine::setFilterResonance(float q)
{
    filterResonance = q;
    filter.setResonance(q);
    updateAllVoices();
}

void SynthEngine::setFilterDrive(float d)
{
    filterDrive = d;
    filter.setDrive(d);
    updateAllVoices();
}

void SynthEngine::setFilterMix(float mix)
{
    filterMix = juce::jlimit(0.0f, 1.0f, mix);
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* v = getVoice(i)) v->setFilterMix(mix);
}

void SynthEngine::setFilterKeyTrack(float kt)
{
    keyTrackAmount = kt;
    updateAllVoices();
}

void SynthEngine::setFilterEnvDepth(float d)
{
    filterEnvDepth = d;
    updateAllVoices();
}

void SynthEngine::setAmpEnvelope(float a, float d, float s, float r)
{
    ampAttack = a; ampDecay = d; ampSustain = s; ampRelease = r;
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* v = getVoice(i)) v->setAmpEnvelope(a, d, s, r);
}

void SynthEngine::setFilterEnvelope(float a, float d, float s, float r)
{
    filtAttack = a; filtDecay = d; filtSustain = s; filtRelease = r;
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* v = getVoice(i)) v->setFilterEnvelope(a, d, s, r);
}

void SynthEngine::setModEnvelope(float a, float d, float s, float r)
{
    modAttack = a; modDecay = d; modSustain = s; modRelease = r;
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* v = getVoice(i)) v->setModEnvelope(a, d, s, r);
}

void SynthEngine::setLFO1Dest(ModDestination dest) { lfo1Dest = dest; }
void SynthEngine::setLFO2Dest(ModDestination dest) { lfo2Dest = dest; }
void SynthEngine::setSeqDest(ModDestination dest) { seqDest = dest; }

void SynthEngine::setModEnvParams(float a, float d, float s, float r)
{
    modAttack = a; modDecay = d; modSustain = s; modRelease = r;
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* v = getVoice(i)) v->setModEnvelope(a, d, s, r);
}

void SynthEngine::setModEnvDest(ModDestination dest) { modEnvDest = dest; }

void SynthEngine::setXoverFrequencies(float low, float high)
{
    crossover.setCrossoverFrequencies(low, high);
}
