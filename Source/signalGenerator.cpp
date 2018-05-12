#include "../JuceLibraryCode/JuceHeader.h"
#include "signalGenerator.h"

signalGenerator::signalGenerator():
	sampleRate(0),
	isPrepared(false)
{
}

signalGenerator::~signalGenerator()
{
}

void signalGenerator::initialize(double sampleRate, float initialFrequency)
{
	this->sampleRate = sampleRate;
	isPrepared = true;
}

void signalGenerator::processAudioBlock(const AudioSourceChannelInfo& bufferToFill, float volume, double targetFrequency)
{
	bufferToFill.clearActiveBufferRegion();
}

void signalGenerator::processAudioBlock(const AudioSourceChannelInfo & bufferToFill, AudioSampleBuffer volumeBuffer, double targetFrequency)
{
	bufferToFill.clearActiveBufferRegion();
}

void signalGenerator::processAudioBlock(const AudioSourceChannelInfo & bufferToFill, float volume, AudioSampleBuffer frequencyBuffer)
{
	bufferToFill.clearActiveBufferRegion();
}
