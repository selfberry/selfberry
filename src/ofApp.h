#pragma once

#include "ofMain.h"
#if defined(TARGET_OPENGLES)
#include "ofAppEGLWindow.h"
#include "TerminalListener.h"
#include "ofxRPiCameraVideoGrabber.h"
#include "ImageFilterCollection.h"
#endif
#include "ofxXmlSettings.h"
#include "ofxGifEncoder.h"
#include "videoSlot.h"

#if defined(TARGET_OPENGLES)
class ofApp : public ofBaseApp, public KeyListener {
#else
class ofApp : public ofBaseApp {
#endif
public:

	void setup();
	void update();
	void draw();
	void keyPressed(int key);


#if defined(TARGET_OPENGLES)
	ofxRPiCameraVideoGrabber videoGrabber;
    OMXCameraSettings omxCameraSettings;
	ImageFilterCollection filterCollection;
	void onCharacterReceived(KeyListenerEventData& e);
	TerminalListener consoleListener;
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

