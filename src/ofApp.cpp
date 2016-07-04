#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetLogLevel("ofThread", OF_LOG_ERROR);

	ofEnableAlphaBlending();

	doDrawInfo = true;
	validationMode = false;
	targetWidth = 640;
	targetHeight = 480;
#if defined(TARGET_OPENGLES)
	consoleListener.setup(this);
	omxCameraSettings.width = targetWidth;
	omxCameraSettings.height = targetHeight;
	omxCameraSettings.framerate = 15;
	omxCameraSettings.enableTexture = true;

	videoGrabber.setup(omxCameraSettings);
	filterCollection.setup();
	ofSetVerticalSync(false);
	shader.load("shaderRpi");
#else
	videoGrabber.setDeviceID(0);
	videoGrabber.setDesiredFrameRate(30);
	videoGrabber.setup(targetWidth, targetHeight);
	ofSetVerticalSync(true);
	shader.load("shaderDesktop");
#endif
	doShader = true;
	iEffect = 2;

	fbo.allocate(targetWidth, targetHeight);
	fbo.begin();
	ofClear(0, 0, 0, 0);
	fbo.end();
	// selfberry
	colorGifEncoder.setup(targetWidth, targetHeight, .2, 256);
	ofAddListener(ofxGifEncoder::OFX_GIF_SAVE_FINISHED, this, &ofApp::onGifSaved);

	videoTexture.allocate(targetWidth, targetHeight, GL_RGB);

	bufferDir = "buffer";

	timeZero = ofGetElapsedTimeMicros();
	frameNumber = 0;
	slotRecording = 255;
	slotAmount = 4;
	if (dirSRC.doesDirectoryExist(bufferDir)) {
		dirSRC.removeDirectory(bufferDir, true);
	}
	if (!dirSRC.doesDirectoryExist("slot1")) {
		dirSRC.createDirectory("slot1");
	}
	if (!dirSRC.doesDirectoryExist("slot2")) {
		dirSRC.createDirectory("slot2");
	}
	if (!dirSRC.doesDirectoryExist("slot3")) {
		dirSRC.createDirectory("slot3");
	}
	if (!dirSRC.doesDirectoryExist("tmp")) {
		dirSRC.createDirectory("tmp");
	}
	if (!dirSRC.doesDirectoryExist("gif")) {
		dirSRC.createDirectory("gif");
	}
	dirSRC.createDirectory(bufferDir);
	indexSavedPhoto = 0;
	isRecording = false;
	amountOfFrames = 10;
	maxFrames = 10;
	if (settings.loadFile("settings.xml") == false) {
		ofLog() << "XML ERROR, possibly quit";
	}
	settings.pushTag("settings");

	slotDir = "slot";
	for (int i = 0; i < settings.getNumTags("slot"); i++) {
		settings.pushTag("slot", i);
		videoGrid[i].init(settings.getValue("id", i), settings.getValue("x", 700), settings.getValue("y", 500), &slotDir, settings.getValue("key", 0));
		settings.popTag();
	}
	gifFileName = "";
	lastSpot = 0;
	currentDisplaySlot = 1;
	bkgLayer.loadImage("ui.png");
	valideLayer.loadImage("valide.png");
	sourisitepajoli.loadImage("sourisitepajoli.png");
	trois.loadImage("trois.png");
	deux.loadImage("deux.png");
	un.loadImage("un.png");
	qrcode.loadImage("qrcode.jpg");
	ftpClient.setup("videodromm.com", "u39314325-selfberry", "tocs2016!");
	ftpClient.setVerbose(true);
}

//--------------------------------------------------------------
void ofApp::update()
{
#if defined(TARGET_OPENGLES)

#else
	videoGrabber.update();
#endif
	if (!doShader || !videoGrabber.isFrameNew())
	{
		//ofLogNotice("update() !videoGrabber.isFrameNew return");
		return;
	}
	//ofLogNotice("update() fbo begin");
	fbo.begin();
	ofClear(1, 1, 0, 0);
	shader.begin();
	shader.setUniform1f("time", ofGetElapsedTimef());
	shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	shader.setUniform3f("iResolution", ofGetWidth(), ofGetHeight(), 0);
	shader.setUniform2f("iMouse", indexSavedPhoto, indexSavedPhoto);
	shader.setUniform1f("iGlobalTime", ofGetElapsedTimef());
	shader.setUniform4f("iDate", ofGetYear(), ofGetMonth(), ofGetDay(), ofGetSeconds());
	shader.setUniform1i("iEffect", iEffect);// floor(ofRandom(0, 4.9)));
	shader.setUniform1f("iChromatic", ofRandom(-1, 1));
	shader.setUniform1f("iShift", ofRandom(-1.0, 1.0));
	shader.setUniform1f("iGlitch", ofRandom(0.0, 1.0));
	shader.setUniform1f("iPixelate", ofRandom(0.7, 1.0));
#if defined(TARGET_OPENGLES)
	shader.setUniformTexture("tex0", videoGrabber.getTextureReference(), videoGrabber.getTextureID());
	videoGrabber.draw();
#else
	shader.setUniformTexture("tex0", videoGrabber.getTexture(), 0);// 0 or 1?
	videoGrabber.draw(0, 0);
#endif
	shader.end();
	fbo.end();
	//ofLogNotice("update() fbo end");

	if (isRecording == true) {
		finalCountdown = ofGetSeconds() - startSecond;
		ofLogNotice("update() rec " + ofToString(finalCountdown) + "=" + ofToString(ofGetSeconds()) + "-" + ofToString(startSecond));
		if (finalCountdown > 2 || finalCountdown < 0) {
			dirSRC.createDirectory(bufferDir);
			dirSRC.listDir(bufferDir);
			recordedFramesAmount = dirSRC.size();
			ofLogNotice("AMOUNT OF FILES: " + ofToString(recordedFramesAmount) + "/" + ofToString(maxFrames));
			if (recordedFramesAmount == maxFrames) {
				isRecording = false;
				indexSavedPhoto = 0;
				ofLogNotice("update() stop recording");
			}
			else {
				if (videoGrabber.isFrameNew()) {
					ofLogNotice("update() isFrameNew");

					string filename;
					string path;
					if (indexSavedPhoto < 10) filename = "seq00" + ofToString(indexSavedPhoto) + ".tga";
					if (indexSavedPhoto >= 10 && indexSavedPhoto < 100) filename = "seq0" + ofToString(indexSavedPhoto) + ".tga";
					if (indexSavedPhoto >= 100) filename = "seq" + ofToString(indexSavedPhoto) + ".tga";
					path = "slot" + ofToString(currentDisplaySlot) + "//";
					// fbo to pixels
					fbo.readToPixels(pix);
					fbo.draw(0, 0, targetWidth, targetHeight);
					ofLogNotice("AMOUNT OF FILES: " + ofToString(recordedFramesAmount) + "/" + ofToString(maxFrames));

					//pix.resize(targetWidth, targetHeight, OF_INTERPOLATE_NEAREST_NEIGHBOR);
					savedImage.setFromPixels(pix);
					savedImage.setImageType(OF_IMAGE_COLOR);
					savedImage.saveImage(path + filename);

					ofLogNotice("update() currentDisplaySlot " + ofToString(currentDisplaySlot));

					savedImage.saveImage(bufferDir + "//" + filename);

					// add frame to gif encoder
					colorGifEncoder.addFrame(
						pix.getPixels(),
						targetWidth,
						targetHeight,
						pix.getBitsPerPixel()/*,
											 .1f duration */
					);

					recordedFramesAmount++;
					pix.clear();
					savedImage.clear();
					indexSavedPhoto++;
					if (indexSavedPhoto == amountOfFrames) {
						ofLogNotice("Stop recording: " + ofToString(indexSavedPhoto) + "/" + ofToString(amountOfFrames));
						isRecording = false;
						indexSavedPhoto = 0;
						saveGif();
					}
				}
			}
		}

	}
	for (i = 1; i < slotAmount; i++) {
		videoGrid[i].loadFrameNumber(frameNumber);
	}
	frameNumber++;
	if (frameNumber == maxFrames) {
		frameNumber = 0;
	}
}
void ofApp::saveGif()
{
	gifFileName = ofToString(ofGetMonth()) + "-" + ofToString(ofGetDay()) + "-" + ofToString(ofGetHours()) + "-" + ofToString(ofGetMinutes()) + "-" + ofToString(ofGetSeconds()) + ".gif";
	ofLogNotice("saveGif: " + gifFileName);
	colorGifEncoder.save("gif//" + gifFileName);
	ofLogNotice("saveGif end");
}
void ofApp::onGifSaved(string & fileName) {
	cout << "gif saved as " << fileName << endl;
	ofLogNotice("onGifSaved: " + fileName);
	colorGifEncoder.reset();
	ofLogNotice("onGifSaved reset");
	validationMode = true;
}
//--------------------------------------------------------------
void ofApp::draw() {

	ofClear(0, 0, 0, 0);
	stringstream info;
	info << "APP FPS: " << ofToString(ofGetFrameRate()) << " sec: " << ofToString(ofGetSeconds()) << " st: " << ofToString(startSecond) << " fin: " << ofToString(finalCountdown) << "\n";
	info << "SHADER ENABLED: " << doShader << "\n";
	info << "Display slot: " << ofToString(currentDisplaySlot) << "\n";
	info << "Effet: " << ofToString(iEffect) << "\n";

	if (doShader)
	{
#if defined(TARGET_OPENGLES)
		fbo.draw(330, 13);
#else
		videoGrabber.draw(330, 13);
#endif
	}
	else
	{
#if defined(TARGET_OPENGLES)
		videoGrabber.draw();
#else
		videoGrabber.draw(330, 13);
#endif
	}
	for (int i = 1; i < slotAmount; i++) {
		videoGrid[i].draw();
	}
#if defined(TARGET_OPENGLES)
	info << "Resolution Camera: " << videoGrabber.getWidth() << "x" << videoGrabber.getHeight() << " @ " << videoGrabber.getFrameRate() << "FPS" << "\n";
	info << "FILTRE: " << filterCollection.getCurrentFilterName() << "\n";
#endif

	info << "\n";
	info << "VERT: changement de filtre" << "\n";
	info << "ROUGE: enregistrer" << "\n";
	info << "BLANC: annuler" << "\n";
	info << "JAUNE: partager" << "\n";
	bkgLayer.draw(0, 0);
	if (validationMode) {
		valideLayer.draw(0, 0);
	}
	if (isRecording) {
		sourisitepajoli.draw(400, 0);
		switch (finalCountdown) {
		case 0:
			trois.draw(400, 0);
			break;
		case 1:
			deux.draw(400, 0);
			break;
		case 2:
			un.draw(400, 0);
			break;
		}
	}
	qrcode.draw(10, 700 );//914, 450
	if (doDrawInfo) {
		ofDrawBitmapStringHighlight(info.str(), 950, 940, ofColor::black, ofColor::yellow);
	}
}
void ofApp::fetch(const std::string& data, size_t size, size_t margin)
{
	std::stringstream googleChartsQRurl;
	googleChartsQRurl
		<< "http://chart.googleapis.com/chart?"   // Google Charts Endpoint
		<< "chs=" << size << "x" << size << "&"   // chart size
		<< "cht=qr&"                              // chart type: QR Code
		<< "chld=L|" << margin << "&"             // Error correction [L,M,Q,H], margin
		<< "choe=UTF-8&"                          // Encoding
		<< "chl=" << data;                        // QR code data

	ofRegisterURLNotification(this);
	ofLoadURLAsync(googleChartsQRurl.str(), "qrcode");
}

void ofApp::urlResponse(ofHttpResponse& response)
{
	if (response.request.name == "qrcode")
	{
		if (response.status == 200) {
			qrcode.loadImage(response.data);
			qrcode.saveImage("qrcode.jpg");
		}

		ofUnregisterURLNotification(this);
	}
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	//stringstream html;
	//stringstream html2;
	string htmlFileName;
	string htmlFileName2;
	ofLogNotice("PRESSED KEY: " + ofToString(key));

	switch (key) {
	case 65:  // vert 65 357
	//case 357:
#if defined(TARGET_OPENGLES)
		videoGrabber.setImageFilter(filterCollection.getNextFilter());
#endif
		break;
	case 10: // rouge 10
	case 13: //	Entree
		if (!isRecording) {
			isRecording = true;
			indexSavedPhoto = 0;
			currentDisplaySlot++;
			if (currentDisplaySlot > 3) currentDisplaySlot = 1;
			bufferDir = ofToString(ofGetMonth()) + "-" + ofToString(ofGetDay()) + "-" + ofToString(ofGetHours()) + "-" + ofToString(ofGetMinutes()) + "-" + ofToString(ofGetSeconds());
			startSecond = ofGetSeconds();
		}
		break;
	case 126: //blanc 126 127
	case 120: // x
	case 88: // X
			  //doDrawInfo = !doDrawInfo;
		validationMode = false;
		iEffect = 0;
		currentDisplaySlot++;
		if (currentDisplaySlot > 3) currentDisplaySlot = 1;

		break;
	case 67: // jaune 67 0		
	case 118: // v		
	case 86: // V		
		validationMode = false;
		iEffect = 1;
		if (gifFileName.length() > 0) {
			if (ftpClient.send(gifFileName, ofToDataPath("gif"), "/gif/") > 0) {
				ofLogNotice("Transfert ftp reussi\n" + gifFileName + ", creation fichier html");
				gifValides.push_back(gifFileName);
				ofFile html(htmlFileName, ofFile::WriteOnly);
				html << "<!DOCTYPE html><html><head><title>Selfberry</title><meta http-equiv=\"refresh\" content=\"30\"><style>body{background-color: #111111;}</style></head><body>";
				for (unsigned int i = 0; i < 60; i++) {
					html << "<a href=\"" << i << ".html\">" << i << " </a>";
				}
				for (auto gifFile : gifValides) {
					html << "<img src=\"gif/" << gifFile << "\" /><br />";
					html << "<a href=\"http://www.facebook.com/share.php?u=http://videodromm.com/selfberry/" << gifFile << ".html\" target=\"_blank\"><button class=\"btn btn-social btn-facebook\"><span class =\"icon icon-facebook\"></span>Partager sur Facebook</button></a>";
					html << "<a href=\"https://twitter.com/home?status=#selfberry%20http://videodromm.com/selfberry/" << gifFile << ".html\">Twitter</a><br />";
				}
				html << "</body></html>";
				// ecriture index.html
				htmlFileName = "index.html";
				// pas sur rpi.. ofBufferToFile(htmlFileName, html);

				if (ftpClient.send(htmlFileName, ofToDataPath(""), "/") > 0) {
					// 
					htmlFileName2 = gifFileName + ".html";
					ofFile html2(htmlFileName2, ofFile::WriteOnly);
					html2 << "<!DOCTYPE html><html><head><title>Selfberry</title><style>body{background-color: #111111;}</style></head><body>";
					html2 << "<img src=\"gif/" << gifFileName << "\" /><br />";
					html2 << "<a href = \"http://www.facebook.com/share.php?u=http://videodromm.com/selfberry/" << htmlFileName2 << "\" target=\"_blank\"><button class=\"btn btn-social btn-facebook\"><span class =\"icon icon-facebook\"></span>Partager sur Facebook</button></a>";
					html2 << "</body></html>";

					// pas sur rpi.. ofBufferToFile(htmlFileName2, html2);
					if (ftpClient.send(htmlFileName2, ofToDataPath(""), "/") > 0) {
						fetch("videodromm.com/selfberry/" + htmlFileName2, 300, 1);
					}
				}
			}
		}
		break;
	case 66: // bleu 66 0		
	case 112: // p		
	case 80: // P		
		iEffect = 2;
		ofSaveScreen(ofToString(ofGetSeconds()) + ".jpg");
		break;
	case 50:
	case 359:
		currentDisplaySlot = 1;
		//doShader = !doShader;
		iEffect = 3;
		break;
	}
}
#if defined(TARGET_OPENGLES)
void ofApp::onCharacterReceived(KeyListenerEventData& e)
{
	keyPressed((int)e.character);
}
#endif	
