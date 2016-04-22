#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetLogLevel("ofThread", OF_LOG_ERROR);
	ofSetVerticalSync(false);
	ofEnableAlphaBlending();
	
	doDrawInfo	= true;
		
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
	videoTexture.allocate(omxCameraSettings.width, omxCameraSettings.height, GL_RGB);

	bufferDir = "buffer";
	//uiBackground.init("ui.png");
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
		ofLogNotice("AMOUNT OF FILES: "+ofToString(recordedFramesAmount)+"/"+ofToString(maxFrames));
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
				// FBO TODO GETTEXTURE? videoTexture = videoGrabber.getTextureReference();
				fbo.readToPixels(pix);
				fbo.draw(0,0, omxCameraSettings.width, omxCameraSettings.height);
						ofLogNotice("AMOUNT OF FILES: "+ofToString(recordedFramesAmount)+"/"+ofToString(maxFrames));

				//pix.resize(targetWidth, targetHeight, OF_INTERPOLATE_NEAREST_NEIGHBOR);
				savedImage.setFromPixels(pix);
				savedImage.setImageType(OF_IMAGE_COLOR);
				savedImage.saveImage("buffer//" + filename + ".tga");
				//omxCameraSettings.width, omxCameraSettings.height
				// add frame to gif encoder
				colorGifEncoder.addFrame(
					pix.getPixels(),
					omxCameraSettings.width, 
					omxCameraSettings.height, 
					pix.getBitsPerPixel()/*,
										 .1f duration */
				);				
				/*colorGifEncoder.addFrame(
					pix.getPixels(),
					pix.getWidth(),
					pix.getHeight(),
					pix.getBitsPerPixel()
				);*/
				recordedFramesAmount++;
				pix.clear();
				savedImage.clear();
				indexSavedPhoto++;
				if (indexSavedPhoto == (amountOfFrames + 1)) {
					isRecording = false;
					indexSavedPhoto = 0;
					saveGif();
				}
			}

		}
	}
}
void ofApp::saveGif()
{

	string fileName = ofToString(ofGetMonth()) + "-" + ofToString(ofGetDay()) + "-" + ofToString(ofGetHours()) + "-" + ofToString(ofGetMinutes()) + "-" + ofToString(ofGetSeconds());
	colorGifEncoder.save("gif//" + fileName + ".gif");
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	if (doShader)
	{
		fbo.draw(0, 0);		
	}else 
	{
		videoGrabber.draw();
	}

	stringstream info;
	info << "APP FPS: " << ofGetFrameRate() << "\n";
	info << "Camera Resolution: " << videoGrabber.getWidth() << "x" << videoGrabber.getHeight()	<< " @ "<< videoGrabber.getFrameRate() <<"FPS"<< "\n";
	info << "CURRENT FILTER: " << filterCollection.getCurrentFilterName() << "\n";
	info << "SHADER ENABLED: " << doShader << "\n";
	//info <<	filterCollection.filterList << "\n";
	
	info << "\n";
	info << "Press e to increment filter" << "\n";
	info << "Press g to Toggle info" << "\n";
	info << "Press s to Toggle Shader" << "\n";
	
	if (doDrawInfo) 
	{
		ofDrawBitmapStringHighlight(info.str(), 100, 100, ofColor::black, ofColor::yellow);
	}
	
	//
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key)
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

