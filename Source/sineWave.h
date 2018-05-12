#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "signalGenerator.h"

class sineWave : public signalGenerator
{
public:
	sineWave();
	~sineWave();

	void virtual initialize(double sampleRate, float initialFrequency) override;
	void processAudioBlock(const AudioSourceChannelInfo& bufferToFill, float volume, double targetFrequency) override;
	void processAudioBlock(const AudioSourceChannelInfo& bufferToFill, AudioSampleBuffer volumeBuffer, double targetFrequency) override;
	void processAudioBlock(const AudioSourceChannelInfo& bufferToFill, float volume, AudioSampleBuffer frequencyBuffer) override;
	void updateAngleDelta();

private:
	//frequency control
	double currentAngle, angleDelta;
	double currentFrequency;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(sineWave)
};