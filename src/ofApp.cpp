#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetLogLevel("ofThread", OF_LOG_ERROR);

	ofEnableAlphaBlending();

	doDrawInfo = true;

	targetWidth = 640;
	targetHeight = 480;
#if defined(TARGET_OPENGLES)
	consoleListener.setup(this);
	omxCameraSettings.width = targetWidth;
	omxCameraSettings.height = targetHeight;
	omxCameraSettings.framerate = 15;
	omxCameraSettings.enableTexture = true;

	videoGrabber.setup(omxCameraSettings);
	filterCollection.setup();
	ofSetVerticalSync(false);
#else
	videoGrabber.setDeviceID(0);
	videoGrabber.setDesiredFrameRate(30);
	videoGrabber.setup(targetWidth, targetHeight);
	ofSetVerticalSync(true);
#endif
	doShader = true;
	shader.load("shaderExample");

	fbo.allocate(targetWidth, targetHeight);
	fbo.begin();
	ofClear(0, 0, 0, 0);
	fbo.end();
	// selfberry
	colorGifEncoder.setup(targetWidth, targetHeight, .2, 256);
	ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);

	videoTexture.allocate(targetWidth, targetHeight, GL_RGB);

	bufferDir = "buffer";

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
	bkgLayer.loadImage("ui.png");
}

//--------------------------------------------------------------
void ofApp::update()
{
#if defined(TARGET_OPENGLES)

#else
	videoGrabber.update();
#endif
	if (!doShader )
	{
		ofLogNotice("update() !doShader return");
		return;
	}
	if (!videoGrabber.isFrameNew())
	{
		ofLogNotice("update() !videoGrabber.isFrameNew return");
		return;
	}
	//ofLogNotice("update() fbo begin");

	fbo.begin();
	ofClear(1, 1, 0, 0);
	shader.begin();
	shader.setUniform1f("time", ofGetElapsedTimef());
	shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
#if defined(TARGET_OPENGLES)
	shader.setUniformTexture("tex0", videoGrabber.getTextureReference(), videoGrabber.getTextureID());
	videoGrabber.draw();
#else
	shader.setUniformTexture("tex0", videoGrabber.getTexture(), 0 );// 0 or 1?
	videoGrabber.draw(0, 0);
#endif
	shader.end();
	fbo.end();
	//ofLogNotice("update() fbo end");

	if (isRecording == true) {
		ofLogNotice("update() rec");

		dirSRC.createDirectory(bufferDir);
		dirSRC.listDir(bufferDir);
		recordedFramesAmount = dirSRC.size();
		ofLogNotice("AMOUNT OF FILES: " + ofToString(recordedFramesAmount) + "/" + ofToString(maxFrames));
		if (recordedFramesAmount == maxFrames) {
			isRecording = false;
			indexSavedPhoto = 0;
			ofLogNotice("update() stop recording");
		}
		else {
			if (videoGrabber.isFrameNew()) {
				ofLogNotice("update() isFrameNew");

				string filename;
				if (indexSavedPhoto < 10) filename = "seq00" + ofToString(indexSavedPhoto);
				if (indexSavedPhoto >= 10 && indexSavedPhoto < 100) filename = "seq0" + ofToString(indexSavedPhoto);
				if (indexSavedPhoto >= 100 && indexSavedPhoto < 1000) filename = "seq" + ofToString(indexSavedPhoto);
				// fbo to pixels
				fbo.readToPixels(pix);
				fbo.draw(0, 0, targetWidth, targetHeight);
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
					targetWidth,
					targetHeight,
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
	for (i = 1; i < slotAmount; i++) {
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
	ofLogNotice("saveGif end");
}
void ofApp::onGifSaved(string & fileName) {
	cout << "gif saved as " << fileName << endl;
	ofLogNotice("onGifSaved: " + fileName);
	colorGifEncoder.reset();
	ofLogNotice("onGifSaved reset");
}
//--------------------------------------------------------------
void ofApp::draw() {

	ofClear(0, 0, 0, 0);
	stringstream info;
	info << "APP FPS: " << ofGetFrameRate() << "\n";
	info << "SHADER ENABLED: " << doShader << "\n";

	if (doShader)
	{
#if defined(TARGET_OPENGLES)
		fbo.draw(330, 13);
#else
		videoGrabber.draw(330, 13);
#endif
	}
	else
	{
#if defined(TARGET_OPENGLES)
		videoGrabber.draw();
		info << "Camera Resolution: " << videoGrabber.getWidth() << "x" << videoGrabber.getHeight() << " @ " << videoGrabber.getFrameRate() << "FPS" << "\n";
		info << "CURRENT FILTER: " << filterCollection.getCurrentFilterName() << "\n";
#else
		videoGrabber.draw(0, 0);
#endif
	}
	for (int i = 1; i < slotAmount; i++) {
		videoGrid[i].draw();
	}

	info << "\n";
	info << "VERT: changement de filtre" << "\n";
	info << "ROUGE: enregistrer" << "\n";
	bkgLayer.draw(0, 0);
	if (doDrawInfo)
	{
		ofDrawBitmapStringHighlight(info.str(), 50, 940, ofColor::black, ofColor::yellow);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	//ofLog(OF_LOG_VERBOSE, "%c keyPressed", key);
	ofLogNotice("PRESSED KEY: " + ofToString(key));
	/*RED 13 10
		WHITE 127 126
		YELLOW 54
		GREEN 357 65
		BLUE 50*/
	switch (key) {
	case 65:
	case 357:
#if defined(TARGET_OPENGLES)
		videoGrabber.setImageFilter(filterCollection.getNextFilter());
#endif
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
	case 50:
	case 359:
		doShader = !doShader;
		break;
	}
}
#if defined(TARGET_OPENGLES)
void ofApp::onCharacterReceived(KeyListenerEventData& e)
{
	keyPressed((int)e.character);
}
#endif	
