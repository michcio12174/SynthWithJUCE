#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "sineWave.h"
#include "redNoise.h"

class lowpassFilter
{
public:
    lowpassFilter();
    ~lowpassFilter();

	void initialize(int samplesPerBlockExpected, double sampleRate, double cutoffFrequency, double q);
	void processAudioBlock(const AudioSourceChannelInfo& bufferToFill);
	void setCutoffAndQ(double cutoffFrequency, double q); //used to change lowpass parameters AND recalculate coefficients
	void clear();

	//functions used to control WahWah
	void enableWahWah(int chosenGenerator, double wahWahSamples, double wahWahDeviance); //to enable
	void updateWahWah(int chosenGenerator, double wahWahSamples, double wahWahDeviance); //to change parameters
	void disableWahWah();
private:
	//used to recalculate internal parameters of filter, when overall cutoffFrequency and q should stay the same
	//for example during WahWah
	void recalculateCoefficients(double cutoffFrequency, double q); 
	void process(const AudioSourceChannelInfo& bufferToFill);
	void processWahWah(const AudioSourceChannelInfo& bufferToFill);

	double sampleRate;
	bool wasSampleRateSet, isWahWahOn;
	//lowpass variables
	//double cutoffFrequency, q;
	float s, c, a, r;
	float a0, a1, a2, b1, b2;
	double cutoffFrequency, q;
	//remembering samples and buffers
	AudioSourceChannelInfo inputBuffer;
	float sampleInput1, sampleInput2, sampleOutput1, sampleOutput2;
	//wahWah variables
	double wahWahSamples, wahWahDeviance;
	int currentGenerator;
	//sources for wah-wah
	sineWave sineWaveInstance;
	redNoise redNoiseInstance;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (lowpassFilter)
};
