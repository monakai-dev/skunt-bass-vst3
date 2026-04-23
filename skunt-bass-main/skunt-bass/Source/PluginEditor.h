#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"
#include "UI/GothicCamoLookAndFeel.h"
#include "UI/SkuntKnob.h"
#include "UI/SkuntButton.h"
#include "UI/SkuntComboBox.h"
#include "UI/StepSlider.h"
#include "UI/VUMeter.h"
#include "UI/AdsrDisplay.h"
#include "UI/ScopeComponent.h"

class PluginEditor : public juce::AudioProcessorEditor, public juce::Timer
{
public:
    PluginEditor(PluginProcessor&);
    ~PluginEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    PluginProcessor& processor;
    juce::AudioProcessorValueTreeState& apvts;

    // LookAndFeel
    GothicCamoLookAndFeel customLNF;

    // Parameter attachment helpers
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    using ComboAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    // === OSCILLATOR PANEL ===
    juce::GroupComponent oscPanel;
    SkuntComboBox oscAWaveBox, oscBWaveBox;
    SkuntKnob oscAPitchKnob, oscAFineKnob, oscALevelKnob;
    SkuntKnob oscBPitchKnob, oscBFineKnob, oscBLevelKnob;
    SkuntKnob fmAmountKnob, subLevelKnob, noiseLevelKnob;
    std::unique_ptr<ComboAttachment> oscAWaveAttach, oscBWaveAttach;
    std::unique_ptr<Attachment> oscAPitchAttach, oscAFineAttach, oscALevelAttach;
    std::unique_ptr<Attachment> oscBPitchAttach, oscBFineAttach, oscBLevelAttach;
    std::unique_ptr<Attachment> fmAmountAttach, subLevelAttach, noiseLevelAttach;

    // === FILTER PANEL ===
    juce::GroupComponent filterPanel;
    SkuntComboBox filterTypeBox;
    SkuntKnob filterCutoffKnob, filterResKnob, filterDriveKnob, filterKeyTrackKnob;
    std::unique_ptr<ComboAttachment> filterTypeAttach;
    std::unique_ptr<Attachment> filterCutoffAttach, filterResAttach;
    std::unique_ptr<Attachment> filterDriveAttach, filterKeyTrackAttach;

    // === ENVELOPE PANEL ===
    juce::GroupComponent envPanel;
    SkuntKnob ampAttackKnob, ampDecayKnob, ampSustainKnob, ampReleaseKnob;
    SkuntKnob filtAttackKnob, filtDecayKnob, filtSustainKnob, filtReleaseKnob, filtEnvDepthKnob;
    std::unique_ptr<Attachment> ampAtkAttach, ampDecAttach, ampSusAttach, ampRelAttach;
    std::unique_ptr<Attachment> filtAtkAttach, filtDecAttach, filtSusAttach, filtRelAttach, filtDepthAttach;
    AdsrDisplay ampEnvDisplay, filtEnvDisplay;

    // === MODULATION PANEL ===
    juce::GroupComponent modPanel;
    SkuntComboBox lfo1ShapeBox, lfo1DestBox, lfo2ShapeBox, lfo2DestBox;
    SkuntKnob lfo1RateKnob, lfo1DepthKnob, lfo2RateKnob, lfo2DepthKnob;
    SkuntKnob seqRateKnob, seqSmoothKnob;
    StepSlider seqSliders[8];
    SkuntComboBox seqDestBox;
    std::unique_ptr<ComboAttachment> lfo1ShapeAttach, lfo1DestAttach;
    std::unique_ptr<ComboAttachment> lfo2ShapeAttach, lfo2DestAttach;
    std::unique_ptr<Attachment> lfo1RateAttach, lfo1DepthAttach;
    std::unique_ptr<Attachment> lfo2RateAttach, lfo2DepthAttach;
    std::unique_ptr<Attachment> seqRateAttach, seqSmoothAttach;
    std::unique_ptr<ComboAttachment> seqDestAttach;
    std::unique_ptr<Attachment> seqStepAttach[8];

    // Mod Envelope
    SkuntKnob modAtkKnob, modDecKnob, modSusKnob, modRelKnob, modDepthKnob;
    std::unique_ptr<Attachment> modAtkAttach, modDecAttach, modSusAttach, modRelAttach, modDepthAttach;

    // === EFFECTS PANEL ===
    juce::GroupComponent fxPanel;
    SkuntComboBox distTypeBox;
    SkuntKnob distAmountKnob, distMixKnob;
    SkuntKnob xoverLowKnob, xoverHighKnob;
    SkuntKnob compThreshKnob, compRatioKnob;
    SkuntKnob widthKnob;
    std::unique_ptr<ComboAttachment> distTypeAttach;
    std::unique_ptr<Attachment> distAmountAttach, distMixAttach;
    std::unique_ptr<Attachment> xoverLowAttach, xoverHighAttach;
    std::unique_ptr<Attachment> compThreshAttach, compRatioAttach;
    std::unique_ptr<Attachment> widthAttach;

    // === MASTER SECTION ===
    juce::GroupComponent masterPanel;
    SkuntKnob masterGainKnob;
    std::unique_ptr<Attachment> masterGainAttach;
    VUMeter vuMeter;
    ScopeComponent scope;

    // === TOP BAR ===
    SkuntButton initButton, randomButton;
    SkuntComboBox presetBox;
    juce::Label titleLabel;

    // Layout helpers
    void setupAttachments();
    void layoutOscPanel(int x, int y, int w, int h);
    void layoutFilterPanel(int x, int y, int w, int h);
    void layoutEnvelopePanel(int x, int y, int w, int h);
    void layoutModulationPanel(int x, int y, int w, int h);
    void layoutEffectsPanel(int x, int y, int w, int h);
    void layoutMasterSection(int x, int y, int w, int h);
    void layoutTopBar(int x, int y, int w, int h);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};
