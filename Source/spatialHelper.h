#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "spatialSource.h"

class spatialHelper : public AudioSource
{
public:
    spatialHelper(spatialSource *managedSource);

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;

	point2D listenerPosition;

private:
	bool isPrepared;
	spatialSource *managedSource;
	float currentVolume;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (spatialHelper)
};
