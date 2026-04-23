#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

namespace ParamID
{
    // Oscillator A
    inline const char* oscAWave     = "oscAWave";
    inline const char* oscAPitch    = "oscAPitch";
    inline const char* oscAFine     = "oscAFine";
    inline const char* oscALevel    = "oscALevel";

    // Oscillator B
    inline const char* oscBWave     = "oscBWave";
    inline const char* oscBPitch    = "oscBPitch";
    inline const char* oscBFine     = "oscBFine";
    inline const char* oscBLevel    = "oscBLevel";

    // FM & Sub & Noise
    inline const char* fmAmount     = "fmAmount";
    inline const char* subLevel     = "subLevel";
    inline const char* noiseLevel   = "noiseLevel";

    // Filter
    inline const char* filterType       = "filterType";
    inline const char* filterCutoff     = "filterCutoff";
    inline const char* filterResonance  = "filterResonance";
    inline const char* filterDrive      = "filterDrive";
    inline const char* filterKeyTrack   = "filterKeyTrack";

    // Amp Envelope
    inline const char* ampAttack    = "ampAttack";
    inline const char* ampDecay     = "ampDecay";
    inline const char* ampSustain   = "ampSustain";
    inline const char* ampRelease   = "ampRelease";

    // Filter Envelope
    inline const char* filterAttack     = "filterAttack";
    inline const char* filterDecay      = "filterDecay";
    inline const char* filterSustain    = "filterSustain";
    inline const char* filterRelease    = "filterRelease";
    inline const char* filterEnvDepth   = "filterEnvDepth";

    // LFO 1
    inline const char* lfo1Rate       = "lfo1Rate";
    inline const char* lfo1Shape      = "lfo1Shape";
    inline const char* lfo1Depth      = "lfo1Depth";
    inline const char* lfo1Dest       = "lfo1Dest";

    // LFO 2
    inline const char* lfo2Rate       = "lfo2Rate";
    inline const char* lfo2Shape      = "lfo2Shape";
    inline const char* lfo2Depth      = "lfo2Depth";
    inline const char* lfo2Dest       = "lfo2Dest";

    // Step Sequencer
    inline const char* seqRate    = "seqRate";
    inline const char* seqStep1   = "seqStep1";
    inline const char* seqStep2   = "seqStep2";
    inline const char* seqStep3   = "seqStep3";
    inline const char* seqStep4   = "seqStep4";
    inline const char* seqStep5   = "seqStep5";
    inline const char* seqStep6   = "seqStep6";
    inline const char* seqStep7   = "seqStep7";
    inline const char* seqStep8   = "seqStep8";
    inline const char* seqSmooth  = "seqSmooth";
    inline const char* seqDest    = "seqDest";

    // Mod Envelope
    inline const char* modAttack    = "modAttack";
    inline const char* modDecay     = "modDecay";
    inline const char* modSustain   = "modSustain";
    inline const char* modRelease   = "modRelease";
    inline const char* modDepth     = "modDepth";
    inline const char* modDest      = "modDest";

    // Effects
    inline const char* distType     = "distType";
    inline const char* distAmount   = "distAmount";
    inline const char* distMix      = "distMix";
    inline const char* xoverLow     = "xoverLow";
    inline const char* xoverHigh    = "xoverHigh";
    inline const char* compThresh   = "compThresh";
    inline const char* compRatio    = "compRatio";
    inline const char* widthAmount  = "widthAmount";

    // Master
    inline const char* masterGain   = "masterGain";
}

// Helper to create all parameter layouts
juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
