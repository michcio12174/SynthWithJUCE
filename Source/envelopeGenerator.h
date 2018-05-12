#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class envelopeGenerator : public Component, public Button::Listener, public Slider::Listener, public KeyListener
{
public:
    envelopeGenerator();
    ~envelopeGenerator();

	void prepareToPlay(double sampleRate, int keyCode);
	void processAudioBlock(const AudioSourceChannelInfo& bufferToFill);
	void paint(Graphics&) override;
	void resized() override;
	void buttonClicked(Button* button) override;
	void keyDown();
	void keyUp();
	int getKeyCode();
	bool keyStateChanged(bool isKeyDown, Component * originatingComponent) override;
	bool keyPressed(const KeyPress &key, Component *originatingComponent) override;
	void sliderValueChanged(Slider * slider) override;
	float recalculateDelta(int samples, float min, float max);

private:
	bool isOn;
	//remembers whether sample rate was set and signals pressed key
	bool wasSampleRateSet, isKeyPressed;
	//key this envelope generator instance will react to
	int keyCode;
	double sampleRate;
	//variables that set how loud is peak and sustain volume
	float peakVolume, sustainVolume;
	//this signifies whether we are during attack, decay, sustain, release or silence
	int currentState;
	//how many samples schould there be for each state, used to iterate over buffer
	int samplesPerState[4], positionInTheState;
	//change per sample in each state
	float deltaAttack, deltaDecay, deltaRelease; 
	//multiplier used for previous sample
	float prevMultiplier;
	//UI
	String componentName = "EnvelopeGenerator ", componentStatus = "OFF";
	TextButton onOffButton;
	Label componentLabel;
	TextButton startButton;
	Label labelAttack, labelDecay, labelSustain, labelRelease;
	Slider sliderAttack, sliderDecay, sliderSustain, sliderRelease;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (envelopeGenerator)
};
