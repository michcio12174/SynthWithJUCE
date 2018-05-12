#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "lowpassFilter.h"

class lowpassUI : public Component, public Button::Listener, public ComboBox::Listener, public Slider::Listener
{
public:
    lowpassUI();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
	void processAudioBlock(const AudioSourceChannelInfo& bufferToFill);
	void paint(Graphics&) override;
	void resized() override;
	void buttonClicked(Button* button) override;
	void sliderValueChanged(Slider* slider) override;
	void comboBoxChanged(ComboBox *comboBox) override;

private:
	//lowpass backend
	lowpassFilter lowpassFilterInstance;
	bool isOn, isWahWahOn;
	//lowpass UI
	float minFrequency, maxFrequency, defaultFrequency;
	String componentName = "Lowpass filter", componentStatus = " OFF";
	Label componentLabel;
	ToggleButton startButton;
	Label labelCutoff, labelQ;
	Slider sliderCutoff, sliderQ;

	//wah-wah UI
	Label labelWahWah, labelWahWahStatus;
	String wahWahStatus = " OFF";
	ToggleButton buttonWahWah;
	Slider sliderWahWahSamples;
	float minWahWahSamples, maxWahWahSamples;
	//choice of wah-wah sources
	ComboBox comboBoxWahWah;
	int currentWahWahGenerator;
	//deviance from base frequency 
	Label labelWahWahDeviance;
	Slider sliderDeviance;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (lowpassUI)
};
