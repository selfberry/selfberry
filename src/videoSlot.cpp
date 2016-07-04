#include "videoSlot.h"

videoSlot::videoSlot()
{
	//ctor
}

videoSlot::~videoSlot()
{
	//dtor
}

void videoSlot::init(int idNumber, int xx, int yy, std::string* directoreee, int kk) {
	id = idNumber;
	x = xx;
	y = yy;
	key = kk;
	dirString = *directoreee + ofToString(id);
}

void videoSlot::loadFrameNumber(int numbah) {
	std::string filename;

	if (numbah < 10) filename = dirString + "//seq00" + ofToString(numbah) + ".tga";
	if (numbah >= 10 && numbah < 100) filename = dirString + "//seq0" + ofToString(numbah) + ".tga";
	if (numbah >= 100 && numbah < 1000) filename = dirString + "//seq" + ofToString(numbah) + ".tga";

	img.load(filename);	// loadImage
	//ofLogNotice("load: "+ofToString(numbah)+":"+filename);
}

void videoSlot::xchangeWithBuffer() {
	// change directory file names
}

void videoSlot::update() {

}

void videoSlot::draw() {
	img.draw(x, y);
}


void videoSlot::drawAtOrigin() {
	img.draw(0, 0);
}
