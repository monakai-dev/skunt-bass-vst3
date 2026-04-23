cmake_minimum_required(VERSION 3.22)
project(SkuntBass VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows: static runtime to avoid VC++ redistributable dependency
if(WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Add JUCE
add_subdirectory(JUCE)

# Plugin target
juce_add_plugin(SkuntBass
    PLUGIN_MANUFACTURER_CODE Sknt
    PLUGIN_CODE SkBa
    FORMATS VST3 Standalone
    PRODUCT_NAME "SKUNT BASS"
    COMPANY_NAME "SKUNT AUDIO"
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_PLUGIN_AFTER_BUILD FALSE
)

# Source files
target_sources(SkuntBass PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp
    Source/ParameterIds.cpp
    Source/DSP/SynthEngine.cpp
    Source/DSP/SkuntVoice.cpp
    Source/DSP/SkuntSound.cpp
    Source/DSP/Oscillator.cpp
    Source/DSP/SkuntFilter.cpp
    Source/DSP/Envelope.cpp
    Source/DSP/LFO.cpp
    Source/DSP/StepSequencer.cpp
    Source/DSP/Distortion.cpp
    Source/DSP/Multiband.cpp
    Source/DSP/Compressor.cpp
    Source/DSP/StereoWidth.cpp
    Source/DSP/Limiter.cpp
    Source/UI/GothicCamoLookAndFeel.cpp
    Source/UI/SkuntKnob.cpp
    Source/UI/SkuntButton.cpp
    Source/UI/SkuntComboBox.cpp
    Source/UI/StepSlider.cpp
    Source/UI/VUMeter.cpp
    Source/UI/AdsrDisplay.cpp
    Source/UI/ScopeComponent.cpp
)

# Include directories
target_include_directories(SkuntBass PRIVATE
    Source
    Source/DSP
    Source/UI
)

# Compile definitions
target_compile_definitions(SkuntBass PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_REPORT_APP_USAGE=0
)

# Platform-specific definitions
if(LINUX)
    target_compile_definitions(SkuntBass PRIVATE
        JUCE_ALSA=0
        JUCE_JACK=0
    )
endif()

if(APPLE)
    target_compile_definitions(SkuntBass PRIVATE
        JUCE_COREGRAPHICS_RENDER_WITH_MULTIPLE_PAINT_CALLS=1
    )
endif()

# Link JUCE modules
target_link_libraries(SkuntBass PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_plugin_client
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_dsp
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
)
