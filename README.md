# Selfberry
Make animated Gif file from a raspberry pi's camera, share them on social networks.
Ideal for events.
Created by Bruce Lane, Piotr Nierobisz, SebMas

Follow instructions here to setup openFrameworks for raspberry pi: http://openframeworks.cc/setup/raspberrypi/raspberry-pi-getting-started/

Then 
```
cd /home/pi/openFrameworks/apps/
mkdir selfberry
cd selfberry
git clone https://github.com/selfberry/selfberry/
cd selfberry
./clone_addons.sh
make
./bin/selfberry
```

Once compiled, launch directly from command line 
```
./home/pi/openFrameworks/apps/selfberry/selfberry/bin/selfberry
```

Keyboard shortcuts:
`enter` to record,
`up` or `down` arrow to choose shader

Some samples on 
https://videodromm.com/selfberry/