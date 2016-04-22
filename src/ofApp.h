#pragma once

#include "ofMain.h"
#include "ofAppEGLWindow.h"
#include "TerminalListener.h"
#include "ofxRPiCameraVideoGrabber.h"
#include "ImageFilterCollection.h"
#include "ofxXmlSettings.h"
#include "ofxGifEncoder.h"
#include "videoSlot.h"

class ofApp : public ofBaseApp, public KeyListener { 

	public:

		void setup();
		void update();
		void draw();
		void keyPressed(int key);

	void onCharacterReceived(KeyListenerEventData& e);
	TerminalListener consoleListener;
	ofxRPiCameraVideoGrabber videoGrabber;
	
	ImageFilterCollection filterCollection;
	
	bool doDrawInfo;
	
	ofFbo fbo;
	ofShader shader;
	bool doShader;
		
	OMXCameraSettings omxCameraSettings;
	// selfberry
	ofDirectory dirSRC;
	ofDirectory dirDST;
    int slotAmount;
    int frameNumber;
    int maxFrames;
    int      amountOfFrames;

    int fps;
    
    int timer;
    
    ofxXmlSettings settings;
    ofTrueTypeFont TTF;
    
    long long timeZero;
    long long timePresent;
    
    std::string bufferDir;
    std::string slotDir;
    std::string outputDir;
    
    int slotRecording;
    int recordedFramesAmount;
    int processedFramesAmount;
    
    videoSlot videoGrid[4];
    int lastSpot;
    int i;
    //recorder
    ofTexture					videoTexture;
    ofPixels pix;  
    int							indexSavedPhoto;
    bool						isRecording;
    ofImage						savedImage;
    // gif encoder
    void						saveGif();
	ofxGifEncoder				colorGifEncoder;
    int currentDisplaySlot;
};

