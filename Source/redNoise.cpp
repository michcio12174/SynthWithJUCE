#include "../JuceLibraryCode/JuceHeader.h"
#include "redNoise.h"

redNoise::redNoise () :
	nextRandomSample(0), 
	samplesPerCycle(0),
	positionInTheCycle(0), 
	prevSample(0),
	deltaPerSample(0),
	signalGenerator()
{
}

redNoise::~redNoise()
{
}

void redNoise::initialize(double sampleRate, float initialFrequency)
{
	this->sampleRate = sampleRate;
	isPrepared = true;
	recalculate(0.5, initialFrequency);
}

void redNoise::processAudioBlock(const AudioSourceChannelInfo& bufferToFill, float volume, double targetSamplesPerCycle)
{
	if (isPrepared) {
		float tempSample;
		for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
		{
			tempSample = prevSample + deltaPerSample;

			if (positionInTheCycle >= samplesPerCycle) {
				positionInTheCycle = 0;
				recalculate(volume, targetSamplesPerCycle);
			}

			for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
			{
				// Get a pointer to the start sample in the buffer for this audio output channel
				float* buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
				buffer[sample] = tempSample;
			}
			++positionInTheCycle;
			prevSample = tempSample;
		}
	}
	else Logger::getCurrentLogger()->writeToLog("Red noise generator was not prepared to play.\n");
}

void redNoise::processAudioBlock(const AudioSourceChannelInfo & bufferToFill, AudioSampleBuffer volumeBuffer, double targetSamplesPerCycle)
{
	if (isPrepared) {
		const float* volumeBufferPointer = volumeBuffer.getReadPointer(0);

		float tempSample;
		for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
		{
			tempSample = prevSample + deltaPerSample;

			if (positionInTheCycle >= samplesPerCycle) {
				positionInTheCycle = 0;
				recalculate(volumeBufferPointer[sample], targetSamplesPerCycle);
			}

			for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
			{
				// Get a pointer to the start sample in the buffer for this audio output channel
				float* buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
				buffer[sample] = tempSample;
			}
			++positionInTheCycle;
			prevSample = tempSample;
		}
	}
	else Logger::getCurrentLogger()->writeToLog("Red noise generator was not prepared to play.\n");
}

void redNoise::processAudioBlock(const AudioSourceChannelInfo & bufferToFill, float volume, AudioSampleBuffer frequencyBuffer)
{
	if (isPrepared) {
		float tempSample;

		//this class controlls frequency using number of samples per cycle, 
		//not directly using frequency value - conversion is necessary
		float* frequencyBufferPointer = frequencyBuffer.getWritePointer(0);
		for (int sample = 0; sample < bufferToFill.numSamples; ++sample) {

			frequencyBufferPointer[sample] = sampleRate / frequencyBufferPointer[sample];
		}

		for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
		{
			tempSample = prevSample + deltaPerSample;

			if (positionInTheCycle >= samplesPerCycle) {
				positionInTheCycle = 0;
				recalculate(volume, sampleRate / frequencyBufferPointer[sample]);
			}

			for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
			{
				// Get a pointer to the start sample in the buffer for this audio output channel
				float* buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
				buffer[sample] = tempSample;
			}
			++positionInTheCycle;
			prevSample = tempSample;
		}
	}
	else Logger::getCurrentLogger()->writeToLog("Red noise generator was not prepared to play.\n");
}

void redNoise::recalculate(float volume, double targetSamplesPerCycle)
{
	//generate next sample as non-temp, global sample was set just before calling this function
	//as first sample at a begining of current cycle
	float tempNextRandomSample = random.nextFloat() * volume * 2.0 - volume;

	//calculate difference between samples in the cycle
	if(targetSamplesPerCycle != 0) samplesPerCycle = (int)targetSamplesPerCycle;
	else samplesPerCycle = 1;
	deltaPerSample = (tempNextRandomSample - nextRandomSample) / samplesPerCycle;

	nextRandomSample = tempNextRandomSample;			
}
