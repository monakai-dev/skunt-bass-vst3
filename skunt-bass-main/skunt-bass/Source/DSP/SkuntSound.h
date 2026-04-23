#pragma once
#include <juce_audio_basics/juce_audio_basics.h>

// Simple SynthesiserSound that always applies
class SkuntSound : public juce::SynthesiserSound
{
public:
    SkuntSound() = default;
    bool appliesToNote(int) override { return true; }
    bool appliesToChannel(int) override { return true; }
};
