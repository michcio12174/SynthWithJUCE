#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

enum TransportState
{
	Stopped,
	Starting,
	Playing,
	Stopping
};

class audioPlayer : public AudioSource, public ChangeListener
{
public:
	audioPlayer();
	~audioPlayer();

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;
	void changeState(TransportState newState);
	void changeListenerCallback(ChangeBroadcaster* source) override;
	TransportState getState();
	void openFile(File fileToOpen);

	float volume;
	//variables that are exposed to the UI component, so it can update itself
	//changeHappened tells it whether state changed or the file was loaded
	//enableStartButton signifies whether file was loaded ok and start button in UI schould be enabled
	bool changeHappened, enableStartButton;
private:
	AudioFormatManager formatManager;
	ScopedPointer<AudioFormatReaderSource> readerSource;
	AudioTransportSource transportSource;
	TransportState state;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(audioPlayer)
};
