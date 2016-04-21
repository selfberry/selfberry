// that part takes the frame number, reads the file from //buffer// directory
// performs glitching, and saves it into the same place
// it's a separate thread

// joelgethinlewis

#include "saver.h"

saver::saver()
{
	//ctor
}

saver::~saver()
{
	//dtor  fs
}

void saver::init(int maxFrame) {
	colorGifEncoder.setup(512, 512, 1, 256);
	ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &saver::onGifSaved);
	maxFrameNumber = maxFrame;
}

void saver::saveGif(std::string pathe, int slot) {
	ofFile filaz;
	string fileName = "gif" + ofToString(ofGetMonth()) + "-" + ofToString(ofGetDay()) + "-" + ofToString(ofGetHours()) + "-" + ofToString(ofGetMinutes()) + "-" + ofToString(ofGetSeconds());
	//	cout << "saveGif: " << fileName << endl;
    ofLogNotice("saveGif, slot " + ofToString(slot) + ", filename: " + fileName);
	// string fileName = ofToString(slot);

	colorGifEncoder.setup(256, 256, .2, 256);
	string filenamezz;
	for (int i = 0; i<maxFrameNumber; i++) {
		if (i < 10) filenamezz = "seq00" + ofToString(i) + ".tga";
		if (i >= 10 && i < 100) filenamezz = "seq0" + ofToString(i) + ".tga";
		if (i >= 100 && i < 1000) filenamezz = "seq" + ofToString(i) + ".tga";
		if (filaz.doesFileExist(pathe + "/" + filenamezz)) {
			ofLoadImage(pixi, pathe + "/" + filenamezz);
			ofLogNotice("Frame: " + ofToString(i) + pathe + "/" + filenamezz);
			colorGifEncoder.addFrame(pixi.getPixels(), pixi.getWidth(), pixi.getHeight(), pixi.getBitsPerPixel());
		}
	}

	colorGifEncoder.save("gif/" + fileName + ".gif");
}

void saver::onGifSaved(string & fileName) {
    ofLogNotice("Gif saved");
}
