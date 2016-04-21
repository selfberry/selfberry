#ifndef VIDEOSLOT_H
#define VIDEOSLOT_H
#include "ofImage.h"
#include "ofFileUtils.h"

class videoSlot
{
    public:
        int id;
        // int frameNumber;
        // int movieLength;
        int x;
        int y;
        int key;
        std::string dirString;
        ofDirectory dir;
        bool lighted;

        videoSlot();
        virtual ~videoSlot();
        ofImage img;
        ofImage highlight;

        void init(int idNumber, int xx, int yy, std::string* directoreee, int kk, std::string high );
        void draw();
        void update();
        void loadFrameNumber(int numbah);
        void xchangeWithBuffer();

        void highlightSpot();
        void lowlightSpot();
        void showCounterAnimation();
        void hideCounterAnimation();
        void showProcessingAnimation();
        void hideProcessingAnimation();

    protected:
    private:
};

#endif // VIDEOSLOT_H
