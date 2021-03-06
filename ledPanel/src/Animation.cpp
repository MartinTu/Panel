/*
 * Animation.cpp
 *
 *  Created on: 06.09.2014
 *      Author: keKs
 */


#include "Animation.h"


Animation::Animation(int _width, int _height) :
		Layer(_width, _height), aniSkipIterator(0), runningAni(aniNone) {
	plate->append(color_white);
	plate->append(color_black);
	plate->append(color_red);

	/* [0]: frameSkip
	 * [1]: targetPix
	 * [2]: length
	 * [3]: direction
	 * [4]: dropDiff
	 * [5]: pixRed
	 * [6]: pixGreen
	 * [7]: pixBlue
	 */

	/*

	 animation_t startingAni = aniDirFallingPixel;
	 parameter.clear();
	 parameter.push_back(0x04);  //frameSkip
	 parameter.push_back(0x14);
	 parameter.push_back(0x06);
	 parameter.push_back(0x01);
	 parameter.push_back(0xf);//random
	 parameter.push_back(0x00);  //red
	 parameter.push_back(0x60);
	 parameter.push_back(0x00);
	 */
	/*

	 * [0]: frameSkip
	 * [1]: targetPixHigh
	 * [2]: targetPixLow
	 * [3]: fadeSpeed
	 * [4]: pixRed
	 * [5]: pixGreen
	 * [6]: pixBlue
	 */
	 /*
	animation_t startingAni = aniFadingPixels;
	parameter.clear();
	parameter.push_back(0);
	parameter.push_back(0x0);
	parameter.push_back(0x64);
	parameter.push_back(0x02);
	parameter.push_back(0xff);
	parameter.push_back(0x46);
	parameter.push_back(0);
	*/
	animation_t startingAni = aniPlasma;
	parameter.clear();
	parameter.push_back(0);
	parameter.push_back(0xff);
	parameter.push_back(0xff);
	//set starting ani
	this->set(startingAni, parameter, parameter.length());
}

Animation::~Animation() {
}

bool Animation::isFrameNotSkipped(unsigned int skip) {
	if (aniSkipIterator == 0) {
		//update with new skipvalue depending on animation
		aniSkipIterator = skip;
		return true;
	}
	aniSkipIterator--;
	return false;
}

int Animation::set(uint8_t id, string &param, unsigned int paramSize) {
	int ret = -1;
	//backup last animationID
	animation_t lastAni = runningAni;
	if (id < _aniNUM) {
		runningAni = static_cast<animation_t>(id);
	} else {
		cerr << id << " is no animation_t" << endl;
		runningAni = aniNone;
	}

	if (runningAni != lastAni) {
		//dont skip next frame
		aniSkipIterator = 0;
	}

	switch (runningAni) {
	case aniNone:
		cout << "aniNone";
		reset();
		break;
	case aniInvader:
		cout << "[INFO] aniInvader";
		if (paramSize == (unsigned int) lenParamInvader) {
			parameter = param.substr(0, paramSize);
			ret = 0;
			if (runningAni != lastAni) {
				//initialize intern parameter
				internPar.clear();
				internPar.push_back(0x00);
			}
		} else {
			cerr << " param out of bounds: " << paramSize << endl;
		}
		break;
	case aniDirectionFade:
		cout << "[INFO] aniDirectionFade";
		if (paramSize == (unsigned int) lenParamDirectionFade) {
			parameter = param.substr(0, paramSize);
			//frameSkip = 3;
			ret = 0;
			if (runningAni != lastAni) {
				//initialize intern parameter
				internPar.clear();
				internPar.push_back(0xff);  //red
				internPar.push_back(0x00);  //green
				internPar.push_back(0x00);  //blue
			}
		} else {
			cerr << " param out of bounds: " << paramSize << endl;
		}
		break;
	case aniScreenFade:
		cout << "[INFO] aniScreenFade";
		if (paramSize == (unsigned int) lenParamScreenFade) {
			parameter = param.substr(0, paramSize);
			ret = 0;
			if (runningAni != lastAni) {
				//initialize intern parameter
				internPar.clear();
				internPar.push_back(0x00);  //fadeStatus
				internPar.push_back(0xff);  //red
				internPar.push_back(0x00);  //green
				internPar.push_back(0x00);  //blue
			}
		} else {
			cerr << " param out of bounds: " << paramSize << endl;
		}
		break;
	case aniScreenPulse:
		cout << "[INFO] aniScreenPulse";
		if (paramSize == (unsigned int) lenParamScreenPulse) {
			parameter = param.substr(0, paramSize);
			ret = 0;
			if (runningAni != lastAni) {
				//initialize intern parameter
				internPar.clear();
				internPar.push_back(0xff);  //brightness = 255
				internPar.push_back(false);  //getsBrighter = false
			}
		} else {
			cerr << " param out of bounds: " << paramSize << endl;
		}
		break;
	case aniRotateLine:
		cout << "[INFO] aniRotateLine";
		if (paramSize == (unsigned int) lenParamRotateLine) {
			parameter = param.substr(0, paramSize);
			ret = 0;
			if (runningAni != lastAni) {
				//initialize intern parameter
				internPar.clear();
				internPar.push_back(0x00);  //p_x1
				internPar.push_back(0x00);  //p_y1
			}
		} else {
			cerr << " param out of bounds: " << paramSize << endl;
		}
		break;
	case aniWaterdrop:
		cout << "[INFO] aniWaterdrop";
		if (paramSize == (unsigned int) lenParamWaterdrop) {
			parameter = param.substr(0, paramSize);
			ret = 0;
			if (runningAni != lastAni) {
				//initialize intern parameter
				internPar.clear();
				internPar.push_back((uint8_t) rand() % (width - 1));  //x
				internPar.push_back((uint8_t) rand() % (height - 1));  //y
				internPar.push_back(0x00);  //radius
			}
		} else {
			cerr << " param out of bounds: " << paramSize << endl;
		}
		break;
	case aniFadingPixels:
		cout << "[INFO] aniFadingPixels";
		if (paramSize == (unsigned int) lenParamFadingPixels) {
			parameter = param.substr(0, paramSize);
			ret = 0;
			if (runningAni != lastAni) {
				//initialize intern parameter
				internPar.clear();
				internPar.push_back(0x00);  //targetPixOldHigh
				internPar.push_back(0x00);  //targetPixOldLow
			}
		} else {
			cerr << " param out of bounds: " << paramSize << endl;
		}
		break;
	case aniDirFallingPixel:
		cout << "[INFO] aniDirFallingPixel";
		if (paramSize == (unsigned int) lenParamDirFallingPixel) {
			parameter = param.substr(0, paramSize);
			ret = 0;
			if (runningAni != lastAni) {
				//initialize intern parameter
				internPar.clear();
				internPar.push_back(0x00);  //targetPixOld
				internPar.push_back(0x00);  //dirOld
				internPar.push_back(0x00);  //dropSpeedIterator
			}
		} else {
			cerr << " param out of bounds: " << paramSize << endl;
		}
		break;
	case aniPlasma:
		cout << "[INFO] aniPlasma";
		if (paramSize == (unsigned int) lenParamPlasma) {
			parameter = param.substr(0, paramSize);
			ret = 0;
			if (runningAni != lastAni) {
				//initialize intern parameter
				internPar.clear();
				internPar.push_back(0x00);  //time1
				internPar.push_back(0x00);	//time2
			}
		} else {
			cerr << " param out of bounds: " << paramSize << endl;
		}
		break;
	default:
		break;
	}
	if (ret == 0) {
		if (!param.empty()) {
			cout << ": param(0x";
			for (unsigned int i = 0; i < parameter.length(); i++) {
				cout << hex << setw(3) << (int) parameter[i];
			}
			cout << ") ";
			if (internPar.empty()) {
				cout << endl;
			}
		}
		if (!internPar.empty()) {
			cout << "internPar(0x";
			for (unsigned int i = 0; i < internPar.length(); i++) {
				cout << hex << setw(3) << (int) internPar[i];
			}
			cout << ")" << endl;
		}
		return 0;
	}
	//reset to animation from before
	runningAni = lastAni;
	cerr << "animation set() error" << endl;
	return -1;  //error at paramSize
}

void Animation::reset() {
	runningAni = aniNone;
	aniSkipIterator = 0;
	parameter.clear();
	internPar.clear();
}

bool Animation::nextStep() {
	if (mixer != mixOff) {
//        cout << "aniParam: 0x";
//        for (unsigned int i = 0; i < parameter.length(); i++)
//        {
//            cout << hex << setw(3) << (int) parameter[i];
//        }
//        cout << endl;
		switch (runningAni) {
		case aniInvader:
			invader();
			break;
		case aniDirectionFade:
			directionFade();
			break;
		case aniScreenFade:
			screenFade();
			break;
		case aniScreenPulse:
			screenPulse();
			break;
		case aniRotateLine:
			rotateLine();
			break;
		case aniWaterdrop:
			waterdrop();
			break;
		case aniFadingPixels:
			fadingPixels();
			break;
		case aniDirFallingPixel:
			dirFallingPixel();
			break;
		case aniPlasma:
			plasma();
			break;
		default:
			return 0;
		}
		return 1;
	} else {
		return 0;
	}
}

bool Animation::isAnimationRunning() {
	if (runningAni) {  //if (not noAnimation = 0x00)
		return true;
	}
	return false;
}

void Animation::invader() {
	/* ani 0x01
	 * paramSize : 3
	 * [0]: skipFrame
	 * [1]: colorDimmer
	 * [2]: saturation
	 * internPar: 1
	 * [0]: actWheel//0x00
	 */
	if (isFrameNotSkipped(parameter[0])) {
		color_t color;
		float colorDimmer = (float) (parameter[1] / 255.0);
		uint8_t saturation = (uint8_t) parameter[2];
		int invaderOK = width * height;
		bool picture;
		float actWheel = (float)internPar[0] /255.0;
		if (actWheel < 128) {
			picture = true;
		} else {
			picture = false;
		}
		float pictureWheel;
		//check resolution of DISPLAY and correct config
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				actWheel += 1.0/height/width;
				pictureWheel = actWheel;
				if (invaderOK == 200) {  //max
					if (picture) {
						if (bit_invader1_10x20[y][x * 3]) {
							pictureWheel += 0.5;
							if(pictureWheel > 1)
								pictureWheel -= 1.0;
						}
					} else {
						//int nx = x - 1;
						int nx = x;
						if (nx < 0) {
							nx = nx + width;
						}
						if (bit_invader2_10x20[y][nx * 3]) {
							pictureWheel += 0.5;
							if(pictureWheel > 1)
								pictureWheel -= 1.0;
						}
					}
				} else if (invaderOK == 144) {  //martin
					if (bit_invader[y][x * 3]) {
						pictureWheel += 0.5;
							if(pictureWheel > 1)
								pictureWheel -= 1.0;
					}
				}
				color = plate->getColor(pictureWheel);
				color.setDimmer(colorDimmer);
				color.setMinSaturation(saturation);
				frame->setPixel(x, y, color, 1);
			}
		}
		internPar[0]++;
	}

}

void Animation::directionFade() {
	/* ani 0x02
	 * paramSize: 5
	 * [0]: skipFrame
	 * [1]: delta
	 * [2]: direction //original color comes from reversed direction
	 * [3]: colorDimmer
	 * [4]: saturation
	 * internPar: 3
	 * [0]: red//0xff
	 * [1]: green//0x00
	 * [2]: blue//0x00
	 */
	if (isFrameNotSkipped(parameter[0])) {
		color_t color((uint8_t) internPar[0], (uint8_t) internPar[1],
				(uint8_t) internPar[2]);
		uint8_t delta = (uint8_t) parameter[1];
		direction_t dir = static_cast<direction_t>(parameter[2]);
		float colorDimmer = parameter[3] / 255.0;
		uint8_t saturation = (uint8_t) parameter[4];
		color_t calcColor(color);

		int xmax = width;
		int ymax = height;
		int w = width - 1;
		int h = height - 1;
		switch (dir) {

		case dirLeft:
			break;
		case dirLeftBot:
			xmax = height;
			ymax = width;
			xmax += w;
			break;
		case dirBottom:
			xmax = height;
			ymax = width;
			break;
		case dirRightBot:
			xmax = height;
			ymax = width;
			xmax += w;
			break;
		case dirRight:
			break;
		case dirRightTop:
			xmax += h;
			break;
		case dirTop:
			xmax = height;
			ymax = width;
			break;
		case dirLeftTop:
			xmax += h;
			break;
		default:
			break;
		}

		for (uint8_t x = 0; x < xmax; x++) {

			calcColor = color;
			calcColor.setDimmer(colorDimmer);
			calcColor.setSaturation(saturation);
			for (uint8_t y = 0; y < ymax; y++) {
				switch (dir) {
				case dirLeft: {
					frame->setPixel(x, y, calcColor, 1);
					break;
				}
				case dirLeftBot: {
					int xi = x - y;
					if (Utils::isInRange(xi, 0, h))
						frame->setPixel(y,h - xi, calcColor, 1);
					break;
				}
				case dirBottom: {
					frame->setPixel(y, h - x, calcColor, 1);
					break;
				}
				case dirRightBot: {
					int xi = x - y;
					if (Utils::isInRange(xi, 0, h))
						frame->setPixel(w - y, h - xi, calcColor, 1);
					break;
				}
				case dirRight: {
					frame->setPixel(w - x, y, calcColor, 1);
					break;
				}
				case dirRightTop: {
					int xi = x - y;
					if (Utils::isInRange(xi, 0, w))
						frame->setPixel(w - xi, y, calcColor, 1);
					break;
				}
				case dirTop: {
					frame->setPixel(y, x, calcColor, 1);
					break;
				}
				case dirLeftTop: {
					int xi = x - y;
					if (Utils::isInRange(xi, 0, w))
						frame->setPixel(xi, y, calcColor, 1);
					break;
				}
				default:
					break;
				}
			}
			//fade original color
			color.rgbFade(delta);

			/* set new start color */
			if (x == 1) {
				internPar[0] = color.red;
				internPar[1] = color.green;
				internPar[2] = color.blue;
			}
		}
	}
}

void Animation::screenFade() {
	/* ani 0x03
	 * paramSize: 4
	 * [0]: skipFrame
	 * [1]: delta
	 * [2]: colorDimmer
	 * [3]: saturation
	 * internPar: 4
	 * [0]: fadeStatus//0x00
	 * [1]: red//0xff
	 * [2]: green//0x00
	 * [3]: blue//0x00
	 */
	if (isFrameNotSkipped(parameter[0])) {
		int delta = parameter[1];
		float maxBrightness = parameter[2] / 255.0;
		uint8_t fadeStatus = (uint8_t) internPar[0];
		int saturation = parameter[3];
		color_t color(internPar[1], internPar[2], internPar[3]);
		color_t screenColor(color);
		screenColor.setDimmer(maxBrightness);
		screenColor.setSaturation(saturation);
		frame->setColor(screenColor);
		if (!delta) {
			delta = 1;
		}
		if (0x0 == fadeStatus) {
			color.red = Utils::addInBounds(color.red, -delta);
			color.green = Utils::addInBounds(color.green, delta);
			if (0 == color.red)
				fadeStatus++;
		} else if (0x01 == fadeStatus) {
			color.green = Utils::addInBounds(color.green, -delta);
			color.blue = Utils::addInBounds(color.blue, delta);
			if (0 == color.green)
				fadeStatus++;
		} else if (0x02 == fadeStatus) {
			color.blue = Utils::addInBounds(color.blue, -delta);
			color.red = Utils::addInBounds(color.red, delta);
			if (0 == color.blue)
				fadeStatus = 0;
		}
		internPar[0] = fadeStatus;
		internPar[1] = color.red;
		internPar[2] = color.green;
		internPar[3] = color.blue;
	}
}

void Animation::screenPulse() {
	/* ani 0x04
	 * paramSize: 2
	 * [0]: skipFrame
	 * [1]: delta//should be 1
	 * internPar: 2
	 * [0]: brightness//0xff
	 * [1]: getsBrighter//false
	 */
	if (isFrameNotSkipped(parameter[0])) {
		float val = internPar[0] / 255.0;
		color_t color(plate->getLastColor());
		int delta = parameter[1];
		if (!delta) {
			delta = 1;
		}
		color.setDimmer(val);

		if (internPar[1]) {
			internPar[0] = Utils::addInBounds(internPar[0], delta);
			if (0xff == internPar[0])
				internPar[1] = false;
		} else {
			internPar[0] = Utils::addInBounds(internPar[0], -delta);
			if (0x00 == internPar[0])
				internPar[1] = true;
		}
		frame->setColor(color);
	}
}

void Animation::rotateLine() {
	/* ani 0x05
	 * paramSize: 5
	 * [ 0]: frameSkip
	 * [ 1]: antiAliasing//TODO implement
	 * [ 2]: width
	 * [ 3]: rotate_x//TODO implement
	 * [ 4]: rotate_y//TODO implement
	 * internPar: 2
	 * [0]: p_x1//0x00
	 * [1]: p_y1//0x00
	 */
	if (isFrameNotSkipped(parameter[0])) {
		color_t color(plate->getLastColor());
		color_t background(plate->get2LastColor());
		int p_x1 = internPar[0];
		int p_y1 = internPar[1];
		int lineWidth = parameter[2];

		if (p_x1 == width - 1)
			p_y1++;

		if (p_x1 < width - 1)
			p_x1++;

		if (p_y1 == height - 1) {
			p_x1 = 0;
			p_y1 = 0;
		}

		int p_x2 = Utils::invert(p_x1, width);
		int p_y2 = Utils::invert(p_y1, height);

		frame->setColor(background);
		frame->drawLine(p_x1, p_y1, p_x2, p_y2, color, lineWidth);

		internPar[0] = (uint8_t) p_x1;
		internPar[1] = (uint8_t) p_y1;
	}
}

void Animation::waterdrop() {
	/* ani 0x06
	 * paramSize: 3
	 * [0]: frameSkip
	 * [1]: antiAliasing//TODO implement
	 * [2]: width
	 * internPar: 3
	 * [0]: x//rand % width
	 * [1]: y//rand % height
	 * [2]: radius//0x00
	 */
	if (isFrameNotSkipped(parameter[0])) {
		color_t color(plate->getLastColor());
		color_t background(plate->get2LastColor());
		int lineWidth = parameter[2];
		uint8_t x = internPar[0];
		uint8_t y = internPar[1];
		uint8_t radius = internPar[2];

		color.setDimmer(
				(width - ((radius == 0) ? 1 : radius)) / (float) (width + 1));

		frame->setColor(background);
		frame->drawCircle(x, y, radius, color, lineWidth);

		if (radius < sqrt(width * width + height * height)) {
			//next time radius bigger
			internPar[2]++;
		} else {
			internPar[0] = rand() % (width - 1);
			internPar[1] = rand() % (height - 1);
			internPar[2] = 0;
		}
	}
}

void Animation::fadingPixels() {
	/* ani 0x07
	 * paramSize: 4
	 * [0]: frameSkip
	 * [1]: targetPixHigh
	 * [2]: targetPixLow
	 * [3]: fadeSpeed
	 * internPar: 2
	 * [0]: targetPixOldHigh//0x00
	 * [1]: targetPixOldLow//0x00
	 */
	if (isFrameNotSkipped(parameter[0])) {
		color_t color(plate->getLastColor());
		if (color == color_black)
			color = color_green;
		int targetPix = (parameter[1] << 8) | parameter[2];
		int targetPixOld = (internPar[0] << 8) | internPar[1];
		//set fadeSpeed at least 1
		int fadeSpeed = parameter[3];
		if (fadeSpeed == 0x00) {
			fadeSpeed = 0x01;
		}
		float fadeFactor = (255 - fadeSpeed) / 255.0;
		int maximumPixel = width * height;
		if (targetPix > maximumPixel) {
			//trim targetPixel to maximumPixel
			targetPix = maximumPixel;
		}

		if (targetPix != targetPixOld) {
			//initialize vector
			vector<int> emptyPix;
			for (int n = 0; n < maximumPixel; n++) {
				emptyPix.push_back(n);
			}
			//initialize screen with targetPix Pixels
			frame->setColor(color_black);
			for (int i = 0; i < targetPix; i++) {
				float v = (float) ((rand() & color.max()) / 255.0);
				color_t newPixelColor(color * v);
				int num = rand() % emptyPix.size();
				frame->setPixel(emptyPix[num], newPixelColor, 1);
				emptyPix.erase(emptyPix.begin() + num);
			}
			internPar[0] = (targetPix >> 8) & 0xff;
			internPar[1] = targetPix & 0xff;
		}
		//fade existing pixel
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < width; y++) {
				color_t tmp(frame->getPixel(x, y));
				if (tmp != color_black)
					frame->setPixel(x, y, tmp * fadeFactor, 1);
			}
		}
		//get actual amount of pixels
		vector<int> emptyPix;
		int actualPix = 0;
		for (int n = 0; n < maximumPixel; n++) {
			if (frame->getPixel(n) == color_black)
			//fill vector of empty pixels
					{
				emptyPix.push_back(n);
			} else {
				actualPix++;
			}
		}
		//create targetPixel-actualPix Pixels
		int pix = targetPix - actualPix;
		for (int i = 0; i < pix; i++) {
			int n = rand() % emptyPix.size();
			frame->setPixel(emptyPix[n], color, 1);
			//cout << i << " num: " << n << " value: " << emptyPix[n] << " size: " << emptyPix.size() << endl;
			emptyPix.erase(emptyPix.begin() + n);
		}
	}
}

void Animation::dirFallingPixel() {
	/* ani 0x08
	 * paramSize: 5
	 * [0]: frameSkip
	 * [1]: targetPix
	 * [2]: length
	 * [3]: direction//todo: implement
	 * [4]: dropDiff//random parameter in falling
	 * internPar: 2
	 * [0]: targetPixOld//0x00
	 * [1]: directionOld//0x00
	 */
	if (isFrameNotSkipped(parameter[0])) {
		color_t color(plate->getLastColor());
		if (color == color_black)
			color = color_green;

		int targetPix = parameter[1];
		int targetPixOld = internPar[0];
		int length = parameter[2];
		direction_t dir = static_cast<direction_t>(parameter[3]);
		direction_t dirOld = static_cast<direction_t>(internPar[1]);
		int dropDiff = parameter[4];
		//dir==top
		int w = width;
		int h = height;
		color_t dimColor(color / (float) length);
		if (dimColor.isAChannelVal(0)) {
			dimColor.setSaturation(1);
		}
		if (targetPix > w) {
			//trim targetPixel to w
			targetPix = w;
		}
		//initialize
		if ((dirOld != dir) && (targetPixOld != targetPix)) {
			//initialize vector
			vector<int> emptyRow;
			for (int n = 0; n < w; n++) {
				emptyRow.push_back(n);
			}
			//initialize screen with targetPix Pixels
			frame->setColor(color_black);

			//set falling pixels
			for (int i = 0; i < targetPix; i++) {
				int s = rand() % h;
				int num = rand() % emptyRow.size();
				for (int l = 0; l < length; l++) {
					//dir==top
					if (Utils::isInRange(s - l, 0, h))
						frame->setPixel(emptyRow[num], s - l,
								color - (dimColor * l), 1);
				}
				emptyRow.erase(emptyRow.begin() + num);
			}
			internPar[0] = targetPix;
			internPar[1] = dir;
		}
		//let em fall
		for (int n = 0; n < w; n++) {
			//put a bit random... not forward every line...
			int r = rand() % 0xff;
			if (r >= dropDiff) {
				//copy pixels from higher lanes
				for (int s = h - 1; s > 0; s--) {
					frame->setPixel(n, s, frame->getPixel(n, s - 1), 1);
				}
				//add last line
				color_t lineColor = frame->getPixel(n, 0);
				if (lineColor != color_black) {
					frame->setPixel(n, 0, lineColor - dimColor, 1);
				}
			}
		}
		//count used rows
		int actualPix = 0;
		vector<int> emptyRow;
		for (int n = 0; n < w; n++) {
			bool empty = true;
			for (int m = 0; m < h; m++) {
				if (frame->getPixel(n, m) == color) {
					if (m == 0) {
						empty = false;
					}
					actualPix++;
					break;
				}
			}
			if (empty) {
				emptyRow.push_back(n);
			}
		}
		//set new falling pixels..
		int newPix = targetPix - actualPix;
		for (int i = 0; i < newPix; i++) {
			//draw new falling pixel
			int num = rand() % emptyRow.size();
			//dir==top
			frame->setPixel(emptyRow[num], 0, color, 1);
			emptyRow.erase(emptyRow.begin() + num);
		}
	}
}


void Animation::plasma() {
	/* ani 0x0a
	 * paramSize: 3
	 * [0]: frameSkip
	 * [1]: colorDimmer
	 * [2]: saturation
	 * internPar: 1
	 * [0]: time
	 */
	if (isFrameNotSkipped(parameter[0])) {		
		float maxBrightness = parameter[1] / 255.0;
		uint8_t saturation = parameter[2];
		float time = internPar[0]/ 255.0 + internPar[1]/(255.0*255.0);
		color_t color;
		float xf,yf;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				xf = 1.0* x / width;
				yf = 1.0* y / height;
				float n = 0;
				n += Utils::sinn(xf+5*time);
				n += Utils::sinn(Utils::distance(xf,yf,Utils::sinf(3*time),-Utils::cosf(7*time)));
				
				n /=2.0;
				color = plate->getColor((n+time)/2);
				color.setDimmer(maxBrightness);
				color.setMinSaturation(saturation);
				frame->setPixel(x, y, color, 1);
			}
		}
		internPar[1]++;//time
		if(internPar[1]==0)
			internPar[0]++;
	}
}