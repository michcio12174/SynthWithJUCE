#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "spatialListener.h"

class spatialUI : public Component, public KeyListener, public Timer, public AudioSource
{
public:
	spatialUI();
    spatialUI(int width, int height);
    ~spatialUI();

	void initializeUI();
    void paint (Graphics&) override;
    void resized() override;
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	void releaseResources() override;
	bool keyPressed(const KeyPress &key, Component * originatingComponent) override;
	bool keyStateChanged(bool isKeyDown, Component * originatingComponent) override;
	void timerCallback() override;

	spatialListener *listenerInTheScene;

private:
	bool isPrepared;
	int width, height;
	//sources in the scene
	spatialSource *generatedMoving;
	spatialSource *generatedStationary;
	spatialSource *fromFileStationary;
	//reverb zone info - those points mark reverb zone
	point2D upperLeftReverb;
	int reverbWidth, reverbHeight;
	Reverb::Parameters reverbParameters;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (spatialUI)
};