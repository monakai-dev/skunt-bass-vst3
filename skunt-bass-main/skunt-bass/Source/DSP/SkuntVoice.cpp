#include "SkuntSound.h"
#include "SkuntVoice.h"
#include <cmath>

SkuntVoice::SkuntVoice()
{
    osc.setWaveA(WaveA::Saw);
    osc.setWaveB(WaveB::Square);
}

bool SkuntVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<SkuntSound*>(sound) != nullptr;
}

void SkuntVoice::startNote(int midiNoteNumber, float velocity,
                           juce::SynthesiserSound*, int /*pitchWheel*/)
{
    currentNote = midiNoteNumber;
    noteVelocity = velocity;

    float freq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    osc.setPitchA(static_cast<int>(pitchASemitones), pitchAFine);
    osc.setPitchB(static_cast<int>(pitchBSemitones), pitchBFine);

    ampEnv.noteOn();
    filterEnv.noteOn();
    modEnv.noteOn();

    currentCutoff = baseCutoff;
    currentFM = fmAmount;
    currentResonance = filterResonance;
    currentPitch = 0.0f;
}

void SkuntVoice::stopNote(float /*velocity*/, bool allowTailOff)
{
    ampEnv.noteOff();
    filterEnv.noteOff();
    modEnv.noteOff();

    if (!allowTailOff)
    {
        ampEnv.reset();
        filterEnv.reset();
        modEnv.reset();
        clearCurrentNote();
    }
}

void SkuntVoice::pitchWheelMoved(int newValue)
{
    float norm = (newValue - 8192) / 8192.0f;
    pitchBendSemitones = norm * 2.0f; // +/- 2 semitones
}

void SkuntVoice::controllerMoved(int, int)
{
}

void SkuntVoice::setAmpEnvelope(float a, float d, float s, float r)
{
    ampEnv.setParameters(a, d, s, r);
}

void SkuntVoice::setFilterEnvelope(float a, float d, float s, float r)
{
    filterEnv.setParameters(a, d, s, r);
}

void SkuntVoice::setModEnvelope(float a, float d, float s, float r)
{
    modEnv.setParameters(a, d, s, r);
}

void SkuntVoice::applyModulation(const VoiceMods& mods)
{
    // Apply LFO1
    switch (lfo1Dest)
    {
        case ModDestination::FilterCutoff:
            currentCutoff = baseCutoff + mods.lfo1 * 5000.0f;
            break;
        case ModDestination::Pitch:
            currentPitch = mods.lfo1 * 12.0f;
            break;
        case ModDestination::FMAmount:
            currentFM = juce::jlimit(0.0f, 1.0f, fmAmount + mods.lfo1 * 0.5f);
            break;
        case ModDestination::Resonance:
            currentResonance = juce::jlimit(0.0f, 1.0f, filterResonance + mods.lfo1 * 0.3f);
            break;
    }

    // Apply LFO2 (additive)
    switch (lfo2Dest)
    {
        case ModDestination::FilterCutoff:
            currentCutoff += mods.lfo2 * 5000.0f;
            break;
        case ModDestination::Pitch:
            currentPitch += mods.lfo2 * 12.0f;
            break;
        case ModDestination::FMAmount:
            currentFM = juce::jlimit(0.0f, 1.0f, currentFM + mods.lfo2 * 0.5f);
            break;
        case ModDestination::Resonance:
            currentResonance = juce::jlimit(0.0f, 1.0f, currentResonance + mods.lfo2 * 0.3f);
            break;
    }

    // Apply sequencer (additive)
    switch (seqDest)
    {
        case ModDestination::FilterCutoff:
            currentCutoff += mods.seq * 8000.0f;
            break;
        case ModDestination::Pitch:
            currentPitch += mods.seq * 12.0f;
            break;
        case ModDestination::FMAmount:
            currentFM = juce::jlimit(0.0f, 1.0f, currentFM + mods.seq * 0.5f);
            break;
        case ModDestination::Resonance:
            currentResonance = juce::jlimit(0.0f, 1.0f, currentResonance + mods.seq * 0.3f);
            break;
    }

    // Apply mod envelope (additive)
    switch (modEnvDest)
    {
        case ModDestination::FilterCutoff:
            currentCutoff += mods.modEnv * 5000.0f;
            break;
        case ModDestination::Pitch:
            currentPitch += mods.modEnv * 12.0f;
            break;
        case ModDestination::FMAmount:
            currentFM = juce::jlimit(0.0f, 1.0f, currentFM + mods.modEnv * 0.5f);
            break;
        case ModDestination::Resonance:
            currentResonance = juce::jlimit(0.0f, 1.0f, currentResonance + mods.modEnv * 0.3f);
            break;
    }

    currentCutoff = juce::jlimit(20.0f, 20000.0f, currentCutoff);

    // Apply key tracking to cutoff
    float keyTrackOffset = (static_cast<float>(currentNote) - 60.0f) * keyTrackAmount * 100.0f;
    currentCutoff = juce::jlimit(20.0f, 20000.0f, currentCutoff + keyTrackOffset);

    // Apply filter envelope
    float filtEnvVal = filterEnv.getOutput();
    float envMod = filtEnvVal * filterEnvDepth * 10000.0f;
    currentCutoff = juce::jlimit(20.0f, 20000.0f, currentCutoff + envMod);

    // Update filter
    filter.setCutoff(currentCutoff);
    filter.setResonance(currentResonance);
}

void SkuntVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer,
                                  int startSample, int numSamples)
{
    if (!ampEnv.isActive())
    {
        clearCurrentNote();
        return;
    }

    auto* outL = outputBuffer.getWritePointer(0);
    auto* outR = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer(1) : nullptr;

    for (int i = 0; i < numSamples; ++i)
    {
        float ampVal = ampEnv.process();
        float modEnvVal = modEnv.process();
        filterEnv.process();

        // Get oscillator output with FM
        osc.setFMInput(osc.getLastOscAOutput());
        float oscSample = osc.process();

        // Filter
        float filtered = filter.process(oscSample);

        // Apply amplitude envelope and velocity
        float sample = filtered * ampVal * noteVelocity;

        // Write to output
        outL[startSample + i] += sample;
        if (outR)
            outR[startSample + i] += sample;
    }
}
