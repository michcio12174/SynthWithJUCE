#include "../JuceLibraryCode/JuceHeader.h"
#include "spatialListener.h"

spatialListener::spatialListener(point2D bottomRightCorner):
	bottomRightCorner(bottomRightCorner),
	position(bottomRightCorner.x / 2, bottomRightCorner.y / 2),
	dotColour(Colours::red),
	isWOn(false),
	isSOn(false),
	isAOn(false),
	isDOn(false),
	isPrepared(false),
	useReverb(false)
{
	startTimer(timerInterval); //defined in .h
	reverbInstance = new ReverbAudioSource(&sourcesMixer, false);
}

spatialListener::spatialListener(point2D bottomRightCorner, Colour colour):
	bottomRightCorner(bottomRightCorner),
	position(bottomRightCorner.x / 2, bottomRightCorner.y / 2),
	dotColour(colour),
	isWOn(false),
	isSOn(false),
	isAOn(false),
	isDOn(false),
	isPrepared(false),
	useReverb(true)
{
	startTimer(timerInterval); //defined in .h
	reverbInstance = new ReverbAudioSource(&sourcesMixer, false);
}

spatialListener::~spatialListener()
{
	stopTimer();
}

void spatialListener::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	sourcesMixer.prepareToPlay(samplesPerBlockExpected, sampleRate);
	reverbInstance->prepareToPlay(samplesPerBlockExpected, sampleRate);;
	samplesPerBlockExpectedSaved = samplesPerBlockExpected;
	sampleRateSaved = sampleRate;
	isPrepared = true;
	for each (spatialHelper *helper in sourcesList)
	{
		helper->prepareToPlay(samplesPerBlockExpected, sampleRate);
	}
}

void spatialListener::getNextAudioBlock(const AudioSourceChannelInfo & bufferToFill)
{
	if (isPrepared) { 
		if (useReverb) reverbInstance->getNextAudioBlock(bufferToFill);
		else sourcesMixer.getNextAudioBlock(bufferToFill); 
	}
	else Logger::getCurrentLogger()->writeToLog("spatialListener was not prepared to play\n");
}

void spatialListener::releaseResources()
{
	sourcesMixer.releaseResources();
	reverbInstance->releaseResources();
	for each (spatialHelper *helper in sourcesList) helper->releaseResources();
	sourcesList.clear();
}

void spatialListener::timerCallback()
{
	if (isWOn && position.y > 0) position.y -= (velocity* timerInterval) / 100;
	if (isSOn && position.y < bottomRightCorner.y)  position.y += (velocity* timerInterval) / 100;
	if (isAOn && position.x > 0)  position.x -= (velocity* timerInterval) / 100;
	if (isDOn && position.x < bottomRightCorner.x)  position.x += (velocity* timerInterval) / 100;

	//update position of the listener
	for each (spatialHelper *helper in sourcesList) helper->listenerPosition = this->position;
}

void spatialListener::addSpatialSource(spatialSource &source)
{
	if (isPrepared) {
		spatialHelper *tempHelper = new spatialHelper(&source);
		tempHelper->prepareToPlay(samplesPerBlockExpectedSaved, sampleRateSaved);
		sourcesList.add(tempHelper);
		sourcesMixer.addInputSource(tempHelper, false);
	}
	else{
		Logger::getCurrentLogger()->writeToLog("prepare to play was not called before calling addSpatialSource on a spatialListener\n");
	}
}

void spatialListener::enableReverb(Reverb::Parameters parameters)
{
	useReverb = true;
	reverbInstance->setParameters(parameters);
}

void spatialListener::disableReverb()
{
	useReverb = false;
}
