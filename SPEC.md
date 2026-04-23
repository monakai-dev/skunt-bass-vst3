# SKUNT BASS — VST3 Neurofunk Bass Plugin — Specification

## 1. Overview

**SKUNT BASS** is a VST3 synthesizer plugin designed specifically for neurofunk drum & bass bass sounds. It features a simplified parameter set optimized for quick sound design, powerful modulation capabilities, and a gothic/camo-themed user interface.

**Target Platform**: Linux x86_64 VST3
**Framework**: JUCE 8.0.4
**Build System**: CMake 3.22+

---

## 2. Architecture

```
SKUNT BASS Plugin
├── AudioProcessor (PluginProcessor)
│   ├── Parameter tree (APVTS)
│   ├── Synth engine (Synthesiser)
│   │   └── SkuntVoice (SynthesiserVoice)
│   │       ├── Dual Oscillator
│   │       │   ├── Osc A: Wavetable / Saw / Square / Sine
│   │       │   └── Osc B: Saw / Square / Sine / Noise
│   │       ├── FM Matrix (A→B, B→A, self-mod)
│   │       ├── Mixer (Osc A + Osc B + Sub + Noise)
│   │       ├── Multimode Filter (LP12/LP24/BP/HP)
│   │       ├── Drive (pre-filter saturation)
│   │       └── Amp & Filter Envelopes
│   ├── Modulation Engine
│   │   ├── LFO 1-2 (rate, shape, depth, destination)
│   │   ├── Step Sequencer (8 steps, rate, smooth)
│   │   └── Mod Envelope (attack, decay, depth, destination)
│   ├── Effects Chain
│   │   ├── Distortion (3 modes: Tube, Foldback, Bitcrush)
│   │   ├── Multiband Split (Lo/Mid/Hi crossover)
│   │   ├── Compressor
│   │   └── Stereo Widener
│   └── Output Stage (limiter + master gain)
└── PluginEditor (GUI)
    ├── GothicCamoLookAndFeel
    ├── Oscillator Panel
    ├── Filter Panel
    ├── Envelope Panel
    ├── Modulation Panel
    ├── Effects Panel
    └── Master/Output Panel
```

---

## 3. Parameter Specification (APVTS)

All parameters use `juce::AudioProcessorValueTreeState` with normalized [0,1] ranges where applicable.

### 3.1 Oscillator Section
| Parameter | ID | Range | Default | Description |
|-----------|-----|-------|---------|-------------|
| Osc A Wave | oscAWave | 0-3 (enum) | 0 | 0=Wavetable, 1=Saw, 2=Square, 3=Sine |
| Osc A Pitch | oscAPitch | -24 to +24 semitones | 0 | Coarse pitch |
| Osc A Fine | oscAFine | -100 to +100 cents | 0 | Fine detune |
| Osc A Level | oscALevel | 0-1 | 0.8 | Oscillator A gain |
| Osc B Wave | oscBWave | 0-3 (enum) | 1 | 0=Saw, 1=Square, 2=Sine, 3=Noise |
| Osc B Pitch | oscBPitch | -24 to +24 semitones | -12 | Coarse pitch (sub) |
| Osc B Fine | oscBFine | -100 to +100 cents | 0 | Fine detune |
| Osc B Level | oscBLevel | 0-1 | 0.4 | Oscillator B gain |
| FM Amount | fmAmount | 0-1 | 0.3 | FM depth from A→B |
| Sub Level | subLevel | 0-1 | 0.5 | Sub oscillator level (-1 oct) |
| Noise Level | noiseLevel | 0-1 | 0.05 | White noise level |

### 3.2 Filter Section
| Parameter | ID | Range | Default | Description |
|-----------|-----|-------|---------|-------------|
| Filter Type | filterType | 0-3 (enum) | 0 | 0=LP12, 1=LP24, 2=BP, 3=HP |
| Cutoff | filterCutoff | 20-20000 Hz | 2000 | Filter cutoff frequency |
| Resonance | filterResonance | 0-1 | 0.3 | Filter resonance/Q |
| Drive | filterDrive | 0-1 | 0.2 | Pre-filter saturation drive |
| Filter Key Track | filterKeyTrack | 0-1 | 0.3 | Keyboard tracking amount |

### 3.3 Envelopes
| Parameter | ID | Range | Default | Description |
|-----------|-----|-------|---------|-------------|
| Amp Attack | ampAttack | 0-5000ms | 1 | Amp envelope attack |
| Amp Decay | ampDecay | 0-5000ms | 200 | Amp envelope decay |
| Amp Sustain | ampSustain | 0-1 | 0.8 | Amp envelope sustain |
| Amp Release | ampRelease | 0-5000ms | 300 | Amp envelope release |
| Filter Attack | filterAttack | 0-5000ms | 10 | Filter envelope attack |
| Filter Decay | filterDecay | 0-5000ms | 400 | Filter envelope decay |
| Filter Sustain | filterSustain | 0-1 | 0.2 | Filter envelope sustain |
| Filter Release | filterRelease | 0-5000ms | 500 | Filter envelope release |
| Filter Env Depth | filterEnvDepth | -1 to +1 | 0.7 | Filter envelope modulation amount |

### 3.4 Modulation (LFOs)
| Parameter | ID | Range | Default | Description |
|-----------|-----|-------|---------|-------------|
| LFO1 Rate | lfo1Rate | 0.01-20 Hz | 4 | LFO 1 frequency |
| LFO1 Shape | lfo1Shape | 0-5 (enum) | 0 | 0=Sine,1=Triangle,2=Saw,3=Square,4=S&H,5=Noise |
| LFO1 Depth | lfo1Depth | 0-1 | 0.3 | LFO 1 amount |
| LFO1 Destination | lfo1Dest | 0-3 (enum) | 0 | 0=Cutoff,1=Pitch,2=FM,3=Resonance |
| LFO2 Rate | lfo2Rate | 0.01-20 Hz | 0.25 | LFO 2 frequency |
| LFO2 Shape | lfo2Shape | 0-5 (enum) | 3 | 0=Sine,1=Triangle,2=Saw,3=Square,4=S&H,5=Noise |
| LFO2 Depth | lfo2Depth | 0-1 | 0.2 | LFO 2 amount |
| LFO2 Destination | lfo2Dest | 0-3 (enum) | 2 | 0=Cutoff,1=Pitch,2=FM,3=Resonance |

### 3.5 Step Sequencer
| Parameter | ID | Range | Default | Description |
|-----------|-----|-------|---------|-------------|
| Seq Rate | seqRate | 0.01-10 Hz | 2 | Sequencer rate |
| Seq Steps 1-8 | seqStep1-8 | 0-1 | various | Per-step value (0-1) |
| Seq Smooth | seqSmooth | 0-1 | 0.1 | Step interpolation |
| Seq Destination | seqDest | 0-3 (enum) | 0 | 0=Cutoff,1=Pitch,2=FM,3=Resonance |

### 3.6 Mod Envelope
| Parameter | ID | Range | Default | Description |
|-----------|-----|-------|---------|-------------|
| Mod Attack | modAttack | 0-5000ms | 5 | Mod envelope attack |
| Mod Decay | modDecay | 0-5000ms | 300 | Mod envelope decay |
| Mod Sustain | modSustain | 0-1 | 0 | Mod envelope sustain |
| Mod Release | modRelease | 0-5000ms | 200 | Mod envelope release |
| Mod Depth | modDepth | -1 to +1 | 0.5 | Mod envelope depth |
| Mod Destination | modDest | 0-3 (enum) | 1 | 0=Cutoff,1=Pitch,2=FM,3=Resonance |

### 3.7 Effects
| Parameter | ID | Range | Default | Description |
|-----------|-----|-------|---------|-------------|
| Distortion Type | distType | 0-2 (enum) | 1 | 0=Tube, 1=Foldback, 2=Bitcrush |
| Distortion Amount | distAmount | 0-1 | 0.4 | Distortion drive |
| Distortion Mix | distMix | 0-1 | 0.3 | Dry/wet mix |
| Crossover Low | xoverLow | 50-500 Hz | 150 | Low/Mid crossover |
| Crossover High | xoverHigh | 1000-10000 Hz | 4000 | Mid/Hi crossover |
| Comp Threshold | compThresh | -60 to 0 dB | -20 | Compressor threshold |
| Comp Ratio | compRatio | 1:1 to 20:1 | 4 | Compression ratio |
| Width | widthAmount | 0-1 | 0.3 | Stereo width |
| Master Gain | masterGain | -60 to +12 dB | -6 | Output gain |

---

## 4. DSP Module Specifications

### 4.1 Oscillators

**Oscillator A (Wavetable capable)**
- Waveforms: Interpolated wavetable (64-slot sine-derived), Saw, Square (with pulse width), Sine
- Pitch: ±24 semitones coarse, ±100 cents fine
- Antialiased using polyBLEP for saw/square

**Oscillator B**
- Waveforms: Saw, Square (PWM), Sine, White Noise
- Pitch: ±24 semitones coarse, ±100 cents fine
- Noise uses juce::Random for white noise generation

**FM Matrix**
- Osc A modulates Osc B frequency
- FM amount controls modulation index
- At high FM amounts, produces growl/rasp characteristic of neurofunk

**Sub Oscillator**
- Fixed -1 octave square wave
- Independent level control

**Noise**
- White noise generator
- Lowpass filtered at 8kHz for warmth

### 4.2 Filter

**State Variable Filter (TPT structure)**
- Types: 12dB LP, 24dB LP (cascade), 6dB BP, 12dB HP
- Drive: Pre-filter saturation using tanh() waveshaper
- Resonance: Q control, self-oscillation at high values
- Key tracking: 0-100% tracking of MIDI note number

### 4.3 Envelopes

**ADSR with exponential curves**
- Attack: exponential rise (0-5s)
- Decay: exponential fall (0-5s)
- Sustain: held level (0-1)
- Release: exponential fall (0-5s)
- All segments use per-sample exponential smoothing for click-free transitions

### 4.4 LFO

- Rates: 0.01-20 Hz
- Shapes: Sine, Triangle, Saw (up), Square, Sample & Hold, Smoothed Noise
- All LFOs are free-running (not retriggered per voice) for evolving bass textures
- Output is bipolar [-1, 1]

### 4.5 Step Sequencer

- 8 steps with individual level per step
- Rate: 0.01-10 Hz (free running)
- Smooth: interpolates between steps (0=steppy, 1=smooth glide)
- Output is unipolar [0, 1], can be scaled by destination

### 4.6 Effects

**Distortion (3 modes)**
- Tube: Soft clipping using tanh() waveshaper, warm and musical
- Foldback: Wavefolding using abs() reflection, aggressive neurofunk character
- Bitcrush: Sample rate + bit depth reduction, digital destruction

**Multiband Split**
- 12dB LR4 crossover filters (Linkwitz-Riley)
- Separate Lo/Mid/Hi bands for processing flexibility

**Compressor**
- Feedforward RMS detector
- Threshold, ratio, fast attack (2ms), program-dependent release
- Makeup gain auto-computed

**Stereo Widener**
- M/S processing on mid band
- Width: 0=mono, 1=full stereo
- Mono-compatible bass handling (lows summed to mono below 120Hz)

**Output Limiter**
- Soft brickwall at 0dBFS
- 10ms lookahead via delay line

---

## 5. UI Specification

### 5.1 Visual Style: Gothic + Camo

**Color Palette**
```
Background Primary:   #0D0D0D  (near-black)
Background Secondary: #1A1A1A  (dark grey panels)
Background Tertiary:  #141414  (panel headers)
Accent Primary:       #2D5016  (camo green)
Accent Secondary:     #4A7C2E  (lighter camo green)
Accent Danger:        #8B0000  (dark blood red - gothic)
Accent Gold:          #8B7355  (muted gold/brown)
Text Primary:         #E8E0D4  (warm off-white)
Text Secondary:       #8A8578  (muted grey)
Knob Track:           #2D5016  (camo green)
Knob Fill:            #8B0000  (blood red indicator)
```

**Typography**
- Primary font: "Bebas Neue" or similar condensed sans-serif (loaded via BinaryData)
- Labels: 12-14px condensed
- Section headers: 18-22px bold
- Plugin title: 32px gothic style with subtle glow effect

**Camo Pattern**
- Subtle background pattern with irregular camo shapes in `#1A1A1A`, `#1F2F15`, `#2D5016` tones
- Very low opacity (15-20%) so it doesn't interfere with readability
- Applied as a background fill on main component

### 5.2 Layout

```
+------------------------------------------------------------------+
|  SKUNT BASS                                    [INIT] [RANDOM]   |
|  =========                                    [PRESET]           |
+--------------+--------------+--------------+---------------------+
| OSCILLATORS  |   FILTER     | ENVELOPES    |    MODULATION       |
|              |              |              |                     |
| [A] Wave     | Type [LP12]  | AMP          | LFO1 [Sine→Cut]    |
| Pitch  Fine  | Cutoff       |  A   D  S  R | Rate    Depth       |
| Level        | Resonance    |              |                     |
| [B] Wave     | Drive        | FILTER       | LFO2 [Sqr→FM]      |
| Pitch  Fine  | Key Track    |  A   D  S  R | Rate    Depth       |
| Level        |              | EnvDepth     |                     |
| FM Amount    |              |              | STEP SEQ → Cutoff  |
| Sub  Noise   |              |              | [8 step sliders]   |
+--------------+--------------+--------------+---------------------+
|   EFFECTS (Distortion / Multiband / Compressor / Width)         |
|   Type  Amount  Mix | Xover-Low  Xover-High | Thresh  Ratio      |
+------------------------------------------------------------------+
|                         MASTER GAIN [______]  [VU METER]         |
+------------------------------------------------------------------+
```

**Panel sizes (approximate, 1000x700 total)**
- Title bar: 1000x60
- Oscillators: 250x280
- Filter: 250x280
- Envelopes: 250x280
- Modulation: 250x280
- Effects: 1000x180
- Master: 1000x80

### 5.3 Custom Components

**SkuntKnob**
- Rotary dial with conical gradient fill
- Arc indicator: blood red (`#8B0000`) for value, dark green for track
- Outer ring: thin stroke with glow when hovered
- Size: 60x60px typical
- Value display in center (popup on drag)

**SkuntButton**
- Rectangular with 2px border
- Normal: dark fill, muted text
- Active: camo green fill with slight inner glow
- LED indicator dot for toggle buttons

**SkuntComboBox**
- Custom dropdown with dark styling
- Chevron arrow indicator
- Hover highlight in camo green

**StepSlider**
- Vertical fader for step sequencer
- 16x60px per step
- Blood red fill from bottom
- Current step highlight with gold accent

**VUMeter**
- Vertical LED strip
- Green (low) → Yellow (mid) → Red (peaking)
- 20-segment display
- Peak hold indicator

### 5.4 Preset Controls
- Init button: resets all parameters to default
- Random button: randomizes key parameters for inspiration
- Preset dropdown: selects from factory presets

---

## 6. Factory Presets

### 6.1 Reese Bass
- Saw + Saw, detuned 7 cents
- LP24 filter, cutoff 800Hz, resonance 0.4
- LFO1 → Filter cutoff, 2Hz sine, depth 0.6
- Sub level 0.6, slight tube distortion

### 6.2 Neuro Growl
- Wavetable + Square, FM amount 0.7
- LP24 filter with aggressive envelope
- Mod Env → FM amount, snappy attack/decay
- Foldback distortion, medium mix

### 6.3 Techstep Wub
- Sine + Saw, FM 0.5
- LP12 filter, cutoff modulated by LFO2 (square, 1/8 rate)
- Step sequencer → cutoff for rhythmic wubs
- Heavy compression

### 6.4 Digital Rage
- Wavetable + Noise
- BP filter, high resonance
- LFO1 → Resonance, S&H shape
- Bitcrush distortion, wide stereo

### 6.5 Sub Destroyer
- Sine + Sine (sub octave)
- LP24, cutoff follows envelope tightly
- Minimal effects, pure sub bass
- High output, limiter engaged

---

## 7. File Structure

```
skunt-bass/
├── CMakeLists.txt                    # Main CMake config
├── JUCE/                             # JUCE framework (8.0.4)
├── assets/
│   └── fonts/
│       └── BebasNeue-Regular.ttf     # UI font (to be embedded)
├── Source/
│   ├── PluginProcessor.h             # Main processor class
│   ├── PluginProcessor.cpp           # Processor implementation
│   ├── PluginEditor.h                # Main editor class
│   ├── PluginEditor.cpp              # Editor implementation + layout
│   ├── ParameterIds.h                # String literal parameter IDs
│   ├── DSP/
│   │   ├── SynthEngine.h/.cpp        # Synthesiser subclass
│   │   ├── SkuntVoice.h/.cpp         # Voice (SynthesiserVoice)
│   │   ├── SkuntSound.h/.cpp         # Sound (SynthesiserSound)
│   │   ├── Oscillator.h/.cpp         # Dual oscillator + FM + sub
│   │   ├── SkuntFilter.h/.cpp        # Multimode SVF
│   │   ├── Envelope.h/.cpp           # ADSR envelope
│   │   ├── LFO.h/.cpp                # LFO with multiple shapes
│   │   ├── StepSequencer.h/.cpp      # 8-step sequencer
│   │   ├── Distortion.h/.cpp         # 3-mode distortion
│   │   ├── Multiband.h/.cpp          # Crossover + MB processing
│   │   ├── Compressor.h/.cpp         # RMS compressor
│   │   ├── StereoWidth.h/.cpp        # M/S width processor
│   │   └── Limiter.h/.cpp            # Brickwall limiter
│   └── UI/
│       ├── GothicCamoLookAndFeel.h/.cpp  # Custom LnF
│       ├── SkuntKnob.h/.cpp              # Rotary knob component
│       ├── SkuntButton.h/.cpp            # Button component
│       ├── SkuntComboBox.h/.cpp          # Dropdown component
│       ├── StepSlider.h/.cpp             # Step fader component
│       ├── VUMeter.h/.cpp                # Level meter component
│       ├── AdsrDisplay.h/.cpp            # ADSR visualization
│       └── ScopeComponent.h/.cpp         # Small oscilloscope
└── README.md
```

---

## 8. Build Configuration

```cmake
# Minimum CMake version
cmake_minimum_required(VERSION 3.22)
project(SkuntBass VERSION 1.0.0)

# JUCE setup
add_subdirectory(JUCE)

juce_add_plugin(SkuntBass
    PLUGIN_MANUFACTURER_CODE Sknt
    PLUGIN_CODE Sbass
    FORMATS VST3 Standalone
    PRODUCT_NAME "SKUNT BASS"
    COMPANY_NAME "SKUNT AUDIO"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
)

target_compile_features(SkuntBass PRIVATE cxx_std_17)
target_link_libraries(SkuntBass PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_gui_extra
)

juce_generate_juce_header(SkuntBass)
```

---

## 9. Performance Targets

- Maximum 10% CPU at 48kHz / 256 samples buffer with 8 voices
- No allocations in audio thread (real-time safe)
- Parameter changes smoothed with 5ms ramp
- MIDI latency: < 1ms

---

## 10. Notes

- The plugin operates as a monophonic or polyphonic synth (max 8 voices)
- All modulation sources are summed before application to destinations
- The modulation matrix is hard-wired (simplified for ease of use)
- Pitch bend range: ±2 semitones
- Mod wheel controls LFO1 depth (MIDI CC#1)
- No external preset file format — presets embedded as binary data
