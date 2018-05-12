#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "audioPlayer.h"

class audioPlayerUI : public Component, public Button::Listener, public AudioSource, public Timer
{
public:
    audioPlayerUI();
    ~audioPlayerUI();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;
	void paint(Graphics&) override;
	void resized() override;
	void buttonClicked(Button* button) override;
	void timerCallback() override;
private:
	//UI
	TextButton startButton, openFileButton;
	String componentName = "Audio player";
	Label componentLabel;
	Label statusLabel;
	Slider volumeSlider;
	Label volumeSliderLabel;

	bool isOn;
	//other
	float volume;
	//player that will produce sound for UI component
	audioPlayer playerInstance;
	//the state of audioPlayer needed to deduce in timer callback 
	//whether interface should change in response to change in the player
	TransportState state;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (audioPlayerUI)
};
