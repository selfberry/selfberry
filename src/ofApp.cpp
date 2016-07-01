#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetLogLevel("ofThread", OF_LOG_ERROR);

	ofEnableAlphaBlending();

	doDrawInfo = true;
	targetWidth = 640;
	targetHeight = 480;
    //get back a list of devices.
    vector<ofVideoDevice> devices = videoGrabber.listDevices();

    for(int i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            //log the device
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }else{
            //log the device and note it as unavailable
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }

	videoGrabber.setDeviceID(0);
	videoGrabber.setDesiredFrameRate(60);
	videoGrabber.initGrabber(targetWidth, targetHeight);

    videoInverted.allocate(targetWidth, targetHeight, OF_PIXELS_RGB);
    videoTexture.allocate(videoInverted);
    ofSetVerticalSync(true);

	// 1
	shader.load("shaderDesktop");

	fbo.allocate(targetWidth, targetHeight);
	fbo.begin();
	ofClear(0, 0, 0, 0);
	fbo.end();
}


//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(100, 100, 100);
	videoGrabber.update();

    if(videoGrabber.isFrameNew()){
        ofPixels & pixels = videoGrabber.getPixels();
        for(int i = 0; i < pixels.size(); i++){
            //invert the color of the pixel
            videoInverted[i] = 255 - pixels[i];
        }
        //load the inverted pixels
        videoTexture.loadData(videoInverted);
		fbo.begin();
		ofClear(0, 0, 0, 0);
		shader.begin();	  		  
#if defined(TARGET_OPENGLES)
		shader.setUniformTexture("tex0", videoTexture.texData, videoTexture.texData.textureID);
		videoGrabber.draw(0, 0);
#else
		shader.setUniformTexture("tex0", videoGrabber.getTexture(), 0);// 0 or 1?
		videoGrabber.draw(0, 0);
#endif
		shader.setUniform1f("time", ofGetElapsedTimef());
		shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
		videoGrabber.draw(0,0);
		shader.end();
		fbo.end();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetHexColor(0xffffff);
	videoGrabber.draw(20, 20);
    videoTexture.draw(20 + targetWidth, 20, targetWidth, targetHeight);
	fbo.draw(20 + targetWidth*2, 0);
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // in fullscreen mode, on a pc at least, the 
    // first time video settings the come up
    // they come up *under* the fullscreen window
    // use alt-tab to navigate to the settings
    // window. we are working on a fix for this...

    // Video settings no longer works in 10.7
    // You'll need to compile with the 10.6 SDK for this
    // For Xcode 4.4 and greater, see this forum post on instructions on installing the SDK
    // http://forum.openframeworks.cc/index.php?topic=10343
    if(key == 's' || key == 'S'){
		videoGrabber.videoSettings();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
}
