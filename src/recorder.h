#ifndef RECORDER_H
#define RECORDER_H

#include "ofMain.h"
// Gif encoder
#include "ofxGifEncoder.h"
#ifdef TARGET_OPENGLES
#include "ofxRPiCameraVideoGrabber.h"
#endif
class recorder
{
    public:
        recorder();
        virtual ~recorder();
		void update();
		void init(int frameNumber, int cameraId);
		void record();
		//void draw(int x, int y);

        bool						isRecording;
    protected:
    private:

        int      amountOfFrames;
        ofPixels pix;
        ofFbo fbo;
#ifdef TARGET_OPENGLES
		ofxRPiCameraVideoGrabber	videoGrabber;
		OMXCameraSettings omxCameraSettings;

#else
		ofVideoGrabber 				videoGrabber;
		
#endif
		int 						camWidth;
		int							camHeight;
		int 						targetWidth;
		int							targetHeight;

		int							indexSavedPhoto;
		bool						useShader;

		ofImage						savedImage;
		unsigned char * 			videoPixels;
		ofTexture					videoTexture;
		ofTexture                   noizzz;
		// shaders
		ofShader					shader;
		//ofFbo						shaderFbo;
		// textures
		ofTexture					iChannel0;
		ofTexture					iChannel1;
		// uniform variables
		int							iEffect;
		float						iChromatic; 
		void						saveGif();
		ofxGifEncoder				colorGifEncoder;
		// ascii
		string						asciiCharacters;
		ofTrueTypeFont				font;
		bool						useAscii;
		ofFbo						asciiFbo;

};

#endif // RECORDER_H
