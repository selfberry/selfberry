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

#if defined(TARGET_OPENGLES)

	ofSetVerticalSync(false);
	shader.load("shaderRpi");
#else
	ofSetVerticalSync(true);
	shader.load("shaderDesktop");
#endif
	iEffect = 2;

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
		shader.setUniformTexture("tex0", videoGrabber.getTextureReference(), 1);
		//videoGrabber.draw(0, 0);
#else
		shader.setUniformTexture("tex0", videoGrabber.getTexture(), 0);// 0 or 1?
		//videoGrabber.draw(0, 0);
#endif
		shader.setUniform1f("time", ofGetElapsedTimef());
		shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
		shader.setUniform3f("iResolution", ofGetWidth(), ofGetHeight(), 0);
		//shader.setUniform2f("iMouse", indexSavedPhoto, indexSavedPhoto);
		shader.setUniform1f("iGlobalTime", ofGetElapsedTimef());
		shader.setUniform4f("iDate", ofGetYear(), ofGetMonth(), ofGetDay(), ofGetSeconds());
		shader.setUniform1i("iEffect", iEffect);// floor(ofRandom(0, 4.9)));
		shader.setUniform1f("iChromatic", ofRandom(-1, 1));
		shader.setUniform1f("iShift", ofRandom(-1.0, 1.0));
		shader.setUniform1f("iGlitch", ofRandom(0.0, 1.0));
		shader.setUniform1f("iPixelate", ofRandom(0.7, 1.0));

		shader.end();
		fbo.end();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetHexColor(0xffffff);
	videoGrabber.draw(20, 20);
    videoTexture.draw(20 + targetWidth, 20, targetWidth, targetHeight);
	fbo.draw(0, targetHeight);
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	ofLogNotice("PRESSED KEY: " + ofToString(key));
	switch (key) {
	case 65:  // vert 65 357
			  //case 357:

		break;
	case 10: // rouge 10
	case 13: //	Entree
		
		break;
	case 126: //blanc 126 127
			  
		iEffect = 0;
		
		break;
	case 67: // jaune 67 0		
		iEffect = 1;
	case 66: // bleu 66 0		
		iEffect = 2;
	case 50:
	case 359:
		
		iEffect = 3;
		break;
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
