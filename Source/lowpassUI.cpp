#include "../JuceLibraryCode/JuceHeader.h"
#include "lowpassUI.h"

enum {
	sine = 1,
	red = 2
};

lowpassUI::lowpassUI():
	//frequency
	minFrequency(50.0),
	maxFrequency(5000.0),
	defaultFrequency(2000.0), 
	//wah-wah
	minWahWahSamples(1),
	maxWahWahSamples(100000.0),
	isWahWahOn(false),
	isOn(false)
{
	addAndMakeVisible(componentLabel);
	componentLabel.setText(componentName + componentStatus, dontSendNotification);
	componentLabel.setColour(Label::textColourId, Colours::red);
	componentLabel.setJustificationType(Justification::centred);

	addAndMakeVisible(startButton);
	startButton.setButtonText("Lowpass");
	startButton.addListener(this);

	//Q
	addAndMakeVisible(sliderQ);
	sliderQ.setRange(0, 50);
	sliderQ.setValue(2, sendNotification);
	sliderQ.setSliderStyle(Slider::LinearBar); 
	sliderQ.addListener(this);

	addAndMakeVisible(labelQ);
	labelQ.setText("Q:", dontSendNotification);
	labelQ.setJustificationType(Justification::centred);

	//frequency
	addAndMakeVisible(sliderCutoff);
	sliderCutoff.setRange(minFrequency, maxFrequency);
	sliderCutoff.setSkewFactorFromMidPoint(500.0);
	sliderCutoff.setValue(defaultFrequency, sendNotification);
	sliderCutoff.setSliderStyle(Slider::LinearBar);
	sliderCutoff.setTextValueSuffix("Hz");
	sliderCutoff.addListener(this);

	addAndMakeVisible(labelCutoff);
	labelCutoff.setText("Cutoff frequency:", dontSendNotification);
	labelCutoff.setJustificationType(Justification::centred);

	//wah-wah
	addAndMakeVisible(buttonWahWah);
	buttonWahWah.setButtonText("Wah-wah");
	buttonWahWah.addListener(this);
	buttonWahWah.setEnabled(false);

	addAndMakeVisible(labelWahWahStatus);
	labelWahWahStatus.setText("Wah-wah is" + wahWahStatus, dontSendNotification);
	labelWahWahStatus.setColour(Label::textColourId, Colours::red);
	labelWahWahStatus.setJustificationType(Justification::centred);

	//combo box for choice of wah-wah generator
	addAndMakeVisible(comboBoxWahWah);
	comboBoxWahWah.addListener(this);
	comboBoxWahWah.addItem("Sine", sine);
	comboBoxWahWah.addItem("Red noise", red);
	comboBoxWahWah.setSelectedId(sine, sendNotification);

	addAndMakeVisible(sliderWahWahSamples);
	sliderWahWahSamples.setRange(minWahWahSamples, maxWahWahSamples);
	sliderWahWahSamples.setValue(1000.0, sendNotification);
	sliderWahWahSamples.setSliderStyle(Slider::LinearBar);
	sliderWahWahSamples.addListener(this);

	addAndMakeVisible(labelWahWah);
	labelWahWah.setText("Wah-wah samples:", dontSendNotification);
	labelWahWah.setJustificationType(Justification::centred);

	//setting deviance for wah-wah
	addAndMakeVisible(labelWahWahDeviance);
	labelWahWahDeviance.setText("Wah-wah deviance:", dontSendNotification);
	labelWahWahDeviance.setJustificationType(Justification::centred);

	addAndMakeVisible(sliderDeviance);
	sliderDeviance.setRange(0.0, 1.0);
	sliderDeviance.setValue(0.5, sendNotification);
	sliderDeviance.setSliderStyle(Slider::LinearBar);
	sliderDeviance.addListener(this);
}

void lowpassUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	lowpassFilterInstance.initialize(samplesPerBlockExpected, sampleRate, sliderCutoff.getValue(), sliderQ.getValue());
}

void lowpassUI::processAudioBlock(const AudioSourceChannelInfo & bufferToFill)
{
	if (isOn) lowpassFilterInstance.processAudioBlock(bufferToFill);
}

void lowpassUI::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void lowpassUI::resized()
{
	componentLabel.setBounds(0, 5, getWidth(), 30);
	startButton.setBounds(10, getHeight()*0.2, 90, 30);

	//Q
	labelQ.setBounds(110, getHeight()*0.2, 30, 30);
	sliderQ.setBounds(150, getHeight()*0.2, 200, 30);
	//frequency
	labelCutoff.setBounds(360, getHeight()*0.2, 120, 30);
	sliderCutoff.setBounds(490, getHeight()*0.2, 200, 30);

	//wah-wah
	buttonWahWah.setBounds(10, getHeight()*0.5, 90, 30);
	labelWahWahStatus.setBounds(110, getHeight()*0.5, 120, 30);
	labelWahWah.setBounds(240, getHeight()*0.5, 140, 30);
	sliderWahWahSamples.setBounds(390, getHeight()*0.5, 200, 30);
	comboBoxWahWah.setBounds(610, getHeight()*0.5, 120, 30);
	//wah-wah second row - deviance
	labelWahWahDeviance.setBounds(10, getHeight()*0.8, 140, 30);
	sliderDeviance.setBounds(160, getHeight()*0.8, 200, 30);
}

void lowpassUI::buttonClicked(Button * button)
{
	if (button == &startButton) {
		if (isOn == false) {
			componentStatus = " ON";
			componentLabel.setText(componentName + componentStatus, dontSendNotification);
			componentLabel.setColour(Label::textColourId, Colours::green);
			isOn = true;

			buttonWahWah.setEnabled(true);
		}
		else {
			componentStatus = " OFF";
			componentLabel.setText(componentName + componentStatus, dontSendNotification);
			componentLabel.setColour(Label::textColourId, Colours::red);
			isOn = false;
			lowpassFilterInstance.clear();

			buttonWahWah.setEnabled(false);
			buttonWahWah.setToggleState(false, sendNotification);
		}
	}
	if (button == &buttonWahWah) {
		if (isWahWahOn == false) {
			wahWahStatus = " ON";
			labelWahWahStatus.setText("Wah-wah is" + wahWahStatus, dontSendNotification);
			labelWahWahStatus.setColour(Label::textColourId, Colours::green);
			isWahWahOn = 1;

			isWahWahOn = true;
			lowpassFilterInstance.enableWahWah(currentWahWahGenerator, sliderWahWahSamples.getValue(), sliderDeviance.getValue());
		}
		else {
			wahWahStatus = " OFF";
			labelWahWahStatus.setText("Wah-wah is" + wahWahStatus, dontSendNotification);
			labelWahWahStatus.setColour(Label::textColourId, Colours::red);
			isWahWahOn = 0;

			isWahWahOn = false;
			lowpassFilterInstance.disableWahWah();
		}
	}
}

void lowpassUI::sliderValueChanged(Slider * slider)
{
	if (slider == &sliderCutoff || slider == &sliderQ)
		lowpassFilterInstance.setCutoffAndQ(sliderCutoff.getValue(), sliderQ.getValue());

	if ((slider == &sliderWahWahSamples || slider == &sliderDeviance) && isWahWahOn)
		lowpassFilterInstance.updateWahWah(currentWahWahGenerator, sliderWahWahSamples.getValue(), sliderDeviance.getValue());
}

void lowpassUI::comboBoxChanged(ComboBox * comboBox)
{
	if (comboBox == &comboBoxWahWah) { 
		currentWahWahGenerator = comboBox->getSelectedId(); 
		lowpassFilterInstance.updateWahWah(currentWahWahGenerator, sliderWahWahSamples.getValue(), sliderDeviance.getValue());
	}
}
