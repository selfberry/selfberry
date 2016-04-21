#pragma once

#include "ofMain.h"

#include "videoSlot.h"
#include "recorder.h"
#include "uig.h"
#include "saver.h"
#include "ofxXmlSettings.h"
// Log
#include "VDLog.h"

using namespace VideoDromm;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);

private:
	VDLogRef mVDLog;
    ofDirectory dirSRC;
    ofDirectory dirDST;
    int slotAmount;
    int frameNumber;
    int maxFrames;
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
    recorder cctv;
    uig uiBackground;
    saver saveUs;
    
    int lastSpot;
};
