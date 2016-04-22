#include "uig.h"

uig::uig()
{
	//ctor
}

uig::~uig()
{
	//dtor
}

void uig::init(std::string imago) {
	bkgLayer.loadImage(imago);
	glitchLayer1.loadImage("noise1.jpg");
	glitchLayer2.loadImage("noise2.jpg");
	meh = true;

}


void uig::draw() {
	bkgLayer.draw(0, 0);
}

void uig::bkgDraw() {
	if (meh == true) {
		glitchLayer1.draw(0, 0);
		meh = false;
	}
	else {
		meh = true;
		glitchLayer2.draw(0, 0);
	}
}
