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

    oscAPitchKnob.setLabel("Pitch A"); oscAPitchKnob.setSuffix("");
    oscAFineKnob.setLabel("Fine A"); oscAFineKnob.setSuffix("");
    oscALevelKnob.setLabel("Level A"); oscALevelKnob.setSuffix("");
    oscBPitchKnob.setLabel("Pitch B"); oscBPitchKnob.setSuffix("");
    oscBFineKnob.setLabel("Fine B"); oscBFineKnob.setSuffix("");
    oscBLevelKnob.setLabel("Level B"); oscBLevelKnob.setSuffix("");
    fmAmountKnob.setLabel("FM"); fmAmountKnob.setSuffix("");
    subLevelKnob.setLabel("Sub"); subLevelKnob.setSuffix("");
    noiseLevelKnob.setLabel("Noise"); noiseLevelKnob.setSuffix("");

    addAndMakeVisible(oscPanel);
    addAndMakeVisible(oscAWaveBox); addAndMakeVisible(oscBWaveBox);
    addAndMakeVisible(oscAPitchKnob); addAndMakeVisible(oscAFineKnob); addAndMakeVisible(oscALevelKnob);
    addAndMakeVisible(oscBPitchKnob); addAndMakeVisible(oscBFineKnob); addAndMakeVisible(oscBLevelKnob);
    addAndMakeVisible(fmAmountKnob); addAndMakeVisible(subLevelKnob); addAndMakeVisible(noiseLevelKnob);

    // ===== FILTER PANEL =====
    filterTypeBox.addItemList({"LP12", "LP24", "BP", "HP"}, 1);
    filterCutoffKnob.setLabel("Cutoff"); filterCutoffKnob.setSuffix(" Hz");
    filterResKnob.setLabel("Res");
    filterDriveKnob.setLabel("Drive");
    filterKeyTrackKnob.setLabel("KeyTrk");

    addAndMakeVisible(filterPanel);
    addAndMakeVisible(filterTypeBox);
    addAndMakeVisible(filterCutoffKnob); addAndMakeVisible(filterResKnob);
    addAndMakeVisible(filterDriveKnob); addAndMakeVisible(filterKeyTrackKnob);

    // ===== ENVELOPE PANEL =====
    ampAttackKnob.setLabel("A.Att"); ampDecayKnob.setLabel("A.Dec");
    ampSustainKnob.setLabel("A.Sus"); ampReleaseKnob.setLabel("A.Rel");
    filtAttackKnob.setLabel("F.Att"); filtDecayKnob.setLabel("F.Dec");
    filtSustainKnob.setLabel("F.Sus"); filtReleaseKnob.setLabel("F.Rel");
    filtEnvDepthKnob.setLabel("F.Depth");

    addAndMakeVisible(envPanel);
    addAndMakeVisible(ampAttackKnob); addAndMakeVisible(ampDecayKnob);
    addAndMakeVisible(ampSustainKnob); addAndMakeVisible(ampReleaseKnob);
    addAndMakeVisible(filtAttackKnob); addAndMakeVisible(filtDecayKnob);
    addAndMakeVisible(filtSustainKnob); addAndMakeVisible(filtReleaseKnob);
    addAndMakeVisible(filtEnvDepthKnob);
    addAndMakeVisible(ampEnvDisplay); addAndMakeVisible(filtEnvDisplay);

    // ===== MODULATION PANEL =====
    lfo1ShapeBox.addItemList({"Sine", "Triangle", "Saw", "Square", "S&H", "Noise"}, 1);
    lfo1DestBox.addItemList({"Cutoff", "Pitch", "FM", "Resonance"}, 1);
    lfo2ShapeBox.addItemList({"Sine", "Triangle", "Saw", "Square", "S&H", "Noise"}, 1);
    lfo2DestBox.addItemList({"Cutoff", "Pitch", "FM", "Resonance"}, 1);
    seqDestBox.addItemList({"Cutoff", "Pitch", "FM", "Resonance"}, 1);

    lfo1RateKnob.setLabel("LFO1 Rate"); lfo1DepthKnob.setLabel("LFO1 Dpt");
    lfo2RateKnob.setLabel("LFO2 Rate"); lfo2DepthKnob.setLabel("LFO2 Dpt");
    seqRateKnob.setLabel("Seq Rate"); seqSmoothKnob.setLabel("Smooth");

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
    modAtkKnob.setLabel("M.Att"); modDecKnob.setLabel("M.Dec");
    modSusKnob.setLabel("M.Sus"); modRelKnob.setLabel("M.Rel");
    modDepthKnob.setLabel("M.Depth");
    addAndMakeVisible(modAtkKnob); addAndMakeVisible(modDecKnob);
    addAndMakeVisible(modSusKnob); addAndMakeVisible(modRelKnob);
    addAndMakeVisible(modDepthKnob);

    // ===== EFFECTS PANEL =====
    distTypeBox.addItemList({"Tube", "Foldback", "Bitcrush"}, 1);
    distAmountKnob.setLabel("Dist Amt"); distMixKnob.setLabel("Dist Mix");
    xoverLowKnob.setLabel("Xover Lo"); xoverHighKnob.setLabel("Xover Hi");
    compThreshKnob.setLabel("Thresh"); compRatioKnob.setLabel("Ratio");
    widthKnob.setLabel("Width");

    addAndMakeVisible(fxPanel);
    addAndMakeVisible(distTypeBox);
    addAndMakeVisible(distAmountKnob); addAndMakeVisible(distMixKnob);
    addAndMakeVisible(xoverLowKnob); addAndMakeVisible(xoverHighKnob);
    addAndMakeVisible(compThreshKnob); addAndMakeVisible(compRatioKnob);
    addAndMakeVisible(widthKnob);

    // ===== MASTER PANEL =====
    masterGainKnob.setLabel("Gain");
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
    noiseLevelAttach = std::make_unique<Attachment>(apvts, ParamID::noiseLevel, noiseLevelKnob);

    filterTypeAttach = std::make_unique<ComboAttachment>(apvts, ParamID::filterType, filterTypeBox);
    filterCutoffAttach = std::make_unique<Attachment>(apvts, ParamID::filterCutoff, filterCutoffKnob);
    filterResAttach = std::make_unique<Attachment>(apvts, ParamID::filterResonance, filterResKnob);
    filterDriveAttach = std::make_unique<Attachment>(apvts, ParamID::filterDrive, filterDriveKnob);
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

    setSize(1000, 700);
    startTimerHz(20);
}

PluginEditor::~PluginEditor()
{
    stopTimer();
    setLookAndFeel(nullptr);
}

void PluginEditor::paint(juce::Graphics& g)
{
    // Background
    g.fillAll(juce::Colour(0xFF0D0D0D));

    // Camo pattern
    customLNF.drawCamoBackground(g, getLocalBounds());

    // Subtle grid lines for military feel
    g.setColour(juce::Colour(0xFF2D5016).withAlpha(0.08f));
    for (int x = 0; x < getWidth(); x += 50)
        g.drawVerticalLine(x, 0, getHeight());
    for (int y = 0; y < getHeight(); y += 50)
        g.drawHorizontalLine(y, 0, getWidth());
}

void PluginEditor::resized()
{
    int pad = 4;
    int panelW = (getWidth() - pad * 5) / 4;
    int panelH = 310;
    int topH = 55;
    int fxH = 180;
    int masterH = getHeight() - topH - panelH - fxH - pad * 4;

    // Title bar
    titleLabel.setBounds(pad, 8, 300, 36);
    presetBox.setBounds(getWidth() - 200, 10, 140, 26);
    initButton.setBounds(getWidth() - 340, 10, 60, 26);
    randomButton.setBounds(getWidth() - 410, 10, 60, 26);

    // Row 1: 4 panels
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

void PluginEditor::layoutOscPanel(int x, int y, int w, int h)
{
    oscPanel.setBounds(x, y, w, h);
    int m = 8;
    int cx = x + m;
    int cw = w - m * 2;
    int row1 = y + 22;
    int row2 = row1 + 70;
    int row3 = row2 + 70;
    int row4 = row3 + 70;

    oscAWaveBox.setBounds(cx, row1, cw, 22);
    oscAPitchKnob.setBounds(cx, row1 + 26, cw / 2, 44);
    oscAFineKnob.setBounds(cx + cw / 2, row1 + 26, cw / 2, 44);
    oscALevelKnob.setBounds(cx, row2, cw / 2, 44);

    oscBWaveBox.setBounds(cx, row2 + 48, cw, 22);
    oscBPitchKnob.setBounds(cx, row3, cw / 3, 40);
    oscBFineKnob.setBounds(cx + cw / 3, row3, cw / 3, 40);
    oscBLevelKnob.setBounds(cx + 2 * cw / 3, row3, cw / 3, 40);

    fmAmountKnob.setBounds(cx, row4, cw / 3, 40);
    subLevelKnob.setBounds(cx + cw / 3, row4, cw / 3, 40);
    noiseLevelKnob.setBounds(cx + 2 * cw / 3, row4, cw / 3, 40);
}

void PluginEditor::layoutFilterPanel(int x, int y, int w, int h)
{
    filterPanel.setBounds(x, y, w, h);
    int m = 8;
    int cx = x + m;
    int cw = w - m * 2;
    int row1 = y + 22;
    int row2 = row1 + 50;
    int row3 = row2 + 58;
    int row4 = row3 + 58;

    filterTypeBox.setBounds(cx, row1, cw, 22);
    filterCutoffKnob.setBounds(cx, row2, cw / 2, 52);
    filterResKnob.setBounds(cx + cw / 2, row2, cw / 2, 52);
    filterDriveKnob.setBounds(cx, row3, cw / 2, 52);
    filterKeyTrackKnob.setBounds(cx + cw / 2, row3, cw / 2, 52);
}

void PluginEditor::layoutEnvelopePanel(int x, int y, int w, int h)
{
    envPanel.setBounds(x, y, w, h);
    int m = 6;
    int cx = x + m;
    int cw = w - m * 2;
    int row1 = y + 22;
    int row2 = row1 + 56;
    int row3 = row2 + 56;
    int row4 = row3 + 56;

    // Amp ADSR
    ampAttackKnob.setBounds(cx, row1, cw / 4, 40);
    ampDecayKnob.setBounds(cx + cw / 4, row1, cw / 4, 40);
    ampSustainKnob.setBounds(cx + cw / 2, row1, cw / 4, 40);
    ampReleaseKnob.setBounds(cx + 3 * cw / 4, row1, cw / 4, 40);

    // Filter ADSR
    filtAttackKnob.setBounds(cx, row2, cw / 4, 40);
    filtDecayKnob.setBounds(cx + cw / 4, row2, cw / 4, 40);
    filtSustainKnob.setBounds(cx + cw / 2, row2, cw / 4, 40);
    filtReleaseKnob.setBounds(cx + 3 * cw / 4, row2, cw / 4, 40);

    filtEnvDepthKnob.setBounds(cx, row3, cw / 2, 44);

    ampEnvDisplay.setBounds(cx + cw / 2, row3, cw / 2, 40);
    filtEnvDisplay.setBounds(cx, row4, cw, 40);
}

void PluginEditor::layoutModulationPanel(int x, int y, int w, int h)
{
    modPanel.setBounds(x, y, w, h);
    int m = 6;
    int cx = x + m;
    int cw = w - m * 2;
    int half = cw / 2;
    int row1 = y + 22;
    int row2 = row1 + 48;
    int row3 = row2 + 56;
    int row4 = row3 + 56;

    // LFO 1
    lfo1ShapeBox.setBounds(cx, row1, half - 2, 20);
    lfo1DestBox.setBounds(cx + half, row1, half - 2, 20);
    lfo1RateKnob.setBounds(cx, row2, half / 2, 38);
    lfo1DepthKnob.setBounds(cx + half / 2, row2, half / 2, 38);

    // LFO 2
    lfo2ShapeBox.setBounds(cx, row3, half - 2, 20);
    lfo2DestBox.setBounds(cx + half, row3, half - 2, 20);
    lfo2RateKnob.setBounds(cx, row4, half / 2, 38);
    lfo2DepthKnob.setBounds(cx + half / 2, row4, half / 2, 38);

    // Step sequencers (on the right side)
    int seqX = cx + half + 4;
    int seqY = row2;
    int seqW = half - 8;
    int stepW = seqW / 8;
    for (int i = 0; i < 8; ++i)
    {
        seqSliders[i].setBounds(seqX + i * stepW, seqY - 10, stepW - 2, 48);
    }
    seqRateKnob.setBounds(seqX, seqY + 44, seqW / 2, 34);
    seqSmoothKnob.setBounds(seqX + seqW / 2, seqY + 44, seqW / 2, 34);

    // Mod Env
    modAtkKnob.setBounds(cx + half, row4 + 10, half / 5, 30);
    modDecKnob.setBounds(cx + half + half / 5, row4 + 10, half / 5, 30);
    modSusKnob.setBounds(cx + half + 2 * half / 5, row4 + 10, half / 5, 30);
    modRelKnob.setBounds(cx + half + 3 * half / 5, row4 + 10, half / 5, 30);
    modDepthKnob.setBounds(cx + half + 4 * half / 5, row4 + 10, half / 5, 30);
}

void PluginEditor::layoutEffectsPanel(int x, int y, int w, int h)
{
    fxPanel.setBounds(x, y, w, h);
    int m = 8;
    int cx = x + m;
    int cw = w - m * 2;
    int sec = cw / 4;

    // Distortion section
    distTypeBox.setBounds(cx, y + 22, sec - 10, 22);
    distAmountKnob.setBounds(cx, y + 50, sec / 2, 50);
    distMixKnob.setBounds(cx + sec / 2, y + 50, sec / 2, 50);

    // Crossover
    xoverLowKnob.setBounds(cx + sec, y + 22, sec / 2, 50);
    xoverHighKnob.setBounds(cx + sec + sec / 2, y + 22, sec / 2, 50);

    // Compressor
    compThreshKnob.setBounds(cx + sec * 2, y + 22, sec / 2, 50);
    compRatioKnob.setBounds(cx + sec * 2 + sec / 2, y + 22, sec / 2, 50);

    // Width
    widthKnob.setBounds(cx + sec * 3, y + 22, sec / 2, 50);
}

void PluginEditor::layoutMasterSection(int x, int y, int w, int h)
{
    masterPanel.setBounds(x, y, w, h);
    int m = 8;
    int cx = x + m;

    masterGainKnob.setBounds(cx, y + 10, 80, 60);
    vuMeter.setBounds(cx + 90, y + 10, 24, h - 20);
    scope.setBounds(x + w - 160 - m, y + 10, 150, h - 20);
}

void PluginEditor::timerCallback()
{
    float left = processor.getOutputLevelLeft();
    vuMeter.setLevel(left);

    float scopeVal = (left + processor.getOutputLevelRight()) * 0.5f;
    scope.pushSample(scopeVal);

    // Update ADSR displays from parameters
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
