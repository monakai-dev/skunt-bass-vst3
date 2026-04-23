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
    osc.setNoteFrequency(freq);
    osc.setPitchA(static_cast<int>(pitchASemitones), pitchAFine);
    osc.setPitchB(static_cast<int>(pitchBSemitones), pitchBFine);

    osc.reset();
    // NOTE: Do NOT reset filter state here — the TPT filter outputs ~-50 dB
    // for the first ~50 samples after reset, making short bass hits inaudible.
    // Let the filter state persist between notes for continuous response.
    filter.setCutoff(baseCutoff);
    filter.setResonance(filterResonance);

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

void SkuntVoice::setSampleRate(double sr)
{
    currentSampleRate = sr;
    osc.setSampleRate(sr);
    filter.setSampleRate(sr);
    ampEnv.setSampleRate(sr);
    filterEnv.setSampleRate(sr);
    modEnv.setSampleRate(sr);
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

    // Pre-calculate key track offset (doesn't change per sample)
    float keyTrackOffset = 0.0f;
    if (keyTrackAmount > 0.001f && currentNote > 0)
        keyTrackOffset = (currentNote - 48.0f) * keyTrackAmount * 50.0f;

    // Process audio block
    float lastCutoff = -1.0f;  // Track cutoff to avoid redundant coefficient updates

    for (int i = 0; i < numSamples; ++i)
    {
        float ampVal = ampEnv.process();
        filterEnv.process();
        modEnv.process();

        // Apply filter envelope to cutoff
        // Serum-style: envelope adds to base cutoff for transient bite
        float filtEnvVal = filterEnv.getOutput();
        float envMod = filtEnvVal * filterEnvDepth * 6000.0f;
        float cutoff = juce::jlimit(50.0f, 12000.0f, baseCutoff + envMod + keyTrackOffset);

        // Only update filter coefficients when cutoff changes significantly
        // (recalculating biquad coeffs every sample is extremely expensive)
        float cutoffDelta = std::abs(cutoff - lastCutoff);
        if (cutoffDelta > 5.0f || lastCutoff < 0)  // 5 Hz threshold
        {
            filter.setCutoff(cutoff);
            lastCutoff = cutoff;
        }

        // Get oscillator output (Serum-style: returns filtered path + direct sub)
        osc.setFMInput(osc.getLastOscAOutput());
        OscOutput oscOut = osc.process();

        // --- FILTERED PATH: oscA + oscB + noise ---
        // Goes through filter + distortion (harmonics get shaped)
        float filtered = filter.process(oscOut.filtered);

        // --- SERUM-STYLE MIX: blend filtered and unfiltered ---
        // filterMix = 0 → fully dry (unfiltered)
        // filterMix = 1 → fully filtered
        float filteredPath = oscOut.filtered * (1.0f - filterMix) + filtered * filterMix;

        // --- DIRECT PATH: sub (Serum "Direct Out") ---
        // Bypasses filter completely — clean sub always comes through
        float directPath = oscOut.direct;

        // Combine: filtered path + direct sub
        float voiceSample = filteredPath + directPath;

        // Apply amplitude envelope and velocity
        float sample = voiceSample * ampVal * noteVelocity;

        // Write to output
        outL[startSample + i] += sample;
        if (outR)
            outR[startSample + i] += sample;
    }
}
