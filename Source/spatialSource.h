#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "signalGenerator.h"
#include "whiteNoise.h"
#include "sineWave.h"
#include "sawtoothWave.h"
#include "rectangularWave.h"
#include "triangularWave.h"
#include "redNoise.h"
#include "audioPlayer.h"
#include "point2D.h"
#include "lowpassFilter.h"

enum effects {
	white = 1,
	red = 2,
	sine = 3,
	rectangular = 4,
	triangular = 5,
	sawtooth = 6,
	player = 7,
	wind = 8
};

class spatialSource : public Timer
{
public:
	spatialSource(int sourceType, Colour dotColour, point2D position);
	spatialSource(int sourceType, Colour dotColour, point2D initialPosition, point2D endPosition, float velocity);
    ~spatialSource();

	void initializeSource(int sourceType);
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
	void processAudioBlock(const AudioSourceChannelInfo& bufferToFill, float volume, double targetFrequency);
	void timerCallback() override;

	point2D currentPosition;
	Colour dotColour;

private:
	bool isPrepared;
	int chosenEffect;
	//timer intervl in miliseconds
	int timerInterval = 100;
	//initial position
	point2D initialPosition;
	//end position if animated
	point2D endPosition;
	//variables for animation
	bool mobile, goingUP;
	float deltaX, deltaY;
	int numberOfSteps, currentStep;
	//file to be read if audioPlayer was choosen
	File *audioFile;
	String pathToFile = "D:\\politechnika\\Sound programming\\Syntezator\\guitar-solo.wav";
	//sound sources
	ScopedPointer<audioPlayer> playerSource;
	ScopedPointer<signalGenerator> generatedSource;
	ScopedPointer<lowpassFilter> lowpassInstance;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (spatialSource)
};
