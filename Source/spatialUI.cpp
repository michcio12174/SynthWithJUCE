#include "../JuceLibraryCode/JuceHeader.h"
#include "spatialUI.h"

spatialUI::spatialUI():
	width(100),
	height(100),
	isPrepared(false),
	generatedMoving(nullptr),
	generatedStationary(nullptr),
	fromFileStationary(nullptr)
{
	initializeUI();
}

spatialUI::spatialUI(int width, int height):
	width(width),
	height(height),
	isPrepared(false)
{
	initializeUI();
}

spatialUI::~spatialUI()
{
	delete listenerInTheScene;
	delete generatedMoving;
	this->removeKeyListener(this);
}

void spatialUI::initializeUI()
{
	listenerInTheScene = new spatialListener(point2D(width, height));
	setWantsKeyboardFocus(true);
	this->addKeyListener(this);

	startTimer(100);

	//setting the reverb rectangle boundaries
	upperLeftReverb = point2D(20, 60);
	reverbWidth = 20;
	reverbHeight = 20;
}

void spatialUI::paint(Graphics& g)
{
	g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

	g.setColour(Colours::grey);
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component


	//find the center of this component
	Rectangle<int> componentBoundary = getLocalBounds();
	int centerX = componentBoundary.getWidth() / 2;
	int centerY = componentBoundary.getHeight() / 2;

	//find corner of play area
	int playAreaTopLeftX = centerX - width / 2;
	int playAreaTopLeftY = centerY - height / 2;

	//draw the reverb zone
	g.setColour(Colours::lightgreen);
	g.fillRect(playAreaTopLeftX + (int)upperLeftReverb.x, playAreaTopLeftY + (int)upperLeftReverb.y, reverbWidth, reverbHeight);

	//draw boundaries of the play area
	g.setColour(Colours::brown);
	g.drawRect(playAreaTopLeftX, playAreaTopLeftY, width, height, 2);

	//painting the listener dot
	g.setColour(listenerInTheScene->dotColour);
	g.fillEllipse(playAreaTopLeftX + listenerInTheScene->position.x - 2, playAreaTopLeftY + listenerInTheScene->position.y - 2, 4, 4);
	//painting sources dots
	if (generatedMoving != nullptr) {
		g.setColour(generatedMoving->dotColour);
		g.fillEllipse(playAreaTopLeftX + generatedMoving->currentPosition.x - 2, playAreaTopLeftY + generatedMoving->currentPosition.y - 2, 4, 4);
	}
	if (generatedStationary != nullptr) {
		g.setColour(generatedStationary->dotColour);
		g.fillEllipse(playAreaTopLeftX + generatedStationary->currentPosition.x - 2, playAreaTopLeftY + generatedStationary->currentPosition.y - 2, 4, 4);
	}

	if (fromFileStationary != nullptr) {
		g.setColour(fromFileStationary->dotColour);
		g.fillEllipse(playAreaTopLeftX + fromFileStationary->currentPosition.x - 2, playAreaTopLeftY + fromFileStationary->currentPosition.y - 2, 4, 4);
	}
}

void spatialUI::resized()
{

}

void spatialUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	listenerInTheScene->prepareToPlay(samplesPerBlockExpected, sampleRate);

	//creating audio sources in the scene
	generatedMoving = new spatialSource(sine, Colours::yellow, point2D(0, 0), point2D(20, 20), 10);
	listenerInTheScene->addSpatialSource(*generatedMoving);

	generatedStationary = new spatialSource(wind, Colours::green, point2D(80, 80));
	generatedStationary->prepareToPlay(samplesPerBlockExpected, sampleRate);
	listenerInTheScene->addSpatialSource(*generatedStationary);

	fromFileStationary = new spatialSource(player, Colours::blue, point2D(20, 80));
	listenerInTheScene->addSpatialSource(*fromFileStationary);

	isPrepared = true;
}

void spatialUI::getNextAudioBlock(const AudioSourceChannelInfo & bufferToFill)
{
	if(isPrepared) listenerInTheScene->getNextAudioBlock(bufferToFill);
	else Logger::getCurrentLogger()->writeToLog("spatialUI was not prepared to play\n");

}

void spatialUI::releaseResources()
{
	listenerInTheScene->releaseResources();
}

bool spatialUI::keyPressed(const KeyPress & key, Component * originatingComponent)
{
	return false;
}

bool spatialUI::keyStateChanged(bool isKeyDown, Component * originatingComponent)
{
	listenerInTheScene->isWOn = false;
	listenerInTheScene->isSOn = false;
	listenerInTheScene->isAOn = false;
	listenerInTheScene->isDOn = false;

	if (KeyPress::isKeyCurrentlyDown('w')) listenerInTheScene->isWOn = true;
	if (KeyPress::isKeyCurrentlyDown('s')) listenerInTheScene->isSOn = true;
	if (KeyPress::isKeyCurrentlyDown('a')) listenerInTheScene->isAOn = true;
	if (KeyPress::isKeyCurrentlyDown('d')) listenerInTheScene->isDOn = true;

	//enable reverb when in reverb zone
	if (listenerInTheScene->position.x >= upperLeftReverb.x &&
		listenerInTheScene->position.x < (upperLeftReverb.x + reverbWidth) &&
		listenerInTheScene->position.y >= upperLeftReverb.y &&
		listenerInTheScene->position.y < (upperLeftReverb.y + reverbHeight)) listenerInTheScene->enableReverb(reverbParameters);
	else listenerInTheScene->disableReverb();

	return false;
}

void spatialUI::timerCallback()
{
	repaint();
}
