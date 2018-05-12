#include "spatialHelper.h"

spatialHelper::spatialHelper(spatialSource *managedSource):
managedSource(managedSource),
isPrepared(false)
{
	listenerPosition = point2D();
}

void spatialHelper::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	managedSource->prepareToPlay(samplesPerBlockExpected, sampleRate);

	isPrepared = true;
}

void spatialHelper::getNextAudioBlock(const AudioSourceChannelInfo & bufferToFill)
{
	if (isPrepared) {
		float distance = listenerPosition.distance(managedSource->currentPosition);
		if(distance > 1) currentVolume = 1 / listenerPosition.distance(managedSource->currentPosition);
		else currentVolume = 1;

		managedSource->processAudioBlock(bufferToFill, currentVolume, 300.0);
	}
	else Logger::getCurrentLogger()->writeToLog("spatialHelper was not prepared to play\n");
	
}

void spatialHelper::releaseResources()
{
}
