#include "../JuceLibraryCode/JuceHeader.h"
#include "sawtoothWave.h"

sawtoothWave::sawtoothWave() :
	currentAngle(0.0),
	angleDelta(0.0),
	currentFrequency(0.0),
	signalGenerator()
{
}

sawtoothWave::~sawtoothWave()
{
}

void sawtoothWave::initialize(double sampleRate, float initialFrequency)
{
	this->sampleRate = sampleRate;
	isPrepared = true;
}

void sawtoothWave::processAudioBlock(const AudioSourceChannelInfo& bufferToFill, float volume, double targetFrequency)
{
	if (isPrepared) {
		const double localTargetFrequency = targetFrequency;

		float* buffer;

		if (localTargetFrequency != currentFrequency)
		{
			const double frequencyIncrement = (localTargetFrequency - currentFrequency) / bufferToFill.numSamples;

			for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
			{
				currentFrequency += frequencyIncrement;
				updateAngleDelta();
				for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
				{
					// Get a pointer to the start sample in the buffer for this audio output channel
					buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
					buffer[sample] = fmod(currentAngle, 1) * 2 * volume - volume;
				}
				currentAngle += angleDelta;
			}

			currentFrequency = localTargetFrequency;
		}
		else
		{
			for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
			{
				for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
				{
					// Get a pointer to the start sample in the buffer for this audio output channel
					buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
					buffer[sample] = fmod(currentAngle, 1) * 2 * volume - volume;
				}
				currentAngle += angleDelta;
			}
		}
	}
	else Logger::getCurrentLogger()->writeToLog("Sawtooth wave generator was not prepared to play.\n");
}

void sawtoothWave::processAudioBlock(const AudioSourceChannelInfo & bufferToFill, AudioSampleBuffer volumeBuffer, double targetFrequency)
{
	if (isPrepared) {
		const float* volumeBufferPointer = volumeBuffer.getReadPointer(0);

		const double localTargetFrequency = targetFrequency;

		float* buffer;

		if (localTargetFrequency != currentFrequency)
		{
			const double frequencyIncrement = (localTargetFrequency - currentFrequency) / bufferToFill.numSamples;

			for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
			{
				currentFrequency += frequencyIncrement;
				updateAngleDelta();
				for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
				{
					// Get a pointer to the start sample in the buffer for this audio output channel
					buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
					buffer[sample] = fmod(currentAngle, 1) * 2 * volumeBufferPointer[sample] - volumeBufferPointer[sample];
				}
				currentAngle += angleDelta;
			}

			currentFrequency = localTargetFrequency;
		}
		else
		{
			for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
			{
				for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
				{
					// Get a pointer to the start sample in the buffer for this audio output channel
					buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
					buffer[sample] = fmod(currentAngle, 1) * 2 * volumeBufferPointer[sample] - volumeBufferPointer[sample];
				}
				currentAngle += angleDelta;
			}
		}
	}
	else Logger::getCurrentLogger()->writeToLog("Sawtooth wave generator was not prepared to play.\n");
}

void sawtoothWave::processAudioBlock(const AudioSourceChannelInfo & bufferToFill, float volume, AudioSampleBuffer frequencyBuffer)
{
	if (isPrepared) {
		float* buffer;
		//getting a pointer to the frequency array
		const float* frequencyBufferPointer = frequencyBuffer.getReadPointer(0);

		for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
		{
			currentFrequency = frequencyBufferPointer[sample];
			updateAngleDelta();
			for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
			{
				// Get a pointer to the start sample in the buffer for this audio output channel
				buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
				buffer[sample] = fmod(currentAngle, 1) * 2 * volume - volume;
			}
			currentAngle += angleDelta;
		}
	}
	else Logger::getCurrentLogger()->writeToLog("Sawtooth wave generator was not prepared to play.\n");
}

void sawtoothWave::updateAngleDelta()
{
	//iloœæ okresów na sampla
	angleDelta = currentFrequency / sampleRate;
}