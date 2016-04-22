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

        videoSlot();
        virtual ~videoSlot();
        ofImage img;

        void init(int idNumber, int xx, int yy, std::string* directoreee, int kk );
        void draw();
		void drawAtOrigin();
        void update();
        void loadFrameNumber(int numbah);
        void xchangeWithBuffer();

    protected:
    private:
};

#endif // VIDEOSLOT_H
