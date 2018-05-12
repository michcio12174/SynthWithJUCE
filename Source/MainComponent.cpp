#include "../JuceLibraryCode/JuceHeader.h"
#include "signalGeneratorUI.h"
#include "audioPlayerUI.h"
#include "envelopeGenerator.h"
#include "lowpassUI.h"
#include "spatialUI.h"

class MainContentComponent : public AudioAppComponent, public KeyListener
{
public:
    MainContentComponent()
	{
		addAndMakeVisible(mainViewport);

		containerForComponents.addAndMakeVisible(signalGeneratorUIInstance1);
		//containerForComponents.addAndMakeVisible(signalGeneratorUIInstance2);
		//containerForComponents.addAndMakeVisible(signalGeneratorUIInstance3);

		containerForComponents.addAndMakeVisible(audioPlayerUIInstance);
		containerForComponents.addAndMakeVisible(envelopeGeneratorInstance);
		containerForComponents.addAndMakeVisible(lowpassUIInstance);
		containerForComponents.addAndMakeVisible(spatialUIInstance);

		mainViewport.setViewedComponent(&containerForComponents, false);

		mainMixer.addInputSource(&signalGeneratorUIInstance1, false);
		//mainMixer.addInputSource(&signalGeneratorUIInstance2, false);
		//mainMixer.addInputSource(&signalGeneratorUIInstance3, false);
		mainMixer.addInputSource(&audioPlayerUIInstance, false);
		mainMixer.addInputSource(&spatialUIInstance, false);


        setSize (1024, 800);

		this->addKeyListener(this);

		setAudioChannels(2, 2);
    }

    ~MainContentComponent()
    {
		this->removeKeyListener(this);
		shutdownAudio();
	}


	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
	{
		mainMixer.prepareToPlay(samplesPerBlockExpected, sampleRate);
		signalGeneratorUIInstance1.prepareToPlay(samplesPerBlockExpected, sampleRate);
		//signalGeneratorUIInstance2.prepareToPlay(samplesPerBlockExpected, sampleRate);
		//signalGeneratorUIInstance3.prepareToPlay(samplesPerBlockExpected, sampleRate);
		envelopeGeneratorInstance.prepareToPlay(sampleRate, KeyPress::spaceKey);
		lowpassUIInstance.prepareToPlay(samplesPerBlockExpected, sampleRate);
		spatialUIInstance.prepareToPlay(samplesPerBlockExpected, sampleRate);
	}

	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
	{
		mainMixer.getNextAudioBlock(bufferToFill);
		envelopeGeneratorInstance.processAudioBlock(bufferToFill);
		lowpassUIInstance.processAudioBlock(bufferToFill);
	}

	void releaseResources() override
	{
		mainMixer.releaseResources();
		spatialUIInstance.releaseResources();
	}

    void paint (Graphics& g) override
    {
		g.fillAll(Colours::lightblue);
		g.setColour(Colours::darkblue);
    }

    void resized() override
    {
		//get size of the whole window
		Rectangle<int> area(getLocalBounds());

		//position components
		mainViewport.setBounds(area);

		//set height to combined height of all components
		area.setHeight(160 + 160 + 160 + 80 + 160 + 160);

		containerForComponents.setBounds(area);

		signalGeneratorUIInstance1.setBounds(area.removeFromTop(160));
		//signalGeneratorUIInstance2.setBounds(area.removeFromTop(160));
		//signalGeneratorUIInstance3.setBounds(area.removeFromTop(160));

		audioPlayerUIInstance.setBounds(area.removeFromTop(80));
		envelopeGeneratorInstance.setBounds(area.removeFromTop(160));
		lowpassUIInstance.setBounds(area.removeFromTop(160));
		spatialUIInstance.setBounds(area.removeFromTop(160));
    }

	bool keyPressed(const KeyPress &key, Component *originatingComponent) override {
		int keyCode = key.getKeyCode();

		if (keyCode == KeyPress::escapeKey) {
			JUCEApplication::getInstance()->systemRequestedQuit();
			return true;
		}
		return false;
	}

private:
	Viewport mainViewport;
	Component containerForComponents;
	audioPlayerUI audioPlayerUIInstance;
	signalGeneratorUI signalGeneratorUIInstance1;
	//signalGeneratorUI signalGeneratorUIInstance2;
	//signalGeneratorUI signalGeneratorUIInstance3;
	envelopeGenerator envelopeGeneratorInstance;
	lowpassUI lowpassUIInstance;
	spatialUI spatialUIInstance;

	//this source is used to mix audio from all children components
	MixerAudioSource mainMixer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }
