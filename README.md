# SKUNT BASS v1.0.0

**Neurofunk Drum & Bass Synthesizer VST3 Plugin**

A hard-hitting bass synthesizer designed specifically for neurofunk, techstep, and dark DnB production. Built with JUCE 8 and a gothic/camo military aesthetic.

![SKUNT BASS Logo]

---

## Features

### Sound Engine
- **Dual Oscillator Architecture** - Osc A: Wavetable/Saw/Square/Sine | Osc B: Saw/Square/Sine/Noise
- **FM Synthesis** - Osc A modulates Osc B for aggressive growl bass tones
- **Sub Oscillator** - Dedicated -1 octave square wave for sub weight
- **Multimode Filter** - LP12, LP24, Bandpass, Highpass with drive and key tracking
- **3 Envelope Generators** - Amp ADSR, Filter ADSR, Mod ADSR

### Modulation
- **2 Free-Running LFOs** - Sine, Triangle, Saw, Square, S&H, Noise shapes
- **8-Step Sequencer** - Rhythmic modulation with smoothing
- **Hard-Wired Mod Matrix** - Cutoff, Pitch, FM, Resonance destinations

### Effects Chain
- **3-Mode Distortion** - Tube, Foldback, Bitcrush
- **Multiband Crossover** - Lo/Mid/Hi band splitting
- **RMS Compressor** - Punch and control
- **Stereo Widener** - M/S processing with mono bass protection
- **Brickwall Limiter** - Output protection

### Factory Presets
1. **Init** - Clean starting point
2. **Reese Bass** - Classic detuned reese with LFO filter sweep
3. **Neuro Growl** - Aggressive FM growl with foldback distortion
4. **Techstep Wub** - Rhythmic wub bass with step sequencer
5. **Sub Destroyer** - Pure sub bass for low-end destruction

## Installation

### Windows

```powershell
# Extract the ZIP
Expand-Archive SKUNT_BASS_Windows_x64.zip -DestinationPath .\

# Install to user VST3 directory
$vst3User = "$env:USERPROFILE\AppData\Local\Programs\Common\VST3"
New-Item -ItemType Directory -Path $vst3User -Force
Copy-Item -Path "SKUNT BASS.vst3" -Destination $vst3User -Recurse -Force

# Or install system-wide (requires admin)
# Copy-Item -Path "SKUNT BASS.vst3" -Destination "C:\Program Files\Common Files\VST3\" -Recurse -Force
```

### Linux

```bash
# Extract the VST3 bundle
tar xzf SKUNT_BASS_Linux_x64.tar.gz

# Install to user VST3 directory
mkdir -p ~/.vst3
cp -r "SKUNT BASS.vst3" ~/.vst3/

# Or install system-wide (requires root)
sudo cp -r "SKUNT BASS.vst3" /usr/lib/vst3/
```

### macOS

```bash
# Extract the ZIP
unzip SKUNT_BASS_macOS_universal.zip

# Install to user VST3 directory
mkdir -p ~/Library/Audio/Plug-Ins/VST3
cp -r "SKUNT BASS.vst3" ~/Library/Audio/Plug-Ins/VST3/

# Or install system-wide (requires admin)
sudo cp -r "SKUNT BASS.vst3" /Library/Audio/Plug-Ins/VST3/
```

## System Requirements

| Platform | Minimum Version | Architecture |
|----------|-----------------|--------------|
| Windows  | Windows 10 1903+ | x64 |
| Linux    | Ubuntu 20.04, Debian 11, Fedora 35 | x86_64 |
| macOS    | macOS 11 Big Sur+ | Intel / Apple Silicon |

- **CPU**: SSE2 support (Intel/AMD)
- **RAM**: 4GB minimum
- **Host**: Any VST3-compatible DAW (FL Studio, Ableton Live, Bitwig, Reaper, Cubase, etc.)

## Building from Source

### Windows

Install [Visual Studio 2022](https://visualstudio.microsoft.com/) with **"Desktop development with C++"** workload, then:

```powershell
cd skunt-bass
.\build_windows.bat
```

Or manually:

```powershell
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release
cmake --build . --target SkuntBass_VST3 --config Release --parallel
```

### Linux

```bash
# Install dependencies (Debian/Ubuntu)
sudo apt-get install build-essential cmake libfreetype6-dev \
    libfontconfig1-dev libx11-dev libxcomposite-dev libxcursor-dev \
    libxext-dev libxinerama-dev libxrandr-dev libxrender-dev \
    libglu1-mesa-dev mesa-common-dev

# Clone and build
git clone <repo>
cd skunt-bass
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --target SkuntBass_VST3 -j$(nproc)
```

### macOS

```bash
brew install cmake
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64"
cmake --build . --target SkuntBass_VST3 --config Release --parallel $(sysctl -n hw.ncpu)
```

## CI / Automated Builds

This project includes a GitHub Actions workflow (`.github/workflows/build.yml`) that automatically builds for **Windows, Linux, and macOS** on every push and creates release artifacts for tagged versions.

To use it:
1. Push this repo to GitHub
2. Go to **Actions → Build SKUNT BASS VST3**
3. Download the artifact for your platform
4. Or create a release tag (`v1.0.0`) and assets upload automatically

## Parameter Reference

| Section | Parameters | Range |
|---------|-----------|-------|
| Oscillators | Wave, Pitch, Fine, Level, FM, Sub, Noise | Various |
| Filter | Type, Cutoff, Resonance, Drive, KeyTrack | 20Hz-20kHz |
| Envelopes | Attack, Decay, Sustain, Release | 0-5000ms |
| LFOs | Rate, Shape, Depth, Destination | 0.01-20Hz |
| Step Seq | Rate, 8 Steps, Smooth, Destination | 0-1 |
| Effects | Distortion, Crossover, Compressor, Width | Various |
| Master | Gain | -60 to +12dB |

## License

Copyright (c) 2024 SKUNT AUDIO. All rights reserved.

## Credits

Built with [JUCE](https://juce.com) framework.
