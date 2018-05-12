#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "signalGenerator.h"

//dla u³atwienia obliczeñ okres to 1
class rectangularWave : public signalGenerator
{
public:
	rectangularWave();
	~rectangularWave();

	void virtual initialize(double sampleRate, float initialFrequency) override;
	void processAudioBlock(const AudioSourceChannelInfo& bufferToFill, float volume, double targetFrequency) override;
	void processAudioBlock(const AudioSourceChannelInfo& bufferToFill, AudioSampleBuffer volumeBuffer, double targetFrequency) override;
	void processAudioBlock(const AudioSourceChannelInfo& bufferToFill, float volume, AudioSampleBuffer frequencyBuffer) override;
	void updateAngleDelta();

private:
	//frequency control
	double currentAngle, angleDelta;
	double currentFrequency;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(rectangularWave)
};