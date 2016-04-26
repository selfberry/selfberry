#pragma once

#include "ofMain.h"
#include "ofAppEGLWindow.h"
#include "TerminalListener.h"
#if defined(TARGET_OPENGLES)
#include "ofxRPiCameraVideoGrabber.h"
#endif
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
#if defined(TARGET_OPENGLES)
	ofxRPiCameraVideoGrabber videoGrabber;
    OMXCameraSettings omxCameraSettings;
	ImageFilterCollection filterCollection;
#else
    ofVideoGrabber 		videoGrabber;
#endif
	bool doDrawInfo;

	ofFbo fbo;
	ofShader shader;
	bool doShader;
    int targetWidth, targetHeight;
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

    int                         slotRecording;
    int                         recordedFramesAmount;
    int                         processedFramesAmount;

    videoSlot                   videoGrid[4];
    int                         lastSpot;
    int                         i;
	//recorder
	ofTexture					videoTexture;
	ofPixels pix;
	int							indexSavedPhoto;
	bool						isRecording;
	ofImage						savedImage;
	// gif encoder
	void						saveGif();
	ofxGifEncoder				colorGifEncoder;
	void						onGifSaved(string & fileName);
    int                         currentDisplaySlot;
};

