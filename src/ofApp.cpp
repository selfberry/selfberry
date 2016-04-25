#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetLogLevel("ofThread", OF_LOG_ERROR);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();

	doDrawInfo = true;

	consoleListener.setup(this);

	omxCameraSettings.width = 640;
	omxCameraSettings.height = 480;
	omxCameraSettings.framerate = 15;
	omxCameraSettings.enableTexture = true;

	videoGrabber.setup(omxCameraSettings);
	filterCollection.setup();

	doShader = true;
	shader.load("shaderExample");

	//fbo.allocate(omxCameraSettings.width, omxCameraSettings.height);
	fbo.allocate(omxCameraSettings.width, omxCameraSettings.height);
	fbo.begin();
	ofClear(0, 0, 0, 0);
	fbo.end();
	// selfberry
	colorGifEncoder.setup(omxCameraSettings.width, omxCameraSettings.height, .2, 256);
	ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);

	videoTexture.allocate(omxCameraSettings.width, omxCameraSettings.height, GL_RGB);

	bufferDir = "buffer";
	//uiBackground.init("ui.png");
	timeZero = ofGetElapsedTimeMicros();
	frameNumber = 0;
	slotRecording = 255;
	slotAmount = 3;
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
	indexSavedPhoto = 0;
	isRecording = false;
	amountOfFrames = 10;
	maxFrames = 10;
	if (settings.loadFile("settings.xml") == false) {
		ofLog() << "XML ERROR, possibly quit";
	}
	settings.pushTag("settings");

	/*if (settings.getValue("log", 1) == 0) {
		ofLogLevel(OF_LOG_SILENT);
	}
	fps = settings.getValue("fps", 15);
	maxFrames = settings.getValue("frameAmount", 15);
	bufferDir = settings.getValue("bufferDir", "buffer");
	outputDir = settings.getValue("outputDir", "output");
	if (settings.getValue("fullScreen", 0) == 1) {
		ofToggleFullscreen();
	}*/
	slotDir = "slot";
	for (int i = 0; i < settings.getNumTags("slot"); i++) {
		settings.pushTag("slot", i);
		videoGrid[i].init(settings.getValue("id", i), settings.getValue("x", 700), settings.getValue("y", 500), &slotDir, settings.getValue("key", 0));
		settings.popTag();
	}
	lastSpot = 0;
	currentDisplaySlot = 2;
}

//--------------------------------------------------------------
void ofApp::update()
{
	if (!doShader || !videoGrabber.isFrameNew())
	{
		return;
	}
	fbo.begin();
	ofClear(0, 0, 0, 0);
	shader.begin();
	shader.setUniformTexture("tex0", videoGrabber.getTextureReference(), videoGrabber.getTextureID());
	shader.setUniform1f("time", ofGetElapsedTimef());
	shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	videoGrabber.draw();
	shader.end();
	fbo.end();
	if (isRecording == true) {
		dirSRC.createDirectory(bufferDir);
		dirSRC.listDir(bufferDir);
		recordedFramesAmount = dirSRC.size();
		ofLogNotice("AMOUNT OF FILES: " + ofToString(recordedFramesAmount) + "/" + ofToString(maxFrames));
		if (recordedFramesAmount == maxFrames) {
			isRecording = false;
			indexSavedPhoto = 0;
		}
		else {
			if (videoGrabber.isFrameNew()) {
				string filename;
				if (indexSavedPhoto < 10) filename = "seq00" + ofToString(indexSavedPhoto);
				if (indexSavedPhoto >= 10 && indexSavedPhoto < 100) filename = "seq0" + ofToString(indexSavedPhoto);
				if (indexSavedPhoto >= 100 && indexSavedPhoto < 1000) filename = "seq" + ofToString(indexSavedPhoto);
				// fbo to pixels
				fbo.readToPixels(pix);
				fbo.draw(0, 0, omxCameraSettings.width, omxCameraSettings.height);
				ofLogNotice("AMOUNT OF FILES: " + ofToString(recordedFramesAmount) + "/" + ofToString(maxFrames));

				//pix.resize(targetWidth, targetHeight, OF_INTERPOLATE_NEAREST_NEIGHBOR);
				savedImage.setFromPixels(pix);
				savedImage.setImageType(OF_IMAGE_COLOR);
				switch (currentDisplaySlot) {
				case 2:
					savedImage.saveImage("slot2//" + filename + ".tga");
					break;
				case 3:
					savedImage.saveImage("slot3//" + filename + ".tga");
					break;
				case 4:
					savedImage.saveImage("slot4//" + filename + ".tga");
					break;
				}
				savedImage.saveImage(bufferDir + "//" + filename + ".tga");
				//omxCameraSettings.width, omxCameraSettings.height
				// add frame to gif encoder
				colorGifEncoder.addFrame(
					pix.getPixels(),
					omxCameraSettings.width,
					omxCameraSettings.height,
					pix.getBitsPerPixel()/*,
										 .1f duration */
				);

				recordedFramesAmount++;
				pix.clear();
				savedImage.clear();
				indexSavedPhoto++;
				if (indexSavedPhoto == amountOfFrames) {
					ofLogNotice("Stop recording: " + ofToString(indexSavedPhoto) + "/" + ofToString(amountOfFrames));
					isRecording = false;
					indexSavedPhoto = 0;
					saveGif();
				}
			}

		}
	}
	for (i = 0; i < slotAmount; i++) {
		videoGrid[i].loadFrameNumber(frameNumber);
	}
	frameNumber++;
	if (frameNumber == maxFrames) {
		frameNumber = 0;
	}
}
void ofApp::saveGif()
{
	string fileName = ofToString(ofGetMonth()) + "-" + ofToString(ofGetDay()) + "-" + ofToString(ofGetHours()) + "-" + ofToString(ofGetMinutes()) + "-" + ofToString(ofGetSeconds());
	ofLogNotice("saveGif: " + fileName);
	colorGifEncoder.save("gif//" + fileName + ".gif");
}
void ofApp::onGifSaved(string & fileName) {
	cout << "gif saved as " << fileName << endl;
	ofLogNotice("onGifSaved: " + fileName);
	colorGifEncoder.reset();
}
//--------------------------------------------------------------
void ofApp::draw() {

	if (doShader)
	{
		fbo.draw(0, 0);
	}
	else
	{
		videoGrabber.draw();
	}
	for (int i = 0; i < slotAmount; i++) {
		videoGrid[i].draw();
	}
	stringstream info;
	info << "APP FPS: " << ofGetFrameRate() << "\n";
	info << "Camera Resolution: " << videoGrabber.getWidth() << "x" << videoGrabber.getHeight() << " @ " << videoGrabber.getFrameRate() << "FPS" << "\n";
	info << "CURRENT FILTER: " << filterCollection.getCurrentFilterName() << "\n";
	info << "SHADER ENABLED: " << doShader << "\n";
	//info <<	filterCollection.filterList << "\n";

	info << "\n";
	info << "VERT: changement de filtre" << "\n";
	info << "ROUGE: enregistrer" << "\n";

	if (doDrawInfo)
	{
		ofDrawBitmapStringHighlight(info.str(), 500, 400, ofColor::black, ofColor::yellow);
	}

	//
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	ofLog(OF_LOG_VERBOSE, "%c keyPressed", key);
	ofLogNotice("PRESSED KEY: " + ofToString(key));
	/*RED 13 10
		WHITE 127 126
		YELLOW 54
		GREEN 357 65
		BLUE 50*/
	switch (key) {
	case 65:
		videoGrabber.setImageFilter(filterCollection.getNextFilter());
		break;
	case 10:
	case 13:
		if (!isRecording) {
			isRecording = true;
			indexSavedPhoto = 0;
			currentDisplaySlot++;
			if (currentDisplaySlot > 4) currentDisplaySlot = 2;
			bufferDir = ofToString(ofGetMonth()) + "-" + ofToString(ofGetDay()) + "-" + ofToString(ofGetHours()) + "-" + ofToString(ofGetMinutes()) + "-" + ofToString(ofGetSeconds());
		}
		break;
	case 126:
		doDrawInfo = !doDrawInfo;
		break;
	}

	/*
	if (key == 's')
	{
		doShader = !doShader;
	}*/

}

void ofApp::onCharacterReceived(KeyListenerEventData& e)
{
	keyPressed((int)e.character);
}

