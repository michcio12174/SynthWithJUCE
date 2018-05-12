#include "../JuceLibraryCode/JuceHeader.h"
#include "triangularWave.h"

triangularWave::triangularWave() :
	currentAngle(0.0),
	angleDelta(0.0),
	currentFrequency(0.0),
	signalGenerator()
{
}

triangularWave::~triangularWave()
{
}

void triangularWave::initialize(double sampleRate, float initialFrequency)
{
	this->sampleRate = sampleRate;
	isPrepared = true;
}

void triangularWave::processAudioBlock(const AudioSourceChannelInfo& bufferToFill, float volume, double targetFrequency)
{
	if (isPrepared) {
		double reszta;

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
					reszta = fmod(currentAngle, 1);
					buffer[sample] = 4 * volume*abs(round(reszta) - reszta) - volume;
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
					float* const buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
					reszta = fmod(currentAngle, 1);
					buffer[sample] = (4 * volume*abs(round(reszta) - reszta) - volume);
				}
				currentAngle += angleDelta;
			}
		}
	}
	else Logger::getCurrentLogger()->writeToLog("Triangular wave generator was not prepared to play.\n");
}

void triangularWave::processAudioBlock(const AudioSourceChannelInfo & bufferToFill, AudioSampleBuffer volumeBuffer, double targetFrequency)
{
	if (isPrepared) {
		const float* volumeBufferPointer = volumeBuffer.getReadPointer(0);

		double reszta;

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
					reszta = fmod(currentAngle, 1);
					buffer[sample] = 4 * volumeBufferPointer[sample] *abs(round(reszta) - reszta) - volumeBufferPointer[sample];
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
					float* const buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);
					reszta = fmod(currentAngle, 1);
					buffer[sample] = (4 * volumeBufferPointer[sample] *abs(round(reszta) - reszta) - volumeBufferPointer[sample]);
				}
				currentAngle += angleDelta;
			}
		}
	}
	else Logger::getCurrentLogger()->writeToLog("Triangular wave generator was not prepared to play.\n");
}

void triangularWave::processAudioBlock(const AudioSourceChannelInfo & bufferToFill, float volume, AudioSampleBuffer frequencyBuffer)
{
	if (isPrepared) {
		double reszta;
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
				reszta = fmod(currentAngle, 1);
				buffer[sample] = (4 * volume*abs(round(reszta) - reszta) - volume);
			}
			currentAngle += angleDelta;
		}
	}
	else Logger::getCurrentLogger()->writeToLog("Triangular wave generator was not prepared to play.\n");
}

void triangularWave::updateAngleDelta()
{
	//ile okresów na sampla
	angleDelta = currentFrequency / sampleRate;
}