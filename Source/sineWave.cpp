#include "../JuceLibraryCode/JuceHeader.h"
#include "sineWave.h"

sineWave::sineWave() :
	currentAngle(0.0), 
	angleDelta(0.0),
	currentFrequency(0.0),
	signalGenerator()
{
}

sineWave::~sineWave()
{
}

void sineWave::initialize(double sampleRate, float initialFrequency)
{
	this->sampleRate = sampleRate;
	isPrepared = true;
}

void sineWave::processAudioBlock(const AudioSourceChannelInfo& bufferToFill, float volume, double targetFrequency)
{
	if (isPrepared) {
		float* buffer;

		if (targetFrequency != currentFrequency)
		{
			const double frequencyIncrement = (targetFrequency - currentFrequency) / bufferToFill.numSamples;

			for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
			{
				currentFrequency += frequencyIncrement;
				updateAngleDelta();
				for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
				{
					// Get a pointer to the start sample in the buffer for this audio output channel
					buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
					buffer[sample] = (float)std::sin(currentAngle) * volume;
				}
				currentAngle += angleDelta;
			}

			currentFrequency = targetFrequency;
		}
		else
		{
			for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
			{
				for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
				{
					// Get a pointer to the start sample in the buffer for this audio output channel
					buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
					buffer[sample] = (float)std::sin(currentAngle) * volume;
				}
				currentAngle += angleDelta;
			}
		}
	}
	else Logger::getCurrentLogger()->writeToLog("Sine wave generator was not prepared to play.\n");
}

void sineWave::processAudioBlock(const AudioSourceChannelInfo & bufferToFill, AudioSampleBuffer volumeBuffer, double targetFrequency)
{
	if (isPrepared) {
		float* buffer;

		const float* volumeBufferPointer = volumeBuffer.getReadPointer(0);

		if (targetFrequency != currentFrequency)
		{
			const double frequencyIncrement = (targetFrequency - currentFrequency) / bufferToFill.numSamples;

			for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
			{
				currentFrequency += frequencyIncrement;
				updateAngleDelta();
				for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
				{
					// Get a pointer to the start sample in the buffer for this audio output channel
					buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
					buffer[sample] = (float)std::sin(currentAngle) * volumeBufferPointer[sample];
				}
				currentAngle += angleDelta;
			}

			currentFrequency = targetFrequency;
		}
		else
		{
			for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
			{
				for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
				{
					// Get a pointer to the start sample in the buffer for this audio output channel
					buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
					buffer[sample] = (float)std::sin(currentAngle) * volumeBufferPointer[sample];
				}
				currentAngle += angleDelta;
			}
		}
	}
	else Logger::getCurrentLogger()->writeToLog("Sine wave generator was not prepared to play.\n");
}

void sineWave::processAudioBlock(const AudioSourceChannelInfo & bufferToFill, float volume, AudioSampleBuffer frequencyBuffer)
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
				buffer[sample] = (float)std::sin(currentAngle) * volume;
			}
			currentAngle += angleDelta;
		}
	}
	else Logger::getCurrentLogger()->writeToLog("Sine wave generator was not prepared to play.\n");
}

void sineWave::updateAngleDelta()
{
	//liczę ile okresów jest na sampla, jeśli niecały, to co sampla będę zmieniał o mniej niż 2*pi
	const double cyclesPerSample = currentFrequency / sampleRate;
	angleDelta = cyclesPerSample * 2.0 * double_Pi;
}