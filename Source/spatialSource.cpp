#include "../JuceLibraryCode/JuceHeader.h"
#include "spatialSource.h"

spatialSource::spatialSource(int sourceType, Colour dotColour, point2D position):
	dotColour(dotColour),
	currentPosition(position),
	isPrepared(false),
	mobile(false)
{
	//timer used to change position and listen if we need to testart audio
	startTimer(timerInterval);

	initializeSource(sourceType);
}

spatialSource::spatialSource(int sourceType, Colour dotColour, point2D initialPosition, point2D endPosition, float velocity) :
	dotColour(dotColour),
	currentPosition(initialPosition),
	initialPosition(initialPosition),
	endPosition(endPosition),
	isPrepared(false),
	mobile(true)
{
	//timer used to change position and listen if we need to testart audio
	startTimer(timerInterval);

	//caculate time it will take to go along the road
	//length of the road in meters
	float d = initialPosition.distance(endPosition);
	//time to cross that road
	float t = d / velocity;
	//number of timer intervals to traverse the road
	numberOfSteps = (t * 1000) / timerInterval;
	currentStep = 0;
	//calculating position delta per step
	deltaX = (endPosition.x - initialPosition.x) / numberOfSteps;
	deltaY = (endPosition.y - initialPosition.y) / numberOfSteps;

	goingUP = true;

	initializeSource(sourceType);
}

spatialSource::~spatialSource()
{
}

void spatialSource::initializeSource(int sourceType)
{
	chosenEffect = sourceType;

	switch (sourceType) {
	case red:
		generatedSource = new redNoise();
		break;
	case sine:
		generatedSource = new sineWave();
		break;
	case rectangular:
		generatedSource = new rectangularWave();
		break;
	case triangular:
		generatedSource = new triangularWave();
		break;
	case sawtooth:
		generatedSource = new sawtoothWave();
		break;
	case player:
		playerSource = new audioPlayer();
		break;
	case wind:
		generatedSource = new whiteNoise();
		lowpassInstance = new lowpassFilter();
		break;
	default:
		generatedSource = new whiteNoise();
		break;
	}

	if (chosenEffect == player) {
		audioFile = new File(pathToFile);
		if (audioFile->exists()) playerSource->openFile(*audioFile);
		else Logger::getCurrentLogger()->writeToLog("spatial source didn't find requeested file\n");
	}
}

void spatialSource::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	if (chosenEffect == player) {
		playerSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
		playerSource->changeState(Starting);
	}
	else if (chosenEffect == wind) {
		generatedSource->initialize(sampleRate, 0);
		lowpassInstance->initialize(samplesPerBlockExpected, sampleRate, 2000.0, 6.0);
		lowpassInstance->enableWahWah(2, 50000.0, 0.5);
	}
	else { 
		generatedSource->initialize(sampleRate, 0);

	}

	isPrepared = true;
}

void spatialSource::processAudioBlock(const AudioSourceChannelInfo& bufferToFill, float volume, double targetFrequency)
{
	if (isPrepared) {
		if (chosenEffect == wind) {
			generatedSource->processAudioBlock(bufferToFill, volume, targetFrequency);
			lowpassInstance->processAudioBlock(bufferToFill);
		}

		else if (chosenEffect == player) {
			playerSource->volume = volume;
			playerSource->getNextAudioBlock(bufferToFill);
		}

		else generatedSource->processAudioBlock(bufferToFill, volume, targetFrequency);
	}
	else Logger::getCurrentLogger()->writeToLog("spatialSource was not prepared to play\n");
}

void spatialSource::timerCallback()
{
	if (mobile) {
		if (currentStep >= numberOfSteps) {
			currentStep = 0;
			goingUP = !goingUP;
		}

		if (goingUP) {
			currentPosition.x += deltaX;
			currentPosition.y += deltaY;
		}
		else {
			currentPosition.x -= deltaX;
			currentPosition.y -= deltaY;
		}
		++currentStep;
	}

	//naive way to loop audio
	if (chosenEffect == player && playerSource->getState() == Stopped) playerSource->changeState(Starting);
}