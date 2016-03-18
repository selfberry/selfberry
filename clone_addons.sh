#!/bin/bash

# Clones dependencies for selfberry
# Inspired by ObviousJim and ofxTimeline "clone_addons.sh" script

cd ../../../addons/

if [ -z $1 ]; then
    PREFIX="git clone https://github.com/"
else
    PREFIX="git clone git@github.com:"
fi

${PREFIX}videodromm/ofxGifEncoder.git

echo "IMPORTANT WARNING: If you're using Linux, please make sure you checkout the develop branch of ofxTextInputField"

${PREFIX}jvcleave/ofxImGui.git

${PREFIX}videodromm/ofxRPiCameraVideoGrabber.git

echo "Done cloning addons! :)"
