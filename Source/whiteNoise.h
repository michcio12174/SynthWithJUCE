#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "signalGenerator.h"

class whiteNoise : public signalGenerator
{
public:
    whiteNoise();
    ~whiteNoise();

	void virtual initialize(double sampleRate, float initialFrequency) override;
	void processAudioBlock(const AudioSourceChannelInfo& bufferToFill, float volume, double targetFrequency) override;
	void processAudioBlock(const AudioSourceChannelInfo& bufferToFill, AudioSampleBuffer volumeBuffer, double targetFrequency) override;
	void processAudioBlock(const AudioSourceChannelInfo& bufferToFill, float volume, AudioSampleBuffer frequencyBuffer) override;

private:
	//random number generator for noise
	Random random; 

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(whiteNoise)
};
