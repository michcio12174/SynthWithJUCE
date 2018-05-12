#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "signalGenerator.h"

class redNoise : public signalGenerator
{
public:
	redNoise();
	~redNoise();

	void virtual initialize(double sampleRate, float initialFrequency) override;
	void processAudioBlock(const AudioSourceChannelInfo& bufferToFill, float volume, double targetSamplesPerCycle) override;
	void processAudioBlock(const AudioSourceChannelInfo& bufferToFill, AudioSampleBuffer volumeBuffer, double targetSamplesPerCycle) override;
	void processAudioBlock(const AudioSourceChannelInfo& bufferToFill, float volume, AudioSampleBuffer frequencyBuffer) override;
	void recalculate(float volume, double targetFrequency);

private:
	//frequency control
	float nextRandomSample, deltaPerSample, prevSample;
	int samplesPerCycle, positionInTheCycle;
	//random number generator for noise
	Random random;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(redNoise)
};
