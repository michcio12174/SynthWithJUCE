#include "../JuceLibraryCode/JuceHeader.h"
#include "whiteNoise.h"

whiteNoise::whiteNoise():
	signalGenerator()
{
}

whiteNoise::~whiteNoise()
{
}


void whiteNoise::initialize(double sampleRate, float initialFrequency)
{
	this->sampleRate = sampleRate;
	isPrepared = true;
}

void whiteNoise::processAudioBlock(const AudioSourceChannelInfo& bufferToFill, float volume, double targetFrequency)
{
	if (isPrepared) {
		const float volumeScale = volume * 2.0f;

		for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
		{
			// Get a pointer to the start sample in the buffer for this audio output channel
			float* const buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);

			// Fill the required number of samples with noise
			for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
				buffer[sample] = random.nextFloat() * volumeScale - volume;
		}
	}
	else Logger::getCurrentLogger()->writeToLog("White noise generator was not prepared to play.\n");
}

void whiteNoise::processAudioBlock(const AudioSourceChannelInfo & bufferToFill, AudioSampleBuffer volumeBuffer, double targetFrequency)
{
	if (isPrepared) {
		const float* volumeBufferPointer = volumeBuffer.getReadPointer(0);

		for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
		{
			// Get a pointer to the start sample in the buffer for this audio output channel
			float* const buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);

			// Fill the required number of samples with noise
			for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
				buffer[sample] = random.nextFloat() * (volumeBufferPointer[sample] * 2.0f) - volumeBufferPointer[sample];
		}
	}
	else Logger::getCurrentLogger()->writeToLog("White noise generator was not prepared to play.\n");
}

void whiteNoise::processAudioBlock(const AudioSourceChannelInfo & bufferToFill, float volume, AudioSampleBuffer frequencyBuffer)
{
	if (isPrepared) {
		const float volumeScale = volume * 2.0f;

		for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
		{
			// Get a pointer to the start sample in the buffer for this audio output channel
			float* const buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);

			// Fill the required number of samples with noise
			for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
				buffer[sample] = random.nextFloat() * volumeScale - volume;
		}
	}
	else Logger::getCurrentLogger()->writeToLog("White noise generator was not prepared to play.\n");
}
