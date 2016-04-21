#ifndef SAVER_H
#define SAVER_H
//#include "ofApp.h"
// Gif encoder
#include "ofxGifEncoder.h"

class saver
{
    public:
        saver();
        virtual ~saver();
		void init(int maxFrame);
		//  void seekNewFiles();
		// ofDirectory dir;
		ofxGifEncoder				colorGifEncoder;
		ofPixels                    pixi;
		int                         maxFrameNumber;
		void                        saveGif(std::string pathe, int slot);
		void                        onGifSaved(string & fileName);
    protected:
    private:
};

#endif // GLITCHR_H
