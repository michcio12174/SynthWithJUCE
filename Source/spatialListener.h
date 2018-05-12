#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "spatialHelper.h"

class spatialListener : public Timer, public AudioSource
{
public:
	spatialListener(point2D bottomRightCorner);
	spatialListener(point2D bottomRightCorner, Colour colour);
    ~spatialListener();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;
	void timerCallback() override;
	void addSpatialSource(spatialSource &source);
	void enableReverb(Reverb::Parameters parameters);
	void disableReverb();

	//key variables
	bool isWOn, isSOn, isAOn, isDOn;
	Colour dotColour;
	//border of plane on which listener walks
	point2D bottomRightCorner;
	//coordinates of the listener
	point2D position;

private:
	int timerInterval = 100;
	//we need to remember those, as spatial helpers can be added to listener after prepare to play was called
	//and we still need to prepare them
	int samplesPerBlockExpectedSaved;
	double sampleRateSaved;
	//we need to know if listener was prepared and knows two values above
	//it will need to pass them to each added helper
	bool isPrepared;
	//character velocity in m/s
	float velocity = 2;
	//here all sources are being held
	Array<spatialHelper*> sourcesList;
	//mixer of audio sources
	MixerAudioSource sourcesMixer;
	//variables related to reverb
	ScopedPointer<ReverbAudioSource> reverbInstance;
	bool useReverb;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (spatialListener)
};
