# SKUNT BASS — VST3 Neurofunk Bass Plugin Build Plan

## Overview
Build a VST3 neurofunk bass synthesizer plugin with gothic/camo UI, simplified parameters, and modulators. Named "SKUNT BASS".

## Architecture
- **Framework**: JUCE (C++) — industry standard for VST3/AU/AAX plugins
- **DSP Engine**: Multi-oscillator bass synth with FM, wavetable, and subtractive synthesis
- **Effects Chain**: Distortion, multiband compression, filtering, stereo widening
- **Modulation**: LFOs, envelope followers, step sequencer
- **UI**: Custom Gothic + Camo themed interface with skull motifs, dark palette, military camo accents

## Build Stages

### Stage 1 — Project Scaffold & JUCE Setup
- Create CMake-based JUCE project structure
- Set up VST3 target
- Configure plugin metadata (name, vendor, version)
- Build initial compilation target

### Stage 2 — DSP Core Engine
- Implement multi-oscillator voice architecture
- FM synthesis for growl bass
- Wavetable oscillator for reese bass
- State-variable filter (LP/BP/HP) with drive
- Envelope generators (amp, filter, mod)

### Stage 3 — Modulation System
- LFO (sin, saw, square, S&H, noise) with sync
- Step sequencer for rhythmic modulation
- Envelope follower
- Macro knobs that control multiple parameters

### Stage 4 — Effects Chain
- Multiband distortion (3 bands)
- Compressor/limiter
- Chorus/width processor
- Final output limiter

### Stage 5 — Gothic/Camo UI Design
- Custom LookAndFeel with dark gothic palette
- Camo pattern background elements
- Custom rotary sliders, buttons, combo boxes
- Animated elements (pulsing indicators, meter)
- Parameter layout in logical groups

### Stage 6 — Parameter System & Presets
- AudioProcessorValueTreeState setup
- Parameter smoothing
- Factory presets (Reese, Growl, Wub, Neuro, Techstep)
- State save/load

### Stage 7 — Build & Package
- Compile VST3 for Linux x86_64
- Package with README and installation instructions
- Test audio output

## Skill: vibecoding-general-swarm
