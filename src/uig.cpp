#include "uig.h"

uig::uig()
{
    //ctor
}

uig::~uig()
{
    //dtor
}

void uig::init(std::string imago) {
    bkgLayer.loadImage(imago);
 //   for(int i=0; i<4;++) {
    //    bkgHiglight.loadImage("highLight"+ofToString(i)+".png");
 //   }
}


void uig::draw() {
    bkgLayer.draw(0,0);
}
