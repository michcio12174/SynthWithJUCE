#include "../JuceLibraryCode/JuceHeader.h"
#include "audioPlayer.h"

audioPlayer::audioPlayer():
	state(Stopped),
	enableStartButton(false),
	changeHappened(false),
	volume(1)
{
	//przygotowanie do czytania standardowych formatów
	formatManager.registerBasicFormats();

	transportSource.addChangeListener(this);
}

audioPlayer::~audioPlayer()
{
	releaseResources();
}

void audioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void audioPlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	if(readerSource == nullptr) bufferToFill.clearActiveBufferRegion();
	else { 
		transportSource.setGain(volume);
		transportSource.getNextAudioBlock(bufferToFill); 
	}
}

void audioPlayer::releaseResources()
{
	transportSource.releaseResources();
}

void audioPlayer::changeState(TransportState newState)
{
	if (state != newState) {
		state = newState;

		//in Playing state we only change UI - not responsibility of this component
		switch (state) {
			case Stopped:
				transportSource.setPosition(0.0);
				changeHappened = true;
				break;

			case Starting:
				transportSource.start();
				break;

			case Playing:
				transportSource.start();
				changeHappened = true;
				break;

			case Stopping:
				transportSource.stop();
				break;
		}
	}
}

void audioPlayer::changeListenerCallback(ChangeBroadcaster* source)
{
	if (source == &transportSource)
	{
		if (transportSource.isPlaying())
			changeState(Playing);
		else
			changeState(Stopped);
	}
}

TransportState audioPlayer::getState()
{
	return state;
}

void audioPlayer::openFile(File fileToOpen)
{
	AudioFormatReader* reader = formatManager.createReaderFor(fileToOpen);

	if (reader != nullptr) {
		ScopedPointer<AudioFormatReaderSource> newSource = new AudioFormatReaderSource(reader, true);
		transportSource.setSource(newSource, 0, nullptr, reader->sampleRate);
		readerSource = newSource.release();

		changeHappened = true;
		enableStartButton = true;
	}
}
