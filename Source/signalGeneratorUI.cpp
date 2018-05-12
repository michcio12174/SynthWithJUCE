#include "../JuceLibraryCode/JuceHeader.h"
#include "signalGeneratorUI.h"

enum {
	noGenerator = -1,
	white = 1,
	red = 2,
	sine = 3,
	rectangular = 4,
	triangular = 5,
	sawtooth = 6
};

signalGeneratorUI::signalGeneratorUI() :
	currentSampleRate(0.0),
	defaultFrequency(500.0),
	minFrequency(50.0),
	maxFrequency(5000.0),
	defaultSamples(10),
	minSamples(1),
	maxSamples(500),
	isOn(false),
	isVibratoOn(false),
	isTremoloOn(false)
{
	addAndMakeVisible(componentLabel);
	componentLabel.setText(componentName + componentStatus, dontSendNotification);
	componentLabel.setColour(Label::textColourId, Colours::red);
	componentLabel.setJustificationType(Justification::centred);

	addAndMakeVisible(startButton);
	startButton.setButtonText("Turn on");
	startButton.addListener(this);

	//combo box for choice
	addAndMakeVisible(generatorsComboBox);
	generatorsComboBox.addListener(this);
	generatorsComboBox.addItem("No generator", noGenerator);
	generatorsComboBox.addItem("White noise", white);
	generatorsComboBox.addItem("Red noise", red);
	generatorsComboBox.addItem("Sine wave", sine);
	generatorsComboBox.addItem("Square wave", rectangular);
	generatorsComboBox.addItem("Triangular wave", triangular);
	generatorsComboBox.addItem("Sawtooth wave", sawtooth);
	generatorsComboBox.setSelectedId(noGenerator, dontSendNotification);

	//volume
	addAndMakeVisible(volumeSlider);
	volumeSlider.setRange(0, 1);
	volumeSlider.setValue(0.5, sendNotification);
	volumeSlider.setSliderStyle(Slider::LinearBar);
	volumeSlider.addListener(this);

	addAndMakeVisible(volumeSliderLabel);
	volumeSliderLabel.setText("Volume:", dontSendNotification);
	volumeSliderLabel.setJustificationType(Justification::centred);

	//frequency
	addAndMakeVisible(frequencySlider);
	frequencySlider.setRange(minFrequency, maxFrequency);
	frequencySlider.setSkewFactorFromMidPoint(500.0);
	frequencySlider.setValue(defaultFrequency, dontSendNotification);
	frequencySlider.setSliderStyle(Slider::LinearBar);
	frequencySlider.setTextValueSuffix("Hz");
	frequencySlider.addListener(this);

	addAndMakeVisible(frequencySliderLabel);
	frequencySliderLabel.setText("Frequency:", dontSendNotification);
	frequencySliderLabel.setJustificationType(Justification::centred);

	targetFrequency = defaultFrequency;

	//vibrato
	addAndMakeVisible(labelVibrato);
	labelVibrato.setText("Vibrato:", dontSendNotification);
	labelVibrato.setJustificationType(Justification::left);

	addAndMakeVisible(buttonVibrato);
	buttonVibrato.setButtonText("Turn on");
	buttonVibrato.addListener(this);

	addAndMakeVisible(labelVibratoFrequency);
	labelVibratoFrequency.setText("Frequency:", dontSendNotification);
	labelVibratoFrequency.setJustificationType(Justification::left);
	labelVibratoFrequency.attachToComponent(&sliderVibratoFrequency, true);

	addAndMakeVisible(sliderVibratoFrequency);
	sliderVibratoFrequency.setRange(0.0001, 5000.0);
	sliderVibratoFrequency.setValue(100.0, dontSendNotification);
	sliderVibratoFrequency.setSkewFactorFromMidPoint(500.0);
	sliderVibratoFrequency.setTextValueSuffix("Hz");
	sliderVibratoFrequency.setSliderStyle(Slider::LinearBar);
	sliderVibratoFrequency.addListener(this);

	addAndMakeVisible(labelVibratoDeviance);
	labelVibratoDeviance.setText("Deviance:", dontSendNotification);
	labelVibratoDeviance.setJustificationType(Justification::left);
	labelVibratoDeviance.attachToComponent(&sliderVibratoDeviance, true);

	addAndMakeVisible(sliderVibratoDeviance);
	sliderVibratoDeviance.setRange(0.0, 1.0);
	sliderVibratoDeviance.setValue(0.5, dontSendNotification);
	sliderVibratoDeviance.setSliderStyle(Slider::LinearBar);
	sliderVibratoDeviance.addListener(this);

	//tremolo
	addAndMakeVisible(labelTremolo);
	labelTremolo.setText("Tremolo:", dontSendNotification);
	labelTremolo.setJustificationType(Justification::left);

	addAndMakeVisible(buttonTremolo);
	buttonTremolo.setButtonText("Turn on");
	buttonTremolo.addListener(this);

	addAndMakeVisible(labelTremoloFrequency);
	labelTremoloFrequency.setText("Frequency:", dontSendNotification);
	labelTremoloFrequency.setJustificationType(Justification::left);
	labelTremoloFrequency.attachToComponent(&sliderTremoloFrequency, true);

	addAndMakeVisible(sliderTremoloFrequency);
	sliderTremoloFrequency.setRange(0.0001, 5000.0);
	sliderTremoloFrequency.setValue(100.0, dontSendNotification);
	sliderTremoloFrequency.setSkewFactorFromMidPoint(500.0);
	sliderTremoloFrequency.setTextValueSuffix("Hz");
	sliderTremoloFrequency.setSliderStyle(Slider::LinearBar);
	sliderTremoloFrequency.addListener(this);

	addAndMakeVisible(labelTremoloDeviance);
	labelTremoloDeviance.setText("Deviance:", dontSendNotification);
	labelTremoloDeviance.setJustificationType(Justification::left);
	labelTremoloDeviance.attachToComponent(&sliderTremoloDeviance, true);

	addAndMakeVisible(sliderTremoloDeviance);
	sliderTremoloDeviance.setRange(0.0, 1.0);
	sliderTremoloDeviance.setValue(0.5, dontSendNotification);
	sliderTremoloDeviance.setSliderStyle(Slider::LinearBar);
	sliderTremoloDeviance.addListener(this);
}

signalGeneratorUI::~signalGeneratorUI()
{
	releaseResources();
}

void signalGeneratorUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	currentSampleRate = sampleRate;

	whiteNoiseInstance.initialize(sampleRate, defaultFrequency);
	redNoiseInstance.initialize(sampleRate, defaultSamples);
	sineWaveInstance.initialize(sampleRate, defaultFrequency);
	rectangularWaveInstance.initialize(sampleRate, defaultFrequency);
	triangularWaveInstance.initialize(sampleRate, defaultFrequency);
	sawtoothWaveInstance.initialize(sampleRate, defaultFrequency);

	vibratoAndTremoloGenerator.initialize(sampleRate, defaultFrequency);
}

void signalGeneratorUI::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	if (isOn) {

		if (isVibratoOn) fillBufferVibrato(bufferToFill);
		else if (isTremoloOn) fillBufferTremolo(bufferToFill);
		else fillBuffer(bufferToFill);
	}
	else bufferToFill.clearActiveBufferRegion();
}

void signalGeneratorUI::releaseResources()
{
}

void signalGeneratorUI::fillBuffer(const AudioSourceChannelInfo & bufferToFill)
{
	switch (currentGeneratorChoice) {
	case white:
		whiteNoiseInstance.processAudioBlock(bufferToFill, volume, targetFrequency);
		break;

	case red:
		redNoiseInstance.processAudioBlock(bufferToFill, volume, targetFrequency);
		break;

	case sine:
		sineWaveInstance.processAudioBlock(bufferToFill, volume, targetFrequency);
		break;

	case rectangular:
		rectangularWaveInstance.processAudioBlock(bufferToFill, volume, targetFrequency);
		break;

	case sawtooth:
		sawtoothWaveInstance.processAudioBlock(bufferToFill, volume, targetFrequency);
		break;

	case triangular:
		triangularWaveInstance.processAudioBlock(bufferToFill, volume, targetFrequency);
		break;

	default:
		bufferToFill.clearActiveBufferRegion();
		break;
	}
}

void signalGeneratorUI::fillBufferVibrato(const AudioSourceChannelInfo & bufferToFill)
{
	//here we will hold frequencies for each sample
	AudioSampleBuffer frequencyBuffer(1, bufferToFill.numSamples);
	AudioSourceChannelInfo frequencyChannel(frequencyBuffer);

	//we get values from -1 to 1
	vibratoAndTremoloGenerator.processAudioBlock(frequencyChannel, 1, sliderVibratoFrequency.getValue());

	//we scale those values using base frequency and chosen deviance
	//where deviance is % of base frequency
	float* frequencyBufferPointer = frequencyBuffer.getWritePointer(0);

	for (int sample = 0; sample < bufferToFill.numSamples; ++sample) {

		frequencyBufferPointer[sample] = frequencyBufferPointer[sample] * sliderVibratoDeviance.getValue() * frequencySlider.getValue() + frequencySlider.getValue();
	}

	switch (currentGeneratorChoice) {
	case white:
		whiteNoiseInstance.processAudioBlock(bufferToFill, volume, frequencyBuffer);
		break;

	case red:
		redNoiseInstance.processAudioBlock(bufferToFill, volume, frequencyBuffer);
		break;

	case sine:
		sineWaveInstance.processAudioBlock(bufferToFill, volume, frequencyBuffer);
		break;

	case rectangular:
		rectangularWaveInstance.processAudioBlock(bufferToFill, volume, frequencyBuffer);
		break;

	case sawtooth:
		sawtoothWaveInstance.processAudioBlock(bufferToFill, volume, frequencyBuffer);
		break;

	case triangular:
		triangularWaveInstance.processAudioBlock(bufferToFill, volume, frequencyBuffer);
		break;

	default:
		bufferToFill.clearActiveBufferRegion();
		break;
	}
}

void signalGeneratorUI::fillBufferTremolo(const AudioSourceChannelInfo & bufferToFill)
{
	//here we will hold volume for each sample
	AudioSampleBuffer volumeBuffer(1, bufferToFill.numSamples);
	AudioSourceChannelInfo volumeChannel(volumeBuffer);

	//we get values from -1 to 1
	vibratoAndTremoloGenerator.processAudioBlock(volumeChannel, 1, sliderTremoloFrequency.getValue());

	//we scale those values using chosen deviance
	float* volumeBufferPointer = volumeBuffer.getWritePointer(0);

	for (int sample = 0; sample < bufferToFill.numSamples; ++sample) {

		volumeBufferPointer[sample] = volumeBufferPointer[sample] * sliderTremoloDeviance.getValue() * volumeSlider.getValue() + volumeSlider.getValue();
	}

	switch (currentGeneratorChoice) {
	case white:
		whiteNoiseInstance.processAudioBlock(bufferToFill, volumeBuffer, targetFrequency);
		break;

	case red:
		redNoiseInstance.processAudioBlock(bufferToFill, volumeBuffer, targetFrequency);
		break;

	case sine:
		sineWaveInstance.processAudioBlock(bufferToFill, volumeBuffer, targetFrequency);
		break;

	case rectangular:
		rectangularWaveInstance.processAudioBlock(bufferToFill, volumeBuffer, targetFrequency);
		break;

	case sawtooth:
		sawtoothWaveInstance.processAudioBlock(bufferToFill, volumeBuffer, targetFrequency);
		break;

	case triangular:
		triangularWaveInstance.processAudioBlock(bufferToFill, volumeBuffer, targetFrequency);
		break;

	default:
		bufferToFill.clearActiveBufferRegion();
		break;
	}
}

void signalGeneratorUI::paint(Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

	g.setColour(Colours::grey);
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
}

void signalGeneratorUI::resized()
{
	componentLabel.setBounds(0, (int)(getHeight()*0.025), getWidth(), 30);

	generatorsComboBox.setBounds(10, (int)(getHeight()*0.2), 120, 30);
	startButton.setBounds(140, (int)(getHeight()*0.2), 80, 30);
	//volume
	volumeSliderLabel.setBounds(220, (int)(getHeight()*0.2), 80, 30);
	volumeSlider.setBounds(310, (int)(getHeight()*0.2), 200, 30);

	//frequency
	frequencySliderLabel.setBounds(520, (int)(getHeight()*0.2), 80, 30);
	frequencySlider.setBounds(610, (int)(getHeight()*0.2), 200, 30);

	//vibrato
	labelVibrato.setBounds(10, (int)(getHeight()*0.5), 70, 30);
	buttonVibrato.setBounds(90, (int)(getHeight()*0.5), 90, 30);
	sliderVibratoFrequency.setBounds(190 + 80, (int)(getHeight()*0.5), 180, 30);
	sliderVibratoDeviance.setBounds(540, (int)(getHeight()*0.5), 180, 30);

	//tremolo
	labelTremolo.setBounds(10, (int)(getHeight()*0.8), 70, 30);
	buttonTremolo.setBounds(90, (int)(getHeight()*0.8), 90, 30);
	sliderTremoloFrequency.setBounds(190 + 80, (int)(getHeight()*0.8), 180, 30);
	sliderTremoloDeviance.setBounds(540, (int)(getHeight()*0.8), 180, 30);
}

void signalGeneratorUI::buttonClicked(Button* button)
{
	if (button == &startButton) {
		if (isOn == false) {
			startButton.setButtonText("Turn off");

			componentStatus = " ON";
			componentLabel.setText(componentName + componentStatus, dontSendNotification);
			componentLabel.setColour(Label::textColourId, Colours::green);
		}
		else {
			startButton.setButtonText("Turn on");

			componentStatus = " OFF";
			componentLabel.setText(componentName + componentStatus, dontSendNotification);
			componentLabel.setColour(Label::textColourId, Colours::red);
		}
		isOn = !isOn;
	}
	//vibrato and tremolo can't be on at the same time
	if (button == &buttonVibrato) {
		if (isVibratoOn) {
			isVibratoOn = false;
		}
		else {
			isVibratoOn = true;
			isTremoloOn = false;
			buttonTremolo.setToggleState(false, dontSendNotification);
		}
	}
	if (button == &buttonTremolo) {
		if (isTremoloOn) {
			isTremoloOn = false;
		}
		else {
			isTremoloOn = true;
			isVibratoOn = false;
			buttonVibrato.setToggleState(false, dontSendNotification);
		}
	}
}

void signalGeneratorUI::sliderValueChanged(Slider * slider)
{
	if (slider == &frequencySlider) {
		if (currentSampleRate > 0.0)
			targetFrequency = frequencySlider.getValue();
	}
	if (slider == & volumeSlider) {
	    volume = volumeSlider.getValue();
	}
}

void signalGeneratorUI::comboBoxChanged(ComboBox *comboBox)
{
	if (comboBox == &generatorsComboBox) {
		currentGeneratorChoice = comboBox->getSelectedId();

		//jeœli poprzednio slider by³ takiego samego typu, to nie muszê go zmieniaæ
		//za ka¿dym razem te¿ zmieniam defaultFrequency albo samples, ¿eby pamiêtaæ je, kiedy wrócê do odpowiedniego slidera
		if (currentGeneratorChoice == red) {
			if (wasPrevousSliderFrequency) {
				defaultFrequency = frequencySlider.getValue();
				frequencySliderForSamplesPerCycle();
			}
			wasPrevousSliderFrequency = false;
		}
		else {
			if (!wasPrevousSliderFrequency) {
				defaultSamples = (int)frequencySlider.getValue();
				frequencySliderForFrequency();
			}
			wasPrevousSliderFrequency = true;
		}
		
	}
}

void signalGeneratorUI::frequencySliderForFrequency()
{
	frequencySlider.setRange(minFrequency, maxFrequency);
	frequencySlider.setSkewFactorFromMidPoint(500.0);
	frequencySlider.setValue(defaultFrequency, dontSendNotification);
	frequencySlider.setTextValueSuffix("Hz");

	frequencySliderLabel.setText("Frequency:", dontSendNotification);

	targetFrequency = defaultFrequency;
}

void signalGeneratorUI::frequencySliderForSamplesPerCycle()
{
	frequencySlider.setRange(minSamples, maxSamples);
	frequencySlider.setSkewFactorFromMidPoint((minSamples + maxSamples)/2); //TODO nie wiem jak usun¹æ skew factor
	frequencySlider.setValue(defaultSamples, dontSendNotification);
	frequencySlider.setTextValueSuffix("");

	frequencySliderLabel.setText("Samples:", dontSendNotification);

	targetFrequency = defaultSamples;
}