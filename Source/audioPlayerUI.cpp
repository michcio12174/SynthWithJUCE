#include "../JuceLibraryCode/JuceHeader.h"
#include "audioPlayerUI.h"

audioPlayerUI::audioPlayerUI():
	isOn(false)
{
	addAndMakeVisible(componentLabel);
	componentLabel.setText(componentName, dontSendNotification);
	componentLabel.setJustificationType(Justification::centred);

	addAndMakeVisible(startButton);
	startButton.setButtonText("Start");
	startButton.addListener(this);
	startButton.setEnabled(false);

	addAndMakeVisible(openFileButton);
	openFileButton.setButtonText("Open file");
	openFileButton.addListener(this);

	addAndMakeVisible(statusLabel);
	statusLabel.setText("OFF", dontSendNotification);
	statusLabel.setColour(Label::textColourId, Colours::red);
	statusLabel.setJustificationType(Justification::centred);

	addAndMakeVisible(volumeSlider);
	volumeSlider.setRange(0, 1);
	volumeSlider.setValue(0.5, sendNotification);
	volumeSlider.setSliderStyle(Slider::LinearBar);

	addAndMakeVisible(volumeSliderLabel);
	volumeSliderLabel.setText("Volume:", dontSendNotification);
	volumeSliderLabel.setJustificationType(Justification::centred);

	startTimer(200);
}

audioPlayerUI::~audioPlayerUI()
{
}

void audioPlayerUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	playerInstance.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void audioPlayerUI::getNextAudioBlock(const AudioSourceChannelInfo & bufferToFill)
{
	playerInstance.volume = volumeSlider.getValue();
	playerInstance.getNextAudioBlock(bufferToFill);
}

void audioPlayerUI::releaseResources()
{
	playerInstance.releaseResources();
}

void audioPlayerUI::paint(Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

	g.setColour(Colours::grey);
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
}

void audioPlayerUI::resized()
{
	componentLabel.setBounds(0, getHeight()*0.1, getWidth(), getHeight()*0.4);
	startButton.setBounds(10, getHeight()*0.5, 100, getHeight()*0.4);
	statusLabel.setBounds(120, getHeight()*0.5, 50, getHeight()*0.4);
	openFileButton.setBounds(180, getHeight()*0.5, 100, getHeight()*0.4);
	//volume
	volumeSliderLabel.setBounds(290, getHeight()*0.5, 80, getHeight()*0.4);
	volumeSlider.setBounds(380, getHeight()*0.5, 200, getHeight()*0.4);
}

void audioPlayerUI::buttonClicked(Button* button)
{
	if (button == &startButton) {
		if (!isOn) {
			playerInstance.changeState(Starting);
		}
		else {
			playerInstance.changeState(Stopping);
		}
		isOn = !isOn;
	}

	if (button == &openFileButton) {
		FileChooser chooser("Select a file to play", File::nonexistent, "*.wav");

		if (chooser.browseForFileToOpen()) playerInstance.openFile(chooser.getResult());
	}
}

void audioPlayerUI::timerCallback()
{
	if (playerInstance.changeHappened) {
		if(playerInstance.enableStartButton) startButton.setEnabled(true);
		else startButton.setEnabled(false);
		
		playerInstance.changeHappened = false;
	}

	//we update state based on the state of the source 
	//and set the source volume based on volume slider
	if (state != playerInstance.getState()) {
		state = playerInstance.getState();
		
		if (state == Playing) {
			startButton.setButtonText("Stop");
			statusLabel.setText("Playing", dontSendNotification);
			statusLabel.setColour(Label::textColourId, Colours::green);
		}
		if (state == Stopped) {
			startButton.setButtonText("Start");
			statusLabel.setText("Stopped", dontSendNotification);
			statusLabel.setColour(Label::textColourId, Colours::red);
		}
	}
}
