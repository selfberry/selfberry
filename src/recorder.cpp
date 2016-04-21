#include "recorder.h"

recorder::recorder()
{
	//ctor
}
void recorder::record() {
#ifdef TARGET_OPENGLES
	videoGrabber.startRecording();
#endif
	isRecording = true;
}


void recorder::init(int frameNumber, int cameraID) {

	camWidth = 640;	// try to grab at this size.
	camHeight = 480;

	targetWidth = 512;
	targetHeight = 512;

	amountOfFrames = frameNumber;
#ifdef TARGET_OPENGLES
	omxCameraSettings.width = 1280; //default 1280
	omxCameraSettings.height = 720; //default 720
	omxCameraSettings.enableTexture = true; //default true
	omxCameraSettings.doRecording = false;   //default false
	omxCameraSettings.enablePixels = true;
	//pass in the settings and it will start the camera
	videoGrabber.setup(omxCameraSettings);
	videoGrabber.stopRecording();

	ofLogNotice("recorder::init, using RPI camera");
	shader.load("shaders//shader_GLES.vert", "shaders//shader_GLES.frag", "");
	ofLogNotice("Loading RPI/mobile shader");

#else
	//we can now get back a list of devices.
	vector<ofVideoDevice> devices = videoGrabber.listDevices();
	ofLogNotice("recorder::init, cameraID: " + ofToString(cameraID));
	ofLogNotice("recorder::init, width: " + ofToString(camWidth) + " height: " + ofToString(camHeight));
	ofLogNotice("recorder::init, targetWidth: " + ofToString(targetWidth) + " targetHeight: " + ofToString(targetHeight));
	videoGrabber.setDeviceID(cameraID);
	videoGrabber.initGrabber(camWidth, camHeight);
	videoGrabber.setDesiredFrameRate(30);
	shader.load("shaders/shader.vert", "shaders/shader.frag", "");
	ofLogNotice("Loading desktop shader");
#endif


	//we can now get back a list of devices.
	/*vector<ofVideoDevice> devices = vidGrabber.listDevices();
	int chosen = 0;
	for (int i = 0; i < devices.size(); i++){
	cout << devices[i].id << ": " << devices[i].deviceName;
	if (devices[i].bAvailable){
	if(devices[i].deviceName=="PS3Eye Camera") {
	chosen = i;
	}
	}
	else{
	cout << " - unavailable " << endl;
	}
	}*/
	indexSavedPhoto = 0;
	isRecording = false;
	videoPixels = new unsigned char[targetWidth * targetHeight * 3];
	videoTexture.allocate(targetWidth, targetHeight, GL_RGB);

	iEffect = 0;
	iChromatic = 0.8f;
	useShader = true;

	fbo.allocate(targetWidth, targetHeight, GL_RGB);

	colorGifEncoder.setup(targetWidth, targetHeight, .2, 256);

	// ascii effect
	font.loadFont("Courier New Bold.ttf", 9);
	asciiCharacters = string("  ..,,,'''``--_:;^^**""=+<>iv%&xclrs)/){}I?!][1taeo7zjLunT#@JCwfy325Fp6mqSghVd4EgXPGZbYkOA8U$KHDBWNMR0Q");
	asciiFbo.allocate(targetWidth, targetHeight);

}

void recorder::update() {
	if (isRecording) {
		string filename;
		if (indexSavedPhoto < 10) filename = "seq00" + ofToString(indexSavedPhoto);
		if (indexSavedPhoto >= 10 && indexSavedPhoto < 100) filename = "seq0" + ofToString(indexSavedPhoto);
		if (indexSavedPhoto >= 100 && indexSavedPhoto < 1000) filename = "seq" + ofToString(indexSavedPhoto);
		ofLogNotice("recorder::update, saving: " + filename);
#ifdef TARGET_OPENGLES
		//ofPixelsRef pixelsRef = (unsigned char *)videoGrabber.getPixels();
		//pixels.setFromExternalPixels((unsigned char *)videoGrabber.getPixels(), cameraWidth, cameraHeight, OF_PIXELS_RGBA);
		if (videoGrabber.isFrameNew())
		{
			videoTexture.loadData(videoGrabber.getPixels(), omxCameraSettings.width, omxCameraSettings.height, GL_RGBA);
		}

#else
		videoGrabber.update();
		if (videoGrabber.isFrameNew()) {
			videoTexture = videoGrabber.getTextureReference();
		}
		/*ofPixelsRef pixelsRef = videoGrabber.getPixelsRef();
		savedImage.setFromPixels(pixelsRef);
		savedImage.setImageType(OF_IMAGE_COLOR);
		savedImage.saveImage("tmp/" + filename + ".tga");	
		// ascii
		asciiFbo.begin();
		ofSetHexColor(0xffffff);
		for (int i = 0; i < camWidth; i += 7) {
			for (int j = 0; j < camHeight; j += 9) {
				// get the pixel and its lightness (lightness is the average of its RGB values)
				float lightness = pixelsRef.getColor(i, j).getLightness();
				// calculate the index of the character from our asciiCharacters array
				int character = powf(ofMap(lightness, 0, 255, 0, 1), 2.5) * asciiCharacters.size();
				// draw the character at the correct location
				font.drawString(ofToString(asciiCharacters[character]), i, j);
			}
		}
		asciiFbo.end();
		noizzz = asciiFbo.getTextureReference();		
		*/
#endif		


		// shader
		fbo.begin();
		if (ofRandom(0, 1) > 0.5) {
			ofClear(0, 0, 0, 0);
		}
		shader.begin();
		shader.setUniformTexture("iChannel0", videoTexture, 1);
		// TODO shader.setUniformTexture("iChannel1", noizzz, 2);
		shader.setUniform3f("iResolution", targetWidth, targetHeight, 0);
		shader.setUniform2f("iMouse", indexSavedPhoto, indexSavedPhoto);
		shader.setUniform1f("iGlobalTime", ofGetElapsedTimef());
		shader.setUniform4f("iDate", ofGetYear(), ofGetMonth(), ofGetDay(), ofGetSeconds());
		shader.setUniform1i("iEffect", floor(ofRandom(0, 4.9)));
		shader.setUniform1f("iChromatic", ofRandom(-1, 1));
		shader.setUniform1f("iShift", ofRandom(-1.0, 1.0));
		shader.setUniform1f("iGlitch", ofRandom(0.0, 1.0));
		shader.setUniform1f("iPixelate", ofRandom(0.7, 1.0));

		//ofRect(0, 0, targetWidth, targetHeight);
		// Draw the texture
		videoTexture.draw(0, 0);
		shader.end();
		// Stop rendering to the framebuffer and copy out the pixels to CPU memory.
		fbo.end();

		fbo.readToPixels(pix);
		fbo.draw(300, 10, camWidth, camHeight);
		//pix.resize(targetWidth, targetHeight, OF_INTERPOLATE_NEAREST_NEIGHBOR);
		savedImage.setFromPixels(pix);
		savedImage.setImageType(OF_IMAGE_COLOR);


#ifdef TARGET_OPENGLES
		savedImage.saveImage("buffer//" + filename + ".tga");
#else
		savedImage.saveImage("buffer/" + filename + ".tga");
#endif
		//_savedImage.saveImage("photos/"+filename+".jpg", OF_IMAGE_QUALITY_HIGH);
		//_savedImage.saveImage("photos/" + filename + ".tif");
		//);
		// add frame to gif encoder
		colorGifEncoder.addFrame(
			pix.getPixels(),
			pix.getWidth(),
			pix.getHeight(),
			pix.getBitsPerPixel()/*,
								 .1f duration */
		);

		pix.clear();
		savedImage.clear();
		indexSavedPhoto++;
		if (indexSavedPhoto == (amountOfFrames + 1)) {
			isRecording = false;
#ifdef TARGET_OPENGLES
			videoGrabber.stopRecording();
#endif
			indexSavedPhoto = 0;
			saveGif();
		}
	}
	//}
}
void recorder::saveGif()
{

	string fileName = ofToString(ofGetMonth()) + "-" + ofToString(ofGetDay()) + "-" + ofToString(ofGetHours()) + "-" + ofToString(ofGetMinutes()) + "-" + ofToString(ofGetSeconds());
	cout << "saveGif: " << fileName << endl;
	//gifEncoder.save("gif/" + fileName + ".gif");
#ifdef TARGET_OPENGLES
	colorGifEncoder.save("gif//" + fileName + ".gif");
#else
	colorGifEncoder.save("gif/" + fileName + ".gif");
#endif
}
recorder::~recorder()
{
	//dtor
}
