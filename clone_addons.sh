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

${PREFIX}jvcleave/ofxImGui.git

${PREFIX}videodromm/ofxRPiCameraVideoGrabber.git

${PREFIX}videodromm/ofxVideodromm.git

${PREFIX}pelayomendez/ofxTwitter.git

echo "Done cloning addons! :)"
