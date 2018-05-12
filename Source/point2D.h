#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class point2D {
public:
	point2D();
	point2D(float x, float y);
	point2D(point2D &point);
	~point2D();

	float distance(point2D B);

	float x, y;
};