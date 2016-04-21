#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	// Log
	mVDLog = VDLog::create();
	mVDLog->log("setup");
	if (settings.loadFile("settings.xml") == false) {
		ofLog() << "XML ERROR, possibly quit";
	}
	settings.pushTag("settings");

	/*if (settings.getValue("log", 1) == 0) {
		ofLogLevel(OF_LOG_SILENT);
	}	 */
	fps = settings.getValue("fps", 15);
	maxFrames = settings.getValue("frameAmount", 15);
	bufferDir = settings.getValue("bufferDir", "buffer");
	slotDir = settings.getValue("slotDir", "slot");
	outputDir = settings.getValue("outputDir", "output");
	if (settings.getValue("fullScreen", 0) == 1) {
		ofToggleFullscreen();
	}
	for (int i = 0;i < settings.getNumTags("slot");i++) {
		settings.pushTag("slot", i);
		videoGrid[i].init(settings.getValue("id", i), settings.getValue("x", 0), settings.getValue("y", 0), &slotDir, settings.getValue("key", 0), settings.getValue("highlight", "high"));
		settings.popTag();
	}

	cctv.init(maxFrames, settings.getValue("cameraId", 1));
	// TTF.loadFont("mono.ttf", 7);
	uiBackground.init(settings.getValue("ui", "uuu.png"));
	timeZero = ofGetElapsedTimeMicros();
	frameNumber = 0;
	slotRecording = 255;
	slotAmount = 4;
	if (dirSRC.doesDirectoryExist(bufferDir)) {
		dirSRC.removeDirectory(bufferDir, true);
	}
	if (!dirSRC.doesDirectoryExist("slot1")) {
		dirSRC.createDirectory("slot1");
	}
	if (!dirSRC.doesDirectoryExist("slot2")) {
		dirSRC.createDirectory("slot2");
	}
	if (!dirSRC.doesDirectoryExist("slot3")) {
		dirSRC.createDirectory("slot3");
	}
	if (!dirSRC.doesDirectoryExist("slot4")) {
		dirSRC.createDirectory("slot4");
	}
	if (!dirSRC.doesDirectoryExist("tmp")) {
		dirSRC.createDirectory("tmp");
	}
	dirSRC.createDirectory(bufferDir);
	saveUs.init(maxFrames);
}

//--------------------------------------------------------------
void ofApp::update() {
	int i;
	timePresent = ofGetElapsedTimeMicros();
	if ((timePresent - timeZero) >= (1000000 / fps)) {
		cctv.update();
		timeZero = timePresent;
		for (i = 0; i < slotAmount;i++) {
			if (slotRecording != i) {
				videoGrid[i].loadFrameNumber(frameNumber);
			}
		}
		frameNumber++;
		if (frameNumber == maxFrames) {
			frameNumber = 0;
		}
	}

	if (cctv.isRecording == true) {
		dirSRC.listDir(bufferDir);
		//  dirDST.listDir(slotDir+ofToString(slotRecording));
		recordedFramesAmount = dirSRC.size();
		//  ofLogNotice("AMOUNT OF FILES: "+ofToString(recordedFramesAmount)+"/"+ofToString(maxFrames)+":"+ofToString(slotRecording));
		if (recordedFramesAmount == maxFrames) {
			//ofLog() << videoGrid[slotRecording].dirString << "l";
			dirSRC.removeDirectory(videoGrid[slotRecording].dirString, true);
			dirSRC.renameTo(videoGrid[slotRecording].dirString);
			// dirSRC.renameTo("temp");
			//   dirDST.renameTo(bufferDir);
			dirSRC.createDirectory(bufferDir);
			slotRecording = 255;
			for (i = 0;i < slotAmount;i++) {
				videoGrid[i].lowlightSpot();
			}
			//ofDirectory.crea
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	for (int i = 0; i < slotAmount;i++) {
		videoGrid[i].draw();
	}
	uiBackground.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	ofLogNotice("PRESSED KEY: " + ofToString(key));
	if (slotRecording == 255) {
		/*RED 13
			WHITE 127
			YELLOW 54
			GREEN 357
			BLUE 50*/
		for (int i = 0;i < slotAmount;i++) {
			if ((videoGrid[i].key == key) || (key == 49 + i)) {
				cctv.record();
				slotRecording = i;
				videoGrid[i].highlightSpot();
			}
		}
	}
	else {
		// starts recording from beggining
	}
	ofLogNotice(ofToString(key));
	if (key == settings.getValue("change", 0) || (key == 32)) {
		saveUs.saveGif(videoGrid[lastSpot].dirString, lastSpot);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}
