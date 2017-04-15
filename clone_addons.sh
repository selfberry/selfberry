#!/bin/bash

# Clones dependencies for selfberry
# Inspired by ObviousJim and ofxTimeline "clone_addons.sh" script
# 20170415 was videodromm/ofxGifEncoder.git and videodromm/ofxRPiCameraVideoGrabber.git

cd ../../../addons/

if [ -z $1 ]; then
    PREFIX="git clone https://github.com/"
else
    PREFIX="git clone git@github.com:"
fi

${PREFIX}hamoid/ofxGifEncoder.git

${PREFIX}jvcleave/ofxImGui.git

${PREFIX}jvcleave/ofxRPiCameraVideoGrabber.git

${PREFIX}videodromm/ofxVideodromm.git

${PREFIX}pelayomendez/ofxTwitter.git

${PREFIX}fishkingsin/ofxFTP.git

echo "Done cloning addons! :)"
