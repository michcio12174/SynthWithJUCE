#include "../JuceLibraryCode/JuceHeader.h"
#include "lowpassFilter.h"

enum generators {
	sine = 1,
	red = 2
};

lowpassFilter::lowpassFilter() :
	sampleRate(0),
	wasSampleRateSet(false),
	isWahWahOn(false),
	sampleInput1(0),
	sampleInput2(0),
	sampleOutput1(0),
	sampleOutput2(0),
	cutoffFrequency(0),
	q(0),
	wahWahSamples(0),
	wahWahDeviance(0)
{
}

lowpassFilter::~lowpassFilter()
{
}

void lowpassFilter::initialize(int samplesPerBlockExpected, double sampleRate, double cutoffFrequency, double q)
{
	this->sampleRate = sampleRate;
	wasSampleRateSet = true;
	setCutoffAndQ(cutoffFrequency, q);

	//prepare generators
	sineWaveInstance.initialize(sampleRate, 0);
	redNoiseInstance.initialize(sampleRate, 0);
}

void lowpassFilter::processAudioBlock(const AudioSourceChannelInfo & bufferToFill)
{
	if (wasSampleRateSet) {
		if (isWahWahOn) processWahWah(bufferToFill);
		else process(bufferToFill);
	}
	else {
		bufferToFill.clearActiveBufferRegion();
		if (!wasSampleRateSet) Logger::getCurrentLogger()->writeToLog("prepareToPlay was not called for one of lowpass filters\n");
	}
}

void lowpassFilter::setCutoffAndQ(double cutoffFrequency, double q)
{
	this->cutoffFrequency = cutoffFrequency;
	this->q = q;

	recalculateCoefficients(cutoffFrequency, q);
}

void lowpassFilter::recalculateCoefficients(double cutoffFrequency, double q)
{
	s = sin(2 * double_Pi * cutoffFrequency / sampleRate);
	c = cos(2 * double_Pi * cutoffFrequency / sampleRate);
	if (q <= 0) a = s / (2 * std::numeric_limits<float>::min());
	else a = s / (2 * q);
	r = 1 / (1 + a);

	a0 = 0.5 * (1 - c) * r;
	a1 = (1 - c) * r;
	a2 = a0;
	b1 = -2 * c * r;
	b2 = (1 - a) * r;
}

//clears previous samples so there will be no cracks after turning filter on and off
void lowpassFilter::clear()
{
	sampleInput1 = 0, sampleInput2 = 0, sampleOutput1 = 0, sampleOutput2 = 0;
}

void lowpassFilter::enableWahWah(int chosenGenerator, double wahWahSamples, double wahWahDeviance)
{
	isWahWahOn = true;
	this->currentGenerator = chosenGenerator;
	this->wahWahSamples = wahWahSamples;
	this->wahWahDeviance = wahWahDeviance;
}

void lowpassFilter::updateWahWah(int chosenGenerator, double wahWahSamples, double wahWahDeviance)
{
	this->currentGenerator = chosenGenerator;
	this->wahWahSamples = wahWahSamples;
	this->wahWahDeviance = wahWahDeviance;
}

void lowpassFilter::disableWahWah()
{
	isWahWahOn = false;
}

void lowpassFilter::process(const AudioSourceChannelInfo & bufferToFill)
{
	//remembering the unaltered state of the buffer (input)
	inputBuffer = bufferToFill;
	float* inputBufferPointer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);

	float tempSample;

	//for each sample in buffer
	for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
	{
		tempSample = a0 * inputBufferPointer[sample] + a1 * sampleInput1 + a2 * sampleInput2 - b1 * sampleOutput1 - b2 * sampleOutput2;

		for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
		{
			// Get a pointer to the start sample in the buffer for this audio output channel
			float* buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
			buffer[sample] = tempSample;
		}
		sampleInput2 = sampleInput1;
		sampleInput1 = inputBufferPointer[sample];

		sampleOutput2 = sampleOutput1;
		sampleOutput1 = tempSample;
	}
}

//TODO czêstotliwoœæ lowpasa na ucho nie zgadza siê z czêstotliwoœcia na sliderze
void lowpassFilter::processWahWah(const AudioSourceChannelInfo & bufferToFill)
{
	//here we will hold frequencies for each sample
	AudioSampleBuffer frequencyBuffer(1, bufferToFill.numSamples);
	AudioSourceChannelInfo frequencyChannel(frequencyBuffer);

	//to red noise we pass the wahWahSamples as number of samples between random samples
	//we get values from -1 to 1
	if (currentGenerator == red) redNoiseInstance.processAudioBlock(frequencyChannel, 1, wahWahSamples);
	else sineWaveInstance.processAudioBlock(frequencyChannel, 1, sampleRate / wahWahSamples);

	//we scale those values using base frequency and chosen deviance
	float* frequencyBufferPointer = frequencyBuffer.getWritePointer(0);

	for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
		frequencyBufferPointer[sample] = frequencyBufferPointer[sample] * wahWahDeviance * cutoffFrequency + cutoffFrequency;

	//remembering the unaltered state of the buffer (input)
	inputBuffer = bufferToFill;
	float* inputBufferPointer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);

	float tempSample;

	//for each sample in buffer
	for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
	{
		//w tej wersji dostajemy tablicê zmiennych czêstotliwoœci, wiêc przeliczamy za ka¿ym razem
		//assuming the array is always the same length as buffer
		recalculateCoefficients(frequencyBufferPointer[sample], q);

		tempSample = a0 * inputBufferPointer[sample] + a1 * sampleInput1 + a2 * sampleInput2 - b1 * sampleOutput1 - b2 * sampleOutput2;

		for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
		{
			// Get a pointer to the start sample in the buffer for this audio output channel
			float* buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
			buffer[sample] = tempSample;
		}
		sampleInput2 = sampleInput1;
		sampleInput1 = inputBufferPointer[sample];

		sampleOutput2 = sampleOutput1;
		sampleOutput1 = tempSample;
	}
}
