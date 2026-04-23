#include "PluginEditor.h"
#include "PluginProcessor.h"
#include "ParameterIds.h"

PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p), processor(p), apvts(p.getAPVTS()),
      oscPanel("oscGroup", "OSCILLATORS"),
      filterPanel("filtGroup", "FILTER"),
      envPanel("envGroup", "ENVELOPES"),
      modPanel("modGroup", "MODULATION"),
      fxPanel("fxGroup", "EFFECTS"),
      masterPanel("masterGroup", "MASTER")
{
    setLookAndFeel(&customLNF);

    // ===== OSCILLATOR PANEL =====
    oscAWaveBox.addItemList({"Wavetable", "Saw", "Square", "Sine"}, 1);
    oscBWaveBox.addItemList({"Saw", "Square", "Sine", "Noise"}, 1);

    oscAPitchKnob.setLabel("Pitch A"); oscAPitchKnob.setSuffix(""); oscAPitchKnob.setRealRange(-20.0f, 20.0f); oscAPitchKnob.setDisplayDecimals(0);
    oscAFineKnob.setLabel("Fine A"); oscAFineKnob.setSuffix(""); oscAFineKnob.setRealRange(-100.0f, 100.0f); oscAFineKnob.setDisplayDecimals(0);
    oscALevelKnob.setLabel("Level A"); oscALevelKnob.setSuffix(""); oscALevelKnob.setRealRange(0.0f, 1.0f); oscALevelKnob.setDisplayDecimals(2);
    oscBPitchKnob.setLabel("Pitch B"); oscBPitchKnob.setSuffix(""); oscBPitchKnob.setRealRange(-20.0f, 20.0f); oscBPitchKnob.setDisplayDecimals(0);
    oscBFineKnob.setLabel("Fine B"); oscBFineKnob.setSuffix(""); oscBFineKnob.setRealRange(-100.0f, 100.0f); oscBFineKnob.setDisplayDecimals(0);
    oscBLevelKnob.setLabel("Level B"); oscBLevelKnob.setSuffix(""); oscBLevelKnob.setRealRange(0.0f, 1.0f); oscBLevelKnob.setDisplayDecimals(2);
    fmAmountKnob.setLabel("FM"); fmAmountKnob.setSuffix(""); fmAmountKnob.setRealRange(0.0f, 1.0f); fmAmountKnob.setDisplayDecimals(2);
    subLevelKnob.setLabel("Sub"); subLevelKnob.setSuffix(""); subLevelKnob.setRealRange(0.0f, 1.0f); subLevelKnob.setDisplayDecimals(2);
    noiseLevelKnob.setLabel("Noise"); noiseLevelKnob.setSuffix(""); noiseLevelKnob.setRealRange(0.0f, 1.0f); noiseLevelKnob.setDisplayDecimals(2);
    subWaveBox.addItemList({"Sine", "Square", "Saw", "Triangle"}, 1);

    addAndMakeVisible(oscPanel);
    addAndMakeVisible(oscAWaveBox); addAndMakeVisible(oscBWaveBox);
    addAndMakeVisible(oscAPitchKnob); addAndMakeVisible(oscAFineKnob); addAndMakeVisible(oscALevelKnob);
    addAndMakeVisible(oscBPitchKnob); addAndMakeVisible(oscBFineKnob); addAndMakeVisible(oscBLevelKnob);
    addAndMakeVisible(fmAmountKnob); addAndMakeVisible(subLevelKnob); addAndMakeVisible(subWaveBox); addAndMakeVisible(noiseLevelKnob);

    // ===== FILTER PANEL =====
    filterTypeBox.addItemList({"LP12", "LP24", "BP6", "HP12"}, 1);
    filterCutoffKnob.setLabel("Cutoff"); filterCutoffKnob.setSuffix(" Hz"); filterCutoffKnob.setRealRange(40.0f, 10000.0f); filterCutoffKnob.setDisplayDecimals(0);
    filterResKnob.setLabel("Res"); filterResKnob.setSuffix(""); filterResKnob.setRealRange(0.0f, 1.0f); filterResKnob.setDisplayDecimals(2);
    filterDriveKnob.setLabel("Drive"); filterDriveKnob.setSuffix(""); filterDriveKnob.setRealRange(0.0f, 1.0f); filterDriveKnob.setDisplayDecimals(2);
    filterMixKnob.setLabel("Filter Mix"); filterMixKnob.setSuffix(""); filterMixKnob.setRealRange(0.0f, 1.0f); filterMixKnob.setDisplayDecimals(2);
    filterKeyTrackKnob.setLabel("KeyTrk"); filterKeyTrackKnob.setSuffix(""); filterKeyTrackKnob.setRealRange(0.0f, 1.0f); filterKeyTrackKnob.setDisplayDecimals(2);

    addAndMakeVisible(filterPanel);
    addAndMakeVisible(filterTypeBox);
    addAndMakeVisible(filterCutoffKnob); addAndMakeVisible(filterResKnob);
    addAndMakeVisible(filterDriveKnob); addAndMakeVisible(filterMixKnob); addAndMakeVisible(filterKeyTrackKnob);

    // ===== ENVELOPE PANEL =====
    ampAttackKnob.setLabel("A.Att"); ampAttackKnob.setSuffix(""); ampAttackKnob.setRealRange(0.1f, 10000.0f); ampAttackKnob.setDisplayDecimals(1);
    ampDecayKnob.setLabel("A.Dec"); ampDecayKnob.setSuffix(""); ampDecayKnob.setRealRange(1.0f, 10000.0f); ampDecayKnob.setDisplayDecimals(1);
    ampSustainKnob.setLabel("A.Sus"); ampSustainKnob.setSuffix(""); ampSustainKnob.setRealRange(0.0f, 1.0f); ampSustainKnob.setDisplayDecimals(2);
    ampReleaseKnob.setLabel("A.Rel"); ampReleaseKnob.setSuffix(""); ampReleaseKnob.setRealRange(1.0f, 10000.0f); ampReleaseKnob.setDisplayDecimals(1);
    filtAttackKnob.setLabel("F.Att"); filtAttackKnob.setSuffix(""); filtAttackKnob.setRealRange(0.1f, 10000.0f); filtAttackKnob.setDisplayDecimals(1);
    filtDecayKnob.setLabel("F.Dec"); filtDecayKnob.setSuffix(""); filtDecayKnob.setRealRange(1.0f, 10000.0f); filtDecayKnob.setDisplayDecimals(1);
    filtSustainKnob.setLabel("F.Sus"); filtSustainKnob.setSuffix(""); filtSustainKnob.setRealRange(0.0f, 1.0f); filtSustainKnob.setDisplayDecimals(2);
    filtReleaseKnob.setLabel("F.Rel"); filtReleaseKnob.setSuffix(""); filtReleaseKnob.setRealRange(1.0f, 10000.0f); filtReleaseKnob.setDisplayDecimals(1);
    filtEnvDepthKnob.setLabel("F.Depth"); filtEnvDepthKnob.setSuffix(""); filtEnvDepthKnob.setRealRange(0.0f, 1.0f); filtEnvDepthKnob.setDisplayDecimals(2);

    addAndMakeVisible(envPanel);
    addAndMakeVisible(ampAttackKnob); addAndMakeVisible(ampDecayKnob);
    addAndMakeVisible(ampSustainKnob); addAndMakeVisible(ampReleaseKnob);
    addAndMakeVisible(filtAttackKnob); addAndMakeVisible(filtDecayKnob);
    addAndMakeVisible(filtSustainKnob); addAndMakeVisible(filtReleaseKnob);
    addAndMakeVisible(filtEnvDepthKnob);
    addAndMakeVisible(ampEnvDisplay); addAndMakeVisible(filtEnvDisplay);

    // ===== MODULATION PANEL =====
    lfo1ShapeBox.addItemList({"Sine", "Triangle", "Saw", "Square", "Random"}, 1);
    lfo1DestBox.addItemList({"Filter Cutoff", "Osc Pitch", "Osc Level", "Pan", "Resonance"}, 1);
    lfo2ShapeBox.addItemList({"Sine", "Triangle", "Saw", "Square", "Random"}, 1);
    lfo2DestBox.addItemList({"Filter Cutoff", "Osc Pitch", "Osc Level", "Pan", "Resonance"}, 1);
    seqDestBox.addItemList({"Filter Cutoff", "Osc Pitch", "Osc Level", "Pan", "Resonance"}, 1);

    lfo1RateKnob.setLabel("LFO1 Rate"); lfo1RateKnob.setSuffix(""); lfo1RateKnob.setRealRange(0.01f, 100.0f); lfo1RateKnob.setDisplayDecimals(2);
    lfo1DepthKnob.setLabel("LFO1 Dpt"); lfo1DepthKnob.setSuffix(""); lfo1DepthKnob.setRealRange(0.0f, 1.0f); lfo1DepthKnob.setDisplayDecimals(2);
    lfo2RateKnob.setLabel("LFO2 Rate"); lfo2RateKnob.setSuffix(""); lfo2RateKnob.setRealRange(0.01f, 100.0f); lfo2RateKnob.setDisplayDecimals(2);
    lfo2DepthKnob.setLabel("LFO2 Dpt"); lfo2DepthKnob.setSuffix(""); lfo2DepthKnob.setRealRange(0.0f, 1.0f); lfo2DepthKnob.setDisplayDecimals(2);
    seqRateKnob.setLabel("Seq Rate"); seqRateKnob.setSuffix(""); seqRateKnob.setRealRange(0.1f, 50.0f); seqRateKnob.setDisplayDecimals(1);
    seqSmoothKnob.setLabel("Smooth"); seqSmoothKnob.setSuffix(""); seqSmoothKnob.setRealRange(0.0f, 1.0f); seqSmoothKnob.setDisplayDecimals(2);

    addAndMakeVisible(modPanel);
    addAndMakeVisible(lfo1ShapeBox); addAndMakeVisible(lfo1DestBox);
    addAndMakeVisible(lfo1RateKnob); addAndMakeVisible(lfo1DepthKnob);
    addAndMakeVisible(lfo2ShapeBox); addAndMakeVisible(lfo2DestBox);
    addAndMakeVisible(lfo2RateKnob); addAndMakeVisible(lfo2DepthKnob);
    addAndMakeVisible(seqRateKnob); addAndMakeVisible(seqSmoothKnob);
    addAndMakeVisible(seqDestBox);
    for (int i = 0; i < 8; ++i)
    {
        addAndMakeVisible(seqSliders[i]);
    }

    // Mod Envelope
    modAtkKnob.setLabel("M.Att"); modAtkKnob.setSuffix(""); modAtkKnob.setRealRange(0.1f, 10000.0f); modAtkKnob.setDisplayDecimals(1);
    modDecKnob.setLabel("M.Dec"); modDecKnob.setSuffix(""); modDecKnob.setRealRange(1.0f, 10000.0f); modDecKnob.setDisplayDecimals(1);
    modSusKnob.setLabel("M.Sus"); modSusKnob.setSuffix(""); modSusKnob.setRealRange(0.0f, 1.0f); modSusKnob.setDisplayDecimals(2);
    modRelKnob.setLabel("M.Rel"); modRelKnob.setSuffix(""); modRelKnob.setRealRange(1.0f, 10000.0f); modRelKnob.setDisplayDecimals(1);
    modDepthKnob.setLabel("M.Depth"); modDepthKnob.setSuffix(""); modDepthKnob.setRealRange(0.0f, 1.0f); modDepthKnob.setDisplayDecimals(2);
    addAndMakeVisible(modAtkKnob); addAndMakeVisible(modDecKnob);
    addAndMakeVisible(modSusKnob); addAndMakeVisible(modRelKnob);
    addAndMakeVisible(modDepthKnob);

    // ===== EFFECTS PANEL =====
    distTypeBox.addItemList({"Tube", "Foldback", "Bitcrush"}, 1);
    distAmountKnob.setLabel("Dist Amt"); distAmountKnob.setSuffix(""); distAmountKnob.setRealRange(0.0f, 1.0f); distAmountKnob.setDisplayDecimals(2);
    distMixKnob.setLabel("Dist Mix"); distMixKnob.setSuffix(""); distMixKnob.setRealRange(0.0f, 1.0f); distMixKnob.setDisplayDecimals(2);
    xoverLowKnob.setLabel("Xover Lo"); xoverLowKnob.setSuffix(""); xoverLowKnob.setRealRange(20.0f, 2000.0f); xoverLowKnob.setDisplayDecimals(0);
    xoverHighKnob.setLabel("Xover Hi"); xoverHighKnob.setSuffix(""); xoverHighKnob.setRealRange(200.0f, 8000.0f); xoverHighKnob.setDisplayDecimals(0);
    compThreshKnob.setLabel("Thresh"); compThreshKnob.setSuffix(""); compThreshKnob.setRealRange(-60.0f, 0.0f); compThreshKnob.setDisplayDecimals(1);
    compRatioKnob.setLabel("Ratio"); compRatioKnob.setSuffix(""); compRatioKnob.setRealRange(1.0f, 20.0f); compRatioKnob.setDisplayDecimals(1);
    widthKnob.setLabel("Width"); widthKnob.setSuffix(""); widthKnob.setRealRange(0.0f, 2.0f); widthKnob.setDisplayDecimals(2);

    addAndMakeVisible(fxPanel);
    addAndMakeVisible(distTypeBox);
    addAndMakeVisible(distAmountKnob); addAndMakeVisible(distMixKnob);
    addAndMakeVisible(xoverLowKnob); addAndMakeVisible(xoverHighKnob);
    addAndMakeVisible(compThreshKnob); addAndMakeVisible(compRatioKnob);
    addAndMakeVisible(widthKnob);

    // ===== MASTER PANEL =====
    masterGainKnob.setLabel("Gain"); masterGainKnob.setSuffix(" dB"); masterGainKnob.setRealRange(-60.0f, 12.0f); masterGainKnob.setDisplayDecimals(0);
    addAndMakeVisible(masterPanel);
    addAndMakeVisible(masterGainKnob);
    addAndMakeVisible(vuMeter);
    addAndMakeVisible(scope);

    // ===== TOP BAR =====
    titleLabel.setText("SKUNT BASS", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(juce::FontOptions(28.0f).withStyle("Bold")));
    titleLabel.setColour(juce::Label::textColourId, juce::Colour(0xFFE8E0D4));
    addAndMakeVisible(titleLabel);

    initButton.setButtonText("INIT");
    addAndMakeVisible(initButton);
    initButton.onClick = [this]() { processor.setCurrentProgram(0); };

    randomButton.setButtonText("RND");
    addAndMakeVisible(randomButton);

    presetBox.addItemList({"Init", "Reese Bass", "Neuro Growl", "Techstep Wub", "Sub Destroyer"}, 1);
    addAndMakeVisible(presetBox);
    presetBox.onChange = [this]() { processor.setCurrentProgram(presetBox.getSelectedItemIndex()); };

    // ===== ATTACHMENTS =====
    oscAWaveAttach = std::make_unique<ComboAttachment>(apvts, ParamID::oscAWave, oscAWaveBox);
    oscAPitchAttach = std::make_unique<Attachment>(apvts, ParamID::oscAPitch, oscAPitchKnob);
    oscAFineAttach = std::make_unique<Attachment>(apvts, ParamID::oscAFine, oscAFineKnob);
    oscALevelAttach = std::make_unique<Attachment>(apvts, ParamID::oscALevel, oscALevelKnob);
    oscBWaveAttach = std::make_unique<ComboAttachment>(apvts, ParamID::oscBWave, oscBWaveBox);
    oscBPitchAttach = std::make_unique<Attachment>(apvts, ParamID::oscBPitch, oscBPitchKnob);
    oscBFineAttach = std::make_unique<Attachment>(apvts, ParamID::oscBFine, oscBFineKnob);
    oscBLevelAttach = std::make_unique<Attachment>(apvts, ParamID::oscBLevel, oscBLevelKnob);
    fmAmountAttach = std::make_unique<Attachment>(apvts, ParamID::fmAmount, fmAmountKnob);
    subLevelAttach = std::make_unique<Attachment>(apvts, ParamID::subLevel, subLevelKnob);
    subWaveAttach = std::make_unique<ComboAttachment>(apvts, ParamID::subWaveform, subWaveBox);
    noiseLevelAttach = std::make_unique<Attachment>(apvts, ParamID::noiseLevel, noiseLevelKnob);

    filterTypeAttach = std::make_unique<ComboAttachment>(apvts, ParamID::filterType, filterTypeBox);
    filterCutoffAttach = std::make_unique<Attachment>(apvts, ParamID::filterCutoff, filterCutoffKnob);
    filterResAttach = std::make_unique<Attachment>(apvts, ParamID::filterResonance, filterResKnob);
    filterDriveAttach = std::make_unique<Attachment>(apvts, ParamID::filterDrive, filterDriveKnob);
    filterMixAttach = std::make_unique<Attachment>(apvts, ParamID::filterMix, filterMixKnob);
    filterKeyTrackAttach = std::make_unique<Attachment>(apvts, ParamID::filterKeyTrack, filterKeyTrackKnob);

    ampAtkAttach = std::make_unique<Attachment>(apvts, ParamID::ampAttack, ampAttackKnob);
    ampDecAttach = std::make_unique<Attachment>(apvts, ParamID::ampDecay, ampDecayKnob);
    ampSusAttach = std::make_unique<Attachment>(apvts, ParamID::ampSustain, ampSustainKnob);
    ampRelAttach = std::make_unique<Attachment>(apvts, ParamID::ampRelease, ampReleaseKnob);
    filtAtkAttach = std::make_unique<Attachment>(apvts, ParamID::filterAttack, filtAttackKnob);
    filtDecAttach = std::make_unique<Attachment>(apvts, ParamID::filterDecay, filtDecayKnob);
    filtSusAttach = std::make_unique<Attachment>(apvts, ParamID::filterSustain, filtSustainKnob);
    filtRelAttach = std::make_unique<Attachment>(apvts, ParamID::filterRelease, filtReleaseKnob);
    filtDepthAttach = std::make_unique<Attachment>(apvts, ParamID::filterEnvDepth, filtEnvDepthKnob);

    lfo1ShapeAttach = std::make_unique<ComboAttachment>(apvts, ParamID::lfo1Shape, lfo1ShapeBox);
    lfo1DestAttach = std::make_unique<ComboAttachment>(apvts, ParamID::lfo1Dest, lfo1DestBox);
    lfo1RateAttach = std::make_unique<Attachment>(apvts, ParamID::lfo1Rate, lfo1RateKnob);
    lfo1DepthAttach = std::make_unique<Attachment>(apvts, ParamID::lfo1Depth, lfo1DepthKnob);
    lfo2ShapeAttach = std::make_unique<ComboAttachment>(apvts, ParamID::lfo2Shape, lfo2ShapeBox);
    lfo2DestAttach = std::make_unique<ComboAttachment>(apvts, ParamID::lfo2Dest, lfo2DestBox);
    lfo2RateAttach = std::make_unique<Attachment>(apvts, ParamID::lfo2Rate, lfo2RateKnob);
    lfo2DepthAttach = std::make_unique<Attachment>(apvts, ParamID::lfo2Depth, lfo2DepthKnob);
    seqRateAttach = std::make_unique<Attachment>(apvts, ParamID::seqRate, seqRateKnob);
    seqSmoothAttach = std::make_unique<Attachment>(apvts, ParamID::seqSmooth, seqSmoothKnob);
    seqDestAttach = std::make_unique<ComboAttachment>(apvts, ParamID::seqDest, seqDestBox);

    seqStepAttach[0] = std::make_unique<Attachment>(apvts, ParamID::seqStep1, seqSliders[0]);
    seqStepAttach[1] = std::make_unique<Attachment>(apvts, ParamID::seqStep2, seqSliders[1]);
    seqStepAttach[2] = std::make_unique<Attachment>(apvts, ParamID::seqStep3, seqSliders[2]);
    seqStepAttach[3] = std::make_unique<Attachment>(apvts, ParamID::seqStep4, seqSliders[3]);
    seqStepAttach[4] = std::make_unique<Attachment>(apvts, ParamID::seqStep5, seqSliders[4]);
    seqStepAttach[5] = std::make_unique<Attachment>(apvts, ParamID::seqStep6, seqSliders[5]);
    seqStepAttach[6] = std::make_unique<Attachment>(apvts, ParamID::seqStep7, seqSliders[6]);
    seqStepAttach[7] = std::make_unique<Attachment>(apvts, ParamID::seqStep8, seqSliders[7]);

    modAtkAttach = std::make_unique<Attachment>(apvts, ParamID::modAttack, modAtkKnob);
    modDecAttach = std::make_unique<Attachment>(apvts, ParamID::modDecay, modDecKnob);
    modSusAttach = std::make_unique<Attachment>(apvts, ParamID::modSustain, modSusKnob);
    modRelAttach = std::make_unique<Attachment>(apvts, ParamID::modRelease, modRelKnob);
    modDepthAttach = std::make_unique<Attachment>(apvts, ParamID::modDepth, modDepthKnob);

    distTypeAttach = std::make_unique<ComboAttachment>(apvts, ParamID::distType, distTypeBox);
    distAmountAttach = std::make_unique<Attachment>(apvts, ParamID::distAmount, distAmountKnob);
    distMixAttach = std::make_unique<Attachment>(apvts, ParamID::distMix, distMixKnob);
    xoverLowAttach = std::make_unique<Attachment>(apvts, ParamID::xoverLow, xoverLowKnob);
    xoverHighAttach = std::make_unique<Attachment>(apvts, ParamID::xoverHigh, xoverHighKnob);
    compThreshAttach = std::make_unique<Attachment>(apvts, ParamID::compThresh, compThreshKnob);
    compRatioAttach = std::make_unique<Attachment>(apvts, ParamID::compRatio, compRatioKnob);
    widthAttach = std::make_unique<Attachment>(apvts, ParamID::widthAmount, widthKnob);
    masterGainAttach = std::make_unique<Attachment>(apvts, ParamID::masterGain, masterGainKnob);

    // Load cyber background
    bgImage = juce::ImageCache::getFromMemory(SkuntBinaryData::bg_cyber_jpg, SkuntBinaryData::bg_cyber_jpgSize);

    setSize(1100, 860);
    startTimerHz(20);
}

PluginEditor::~PluginEditor()
{
    stopTimer();
    setLookAndFeel(nullptr);
}

void PluginEditor::paint(juce::Graphics& g)
{
    if (bgImage.isValid())
        g.drawImage(bgImage, getLocalBounds().toFloat(),
                    juce::RectanglePlacement(juce::RectanglePlacement::fillDestination));
    else
        g.fillAll(juce::Colour(0xFF0D0D0D));
}

void PluginEditor::resized()
{
    const int pad = 8;
    const int panelW = (getWidth() - pad * 5) / 4;   // ~265
    const int panelH = 360;
    const int topH = 50;
    const int fxH = 170;
    const int masterH = getHeight() - topH - panelH - fxH - pad * 3;

    // Top bar
    titleLabel.setBounds(pad, 6, 300, 38);
    randomButton.setBounds(getWidth() - 420, 10, 60, 28);
    initButton.setBounds(getWidth() - 350, 10, 60, 28);
    presetBox.setBounds(getWidth() - 280, 10, 180, 28);

    // Row 1: 4 main panels
    int y1 = topH;
    layoutOscPanel(pad, y1, panelW, panelH);
    layoutFilterPanel(pad * 2 + panelW, y1, panelW, panelH);
    layoutEnvelopePanel(pad * 3 + panelW * 2, y1, panelW, panelH);
    layoutModulationPanel(pad * 4 + panelW * 3, y1, getWidth() - (pad * 4 + panelW * 3) - pad, panelH);

    // Row 2: Effects
    int y2 = y1 + panelH + pad;
    layoutEffectsPanel(pad, y2, getWidth() - pad * 2, fxH);

    // Row 3: Master
    int y3 = y2 + fxH + pad;
    layoutMasterSection(pad, y3, getWidth() - pad * 2, masterH);
}

// ===== OSCILLATOR PANEL =====
// Layout: each control gets its own non-overlapping row
// comboH=22, knobH=56, gap=10 between rows
void PluginEditor::layoutOscPanel(int x, int y, int w, int h)
{
    oscPanel.setBounds(x, y, w, h);
    const int m = 6;
    const int cx = x + m;
    const int cw = w - m * 2;
    const int kh = 56;       // knob height
    const int ch = 22;       // combo height
    const int g = 8;         // gap

    int ry = y + 18;         // first row y (below panel title)

    // Row 0: Osc A Wave combo (full width)
    oscAWaveBox.setBounds(cx, ry, cw, ch);
    ry += ch + g;

    // Row 1: Pitch A | Fine A
    oscAPitchKnob.setBounds(cx, ry, cw / 2, kh);
    oscAFineKnob.setBounds(cx + cw / 2, ry, cw / 2, kh);
    ry += kh + g;

    // Row 2: Level A | Sub Wave (side by side, same height)
    oscALevelKnob.setBounds(cx, ry, cw / 2, kh);
    subWaveBox.setBounds(cx + cw / 2, ry + (kh - ch) / 2, cw / 2, ch);
    ry += kh + g;

    // Row 3: Osc B Wave combo
    oscBWaveBox.setBounds(cx, ry, cw, ch);
    ry += ch + g;

    // Row 4: Pitch B | Fine B | Level B
    oscBPitchKnob.setBounds(cx, ry, cw / 3, kh);
    oscBFineKnob.setBounds(cx + cw / 3, ry, cw / 3, kh);
    oscBLevelKnob.setBounds(cx + 2 * cw / 3, ry, cw / 3, kh);
    ry += kh + g;

    // Row 5: FM | Sub Level | Noise
    fmAmountKnob.setBounds(cx, ry, cw / 3, kh);
    subLevelKnob.setBounds(cx + cw / 3, ry, cw / 3, kh);
    noiseLevelKnob.setBounds(cx + 2 * cw / 3, ry, cw / 3, kh);
}

// ===== FILTER PANEL =====
void PluginEditor::layoutFilterPanel(int x, int y, int w, int h)
{
    filterPanel.setBounds(x, y, w, h);
    const int m = 6;
    const int cx = x + m;
    const int cw = w - m * 2;
    const int kh = 56;
    const int ch = 22;
    const int g = 8;

    int ry = y + 18;

    // Row 0: Filter Type combo
    filterTypeBox.setBounds(cx, ry, cw, ch);
    ry += ch + g;

    // Row 1: Cutoff | Res
    filterCutoffKnob.setBounds(cx, ry, cw / 2, kh);
    filterResKnob.setBounds(cx + cw / 2, ry, cw / 2, kh);
    ry += kh + g;

    // Row 2: Drive | Filter Mix | Key Track
    filterDriveKnob.setBounds(cx, ry, cw / 3, kh);
    filterMixKnob.setBounds(cx + cw / 3, ry, cw / 3, kh);
    filterKeyTrackKnob.setBounds(cx + 2 * cw / 3, ry, cw / 3, kh);
}

// ===== ENVELOPE PANEL =====
void PluginEditor::layoutEnvelopePanel(int x, int y, int w, int h)
{
    envPanel.setBounds(x, y, w, h);
    const int m = 6;
    const int cx = x + m;
    const int cw = w - m * 2;
    const int kh = 56;
    const int g = 8;

    int ry = y + 18;

    // Row 0: Amp ADSR (4 knobs)
    int kw = cw / 4;
    ampAttackKnob.setBounds(cx, ry, kw, kh);
    ampDecayKnob.setBounds(cx + kw, ry, kw, kh);
    ampSustainKnob.setBounds(cx + kw * 2, ry, kw, kh);
    ampReleaseKnob.setBounds(cx + kw * 3, ry, kw, kh);
    ry += kh + g;

    // Row 1: Filter ADSR (4 knobs)
    filtAttackKnob.setBounds(cx, ry, kw, kh);
    filtDecayKnob.setBounds(cx + kw, ry, kw, kh);
    filtSustainKnob.setBounds(cx + kw * 2, ry, kw, kh);
    filtReleaseKnob.setBounds(cx + kw * 3, ry, kw, kh);
    ry += kh + g;

    // Row 2: F.Depth knob (half width) + amp env display
    filtEnvDepthKnob.setBounds(cx, ry, cw / 2, kh);
    ampEnvDisplay.setBounds(cx + cw / 2, ry + 8, cw / 2, 40);
    ry += kh + g;

    // Row 3: Filter env display (full width)
    filtEnvDisplay.setBounds(cx, ry, cw, 40);
}

// ===== MODULATION PANEL =====
void PluginEditor::layoutModulationPanel(int x, int y, int w, int h)
{
    modPanel.setBounds(x, y, w, h);
    const int m = 6;
    const int cx = x + m;
    const int cw = w - m * 2;
    const int half = cw / 2;
    const int kh = 56;
    const int ch = 22;
    const int g = 8;

    int ry = y + 18;

    // === LEFT COLUMN: LFO1 ===
    // Row 0: LFO1 Shape + Dest combos
    lfo1ShapeBox.setBounds(cx, ry, half - 4, ch);
    lfo1DestBox.setBounds(cx + half, ry, half - 4, ch);
    ry += ch + g;

    // Row 1: LFO1 Rate | LFO1 Depth
    lfo1RateKnob.setBounds(cx, ry, half / 2, kh);
    lfo1DepthKnob.setBounds(cx + half / 2, ry, half / 2, kh);
    ry += kh + g;

    // Row 2: LFO2 Shape + Dest combos
    lfo2ShapeBox.setBounds(cx, ry, half - 4, ch);
    lfo2DestBox.setBounds(cx + half, ry, half - 4, ch);
    ry += ch + g;

    // Row 3: LFO2 Rate | LFO2 Depth
    lfo2RateKnob.setBounds(cx, ry, half / 2, kh);
    lfo2DepthKnob.setBounds(cx + half / 2, ry, half / 2, kh);
    ry += kh + g + 4;

    // === STEP SEQUENCER (right side) ===
    // 8 small step sliders in a horizontal row
    int seqY = y + 18 + ch + g;  // Aligned with LFO1 knobs
    int stepW = (half - 8) / 8;
    int seqX = cx + half + 4;
    for (int i = 0; i < 8; ++i)
    {
        seqSliders[i].setBounds(seqX + i * stepW, seqY, stepW - 2, 44);
    }

    // Seq Rate + Smooth below step sliders
    int seqRow2 = seqY + 44 + g;
    seqRateKnob.setBounds(seqX, seqRow2, half / 2 - 4, kh);
    seqSmoothKnob.setBounds(seqX + half / 2, seqRow2, half / 2 - 4, kh);

    // Seq Dest combo
    int seqRow3 = seqRow2 + kh + g;
    seqDestBox.setBounds(seqX, seqRow3, half - 4, ch);

    // === MOD ENVELOPE (below LFO2, left side) ===
    int meY = ry;
    int meKw = cw / 3;
    modAtkKnob.setBounds(cx, meY, meKw, kh);
    modDecKnob.setBounds(cx + meKw, meY, meKw, kh);
    modSusKnob.setBounds(cx + meKw * 2, meY, meKw, kh);
    meY += kh + g;
    modRelKnob.setBounds(cx, meY, cw / 2, kh);
    modDepthKnob.setBounds(cx + cw / 2, meY, cw / 2, kh);
}

// ===== EFFECTS PANEL =====
void PluginEditor::layoutEffectsPanel(int x, int y, int w, int h)
{
    fxPanel.setBounds(x, y, w, h);
    const int m = 8;
    const int cx = x + m;
    const int cw = w - m * 2;
    const int sec = cw / 4;
    const int kh = 56;
    const int ch = 22;
    const int g = 8;

    int ry = y + 18;

    // Row 0: Distortion Type combo (left section)
    distTypeBox.setBounds(cx, ry, sec - 10, ch);
    ry += ch + g;

    // Row 1: All FX knobs in a single row
    distAmountKnob.setBounds(cx, ry, sec / 2, kh);
    distMixKnob.setBounds(cx + sec / 2, ry, sec / 2, kh);
    xoverLowKnob.setBounds(cx + sec, ry, sec / 2, kh);
    xoverHighKnob.setBounds(cx + sec + sec / 2, ry, sec / 2, kh);
    compThreshKnob.setBounds(cx + sec * 2, ry, sec / 2, kh);
    compRatioKnob.setBounds(cx + sec * 2 + sec / 2, ry, sec / 2, kh);
    widthKnob.setBounds(cx + sec * 3, ry, sec / 2, kh);
}

// ===== MASTER SECTION =====
void PluginEditor::layoutMasterSection(int x, int y, int w, int h)
{
    masterPanel.setBounds(x, y, w, h);
    const int m = 8;
    const int cx = x + m;

    masterGainKnob.setBounds(cx, y + 10, 90, 60);
    vuMeter.setBounds(cx + 100, y + 10, 24, h - 20);
    scope.setBounds(x + w - 200 - m, y + 10, 190, h - 20);
}

void PluginEditor::timerCallback()
{
    float left = processor.getOutputLevelLeft();
    vuMeter.setLevel(left);

    float scopeVal = (left + processor.getOutputLevelRight()) * 0.5f;
    scope.pushSample(scopeVal);

    if (auto* v = apvts.getRawParameterValue(ParamID::ampAttack))
        ampEnvDisplay.setAttack(*v);
    if (auto* v = apvts.getRawParameterValue(ParamID::ampDecay))
        ampEnvDisplay.setDecay(*v);
    if (auto* v = apvts.getRawParameterValue(ParamID::ampSustain))
        ampEnvDisplay.setSustain(*v);
    if (auto* v = apvts.getRawParameterValue(ParamID::ampRelease))
        ampEnvDisplay.setRelease(*v);

    if (auto* v = apvts.getRawParameterValue(ParamID::filterAttack))
        filtEnvDisplay.setAttack(*v);
    if (auto* v = apvts.getRawParameterValue(ParamID::filterDecay))
        filtEnvDisplay.setDecay(*v);
    if (auto* v = apvts.getRawParameterValue(ParamID::filterSustain))
        filtEnvDisplay.setSustain(*v);
    if (auto* v = apvts.getRawParameterValue(ParamID::filterRelease))
        filtEnvDisplay.setRelease(*v);
}
