#ifndef UI_H
#define UI_H
#include "ofImage.h"

class uig
{
    public:
        uig();
        virtual ~uig();

        bool counterAnimationFinished;

        void init(std::string imago);
        void highlightSpot(int numbah);
        void showCounterAnimation(int numbah);
        void hideCounterAnimation(int numbah);
        void showProcessingAnimation(int numbah);
        void hideProcessingAnimation(int numbah);
        void draw();

        ofImage bkgLayer;
        ofImage glitchLayer1;
         ofImage glitchLayer2;
         bool meh;
        void bkgDraw();

    protected:
    private:
};

#endif // UI_H
