#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class signalGenerator
{
public:
	signalGenerator();
	~signalGenerator();

	void virtual initialize(double sampleRate, float initialFrequency);
	void virtual processAudioBlock(const AudioSourceChannelInfo& bufferToFill, float volume, double targetFrequency);
	void virtual processAudioBlock(const AudioSourceChannelInfo& bufferToFill, AudioSampleBuffer volumeBuffer, double targetFrequency);
	void virtual processAudioBlock(const AudioSourceChannelInfo& bufferToFill, float volume, AudioSampleBuffer frequencyBuffer);

protected:
	double sampleRate;
	bool isPrepared;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(signalGenerator)
};