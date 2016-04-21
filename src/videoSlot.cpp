#include "videoSlot.h"

videoSlot::videoSlot()
{
	//ctor
}

videoSlot::~videoSlot()
{
	//dtor
}

void videoSlot::init(int idNumber, int xx, int yy, std::string* directoreee, int kk, std::string high) {
	id = idNumber;
	x = xx;
	y = yy;
	key = kk;
	dirString = *directoreee + ofToString(id);
	highlight.loadImage(high);
	lighted = false;
	// ofLog() << dirString;
}

void videoSlot::loadFrameNumber(int numbah) {

	std::string dummy;
#ifdef TARGET_OPENGLES
	dummy = dirString + "//seq";
#else
	dummy = dirString + "/seq";
#endif
	//ofLog() << dirString;
	//   string dupa = dirString+"\seq00"+ofToString(numbah);
	//  ofLog() << dupa;
	//ofLog() << dupa << "K";
	if ((numbah < 100) && (numbah>9)) { dummy += "0"; };
	if (numbah < 10) { dummy += "00"; };
	dummy += ofToString(numbah) + ".tga";
	img.loadImage(dummy);
	//  ofLog() << dummy;
}

void videoSlot::xchangeWithBuffer() {
	// change directory file names
}

void videoSlot::update() {

}

void videoSlot::highlightSpot() {
	lighted = true;
}

void videoSlot::lowlightSpot() {
	lighted = false;
}

void videoSlot::showCounterAnimation() {

}


void videoSlot::hideCounterAnimation() {

}

void videoSlot::showProcessingAnimation() {

}

void videoSlot::hideProcessingAnimation() {

}


void videoSlot::draw() {
	img.draw(x, y);
	if (lighted == true) {
		highlight.draw(x, y);
	}
}
