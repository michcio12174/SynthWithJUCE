#include "../JuceLibraryCode/JuceHeader.h"
#include "envelopeGenerator.h"

enum state {
	attack = 0,
	decay = 1,
	sustain = 2,
	release = 3,
	silent = -1
};

envelopeGenerator::envelopeGenerator():
	sampleRate(0),
	keyCode(0),
	positionInTheState(0),
	prevMultiplier(0),
	wasSampleRateSet(false),
	isKeyPressed(false),
	peakVolume(1.0),
	sustainVolume(0.7),
	currentState(silent)
{
	addAndMakeVisible(componentLabel);
	componentLabel.setText(componentName + componentStatus, dontSendNotification);
	componentLabel.setColour(Label::textColourId, Colours::red);
	componentLabel.setJustificationType(Justification::centred);

	addAndMakeVisible(startButton);
	startButton.setButtonText("Turn on");
	startButton.addListener(this);

	//sliders for controlling length of phases
	float max = 10000, min = 0;
	addAndMakeVisible(labelAttack);
	labelAttack.setText("Attack", dontSendNotification);
	labelAttack.setJustificationType(Justification::centred);

	addAndMakeVisible(labelDecay);
	labelDecay.setText("Decay", dontSendNotification);
	labelDecay.setJustificationType(Justification::centred);

	addAndMakeVisible(labelSustain);
	labelSustain.setText("Sustain", dontSendNotification);
	labelSustain.setJustificationType(Justification::centred);

	addAndMakeVisible(labelRelease);
	labelRelease.setText("Release", dontSendNotification);
	labelRelease.setJustificationType(Justification::centred);

	addAndMakeVisible(sliderAttack);
	sliderAttack.setRange(min, max, 1.0);
	sliderAttack.setValue(50, sendNotification); //fire the listener to calculate initial deltas
	sliderAttack.setSliderStyle(Slider::LinearBar);
	sliderAttack.addListener(this);

	addAndMakeVisible(sliderDecay);
	sliderDecay.setRange(min, max, 1.0);
	sliderDecay.setValue(50, sendNotification);
	sliderDecay.setSliderStyle(Slider::LinearBar);
	sliderDecay.addListener(this);

	addAndMakeVisible(sliderSustain);
	sliderSustain.setRange(min, max, 1.0);
	sliderSustain.setValue(100, sendNotification);
	sliderSustain.setSliderStyle(Slider::LinearBar);
	sliderSustain.addListener(this);

	addAndMakeVisible(sliderRelease);
	sliderRelease.setRange(min, max, 1.0);
	sliderRelease.setValue(50, sendNotification);
	sliderRelease.setSliderStyle(Slider::LinearBar);
	sliderRelease.addListener(this);

	isOn = 0;

	setWantsKeyboardFocus(true);
	this->addKeyListener(this);
}

envelopeGenerator::~envelopeGenerator()
{
	this->removeKeyListener(this);
}

void envelopeGenerator::prepareToPlay(double sampleRate, int keyCode)
{
	this->sampleRate = sampleRate;
	this->keyCode = keyCode;
	wasSampleRateSet = true;
}

void envelopeGenerator::processAudioBlock(const AudioSourceChannelInfo & bufferToFill)
{
	if (isOn) { //if not turned on, the sound will not by altered
		if (wasSampleRateSet && currentState != silent) {

			float multiplier = prevMultiplier;

			//for each sample in buffer
			for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
			{
				if (currentState == attack)
				{
					multiplier += deltaAttack;
				}
				if (currentState == decay)
				{
					multiplier += deltaDecay;
				}
				if (currentState == release)
				{
					multiplier += deltaRelease;
				}
				if (currentState == silent)
				{
					multiplier = 0;
				}

				//multiplying the same sample at the same position in all channels
				for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
				{
					// Get a pointer to the start sample in the buffer for this audio output channel
					float* buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
					buffer[sample] *= multiplier;
				}

				//If the state is silent we do not change position or state untill the rest of buffer is filled with silence
				if (currentState != silent) {
					++positionInTheState;
					//go to next state if samples for previous one were used
					//go silent if we iterated over all states
					if (positionInTheState >= samplesPerState[currentState]) {
						//if we are out of samples for sustain, but key is still pressed, we won't go into decay
						if (!(currentState == sustain && isKeyPressed)) {
							++currentState;
							positionInTheState = 0;
						}
					}
					if (currentState >= 4) currentState = silent;
				}

				prevMultiplier = multiplier;
			}
		}
		else {
			bufferToFill.clearActiveBufferRegion();
			if (!wasSampleRateSet) Logger::getCurrentLogger()->writeToLog("prepareToPlay was not called for one of envelope generators\n");
		}
	}
}

void envelopeGenerator::paint (Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

	g.setColour(Colours::grey);
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
}

void envelopeGenerator::resized()
{
	componentLabel.setBounds(0, 10, getWidth(), 30);
	startButton.setBounds(getWidth()*0.5 - 35, getHeight()*0.5 - 25, 70, 50);

	//sliders
	labelAttack.setBounds(10, (getHeight() - 60) * 0.2, getWidth() * 0.4, getHeight() * 0.3);
	sliderAttack.setBounds(10, (getHeight() - 60) * (0.2 + 0.3 + 0.05), getWidth() * 0.4, 30);

	labelDecay.setBounds(10, 30 + (getHeight() - 60) * (0.2 + 0.3 + 0.1), getWidth() * 0.4, getHeight() * 0.3);
	sliderDecay.setBounds(10, 30 + (getHeight() - 60) * (0.2 + 0.6 + 0.15), getWidth() * 0.4, 30);

	labelSustain.setBounds(getWidth() - getWidth() * 0.4 - 10, (getHeight() - 60) * 0.2, getWidth() * 0.4, getHeight() * 0.3);
	sliderSustain.setBounds(getWidth() - getWidth() * 0.4 - 10, (getHeight() - 60) * (0.2 + 0.3 + 0.05), getWidth() * 0.4, 30);

	labelRelease.setBounds(getWidth() - getWidth() * 0.4 - 10, 30 + (getHeight() - 60) * (0.2 + 0.3 + 0.1), getWidth() * 0.4, getHeight() * 0.3);
	sliderRelease.setBounds(getWidth() - getWidth() * 0.4 - 10, 30 + (getHeight() - 60) * (0.2 + 0.6 + 0.15), getWidth() * 0.4, 30);
}

void envelopeGenerator::buttonClicked(Button * button)
{
	if (!isOn) {
		startButton.setButtonText("Turn off");

		componentStatus = " ON";
		componentLabel.setText(componentName + componentStatus, dontSendNotification);
		componentLabel.setColour(Label::textColourId, Colours::green);
		isOn = 1;
	}
	else {
		startButton.setButtonText("Turn on");

		componentStatus = " OFF";
		componentLabel.setText(componentName + componentStatus, dontSendNotification);
		componentLabel.setColour(Label::textColourId, Colours::red);
		isOn = 0;
	}
}

void envelopeGenerator::keyDown()
{
	if (!isKeyPressed) { 
		isKeyPressed = true; 
		if (currentState == silent) {
			currentState = attack;
			positionInTheState = 0;
		}
	}
}

void envelopeGenerator::keyUp()
{
	isKeyPressed = false;
}

int envelopeGenerator::getKeyCode()
{
	return keyCode;
}

bool envelopeGenerator::keyStateChanged(bool isKeyDown, Component * originatingComponent)
//TODO component must be focused for this to work
{
	if (KeyPress::isKeyCurrentlyDown(keyCode)) { 
		keyDown();
		return true; 
	}
	else keyUp();

	return false;
}

bool envelopeGenerator::keyPressed(const KeyPress & key, Component * originatingComponent)
{
	return false;
}

void envelopeGenerator::sliderValueChanged(Slider * slider)
{
	if(slider == &sliderAttack){
		samplesPerState[attack] = slider->getValue();
		deltaAttack = recalculateDelta(samplesPerState[attack], 0, peakVolume);
	}

	if (slider == &sliderDecay) {
		samplesPerState[decay] = slider->getValue();
		deltaDecay = recalculateDelta(samplesPerState[decay], peakVolume, sustainVolume);
	}

	if (slider == &sliderSustain) {
		samplesPerState[sustain] = slider->getValue();
	}

	if (slider == &sliderRelease) {
		samplesPerState[release] = slider->getValue();
		deltaRelease = recalculateDelta(samplesPerState[release], sustainVolume, 0);
	}
}

float envelopeGenerator::recalculateDelta(int samples, float min, float max)
{
	return (max - min) / samples;
}


