#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "signalGenerator.h"
#include "whiteNoise.h"
#include "sineWave.h"
#include "sawtoothWave.h"
#include "rectangularWave.h"
#include "triangularWave.h"
#include "redNoise.h"

//dla ułatwienia obliczeń okres to 1
class signalGeneratorUI : public AudioSource, public Component, public Button::Listener, public Slider::Listener, public ComboBox::Listener
{
public:
	signalGeneratorUI();
	~signalGeneratorUI();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;
	void fillBuffer(const AudioSourceChannelInfo& bufferToFill);
	void fillBufferVibrato(const AudioSourceChannelInfo& bufferToFill);
	void fillBufferTremolo(const AudioSourceChannelInfo& bufferToFill);
	void paint(Graphics&) override;
	void resized() override;
	void buttonClicked(Button* button) override;
	void sliderValueChanged(Slider* slider) override;
	void comboBoxChanged(ComboBox *comboBox) override;
	void frequencySliderForFrequency();
	void frequencySliderForSamplesPerCycle();

private:
	ToggleButton startButton;
	String componentName = "Sound generator", componentStatus = " OFF";
	Label componentLabel;
	bool isOn, isTremoloOn, isVibratoOn;
	//UI for vibrato and tremolo
	Label labelTremolo, labelVibrato;
	ToggleButton buttonVibrato, buttonTremolo;
	Slider sliderVibratoFrequency, sliderVibratoDeviance,
		sliderTremoloFrequency, sliderTremoloDeviance;
	Label labelVibratoFrequency, labelVibratoDeviance, 
		labelTremoloFrequency, labelTremoloDeviance;
	//RNG for noise
	Random random;
	//choice of generators
	ComboBox generatorsComboBox;
	//generator instances
	int currentGeneratorChoice;
	whiteNoise whiteNoiseInstance;
	redNoise redNoiseInstance;
	sineWave sineWaveInstance;
	rectangularWave rectangularWaveInstance;
	triangularWave triangularWaveInstance;
	sawtoothWave sawtoothWaveInstance;
	sineWave vibratoAndTremoloGenerator;
	//volume control
	Slider volumeSlider;
	Label volumeSliderLabel;
	//frequency control
	Slider frequencySlider;
	Label frequencySliderLabel;
	float defaultFrequency, minFrequency, maxFrequency;
	float minSamples, maxSamples;
	int defaultSamples;
	bool wasPrevousSliderFrequency;
	//information for generators
	float volume;
	double targetFrequency;
	double currentSampleRate;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(signalGeneratorUI)
};