/*
 * Animation.h
 *
 *  Created on: 06.09.2014
 *      Author: keKs
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#define COMMAND_DO_ANIMATION    0xa0
#define ANIMATION_MOVE_VLINE    0x01
#define ANIMATION_MOVE_HLINE    0x02
#define ANIMATION_ROTATE_LINE   0x03

//	brightness = 0x01;
//	pulse_getBrighter = false;
//
//	fade_color.red = 0xff;
//	fade_color.green = 0;
//	fade_color.blue = 0;
//	fade_status = 0x0;
//
//	animation_x1 = 0;
//	animation_x2 = 0;
//	animation_y1 = 0;
//	animation_y2 = 0;
//	animation_delay = 2;

#include <string>
#include <vector>
#include "Display.h"
#include "Color.h"
#include "ColorPlate.h"
#include "Utils.h"
#include "Bitmaps.h"
#include "Layer.h"
#include "LUT.h"

using namespace std;

enum animation_t {
	aniNone = 0x00,
	aniInvader = 0x01,
	aniDirectionFade = 0x02,
	aniScreenFade = 0x03,
	aniScreenPulse = 0x04,
	aniRotateLine = 0x05,
	aniWaterdrop = 0x06,
	aniFadingPixels = 0x07,
	aniDirFallingPixel = 0x08,
	aniPlasma = 0x0a,
	_aniNUM  //numbers of ani (if enum values are not doubled or spaced)
};

enum aniParamlen_t {
	lenNone = 0x00,
	lenInvader = 0x03,
	lenDirectionFade = 0x05,
	lenScreenFade = 0x04,
	lenScreenPulse = 0x05,
	lenRotateLine = 0x0b,
	lenWaterdrop = 0x09,
	lenFadingPixels = 0x07,
	lenDirFallingPixel = 0x08,
	lenPlasma = 0x03
};

class Animation: public Layer {
public:
	Animation(int _width, int _height);
	virtual ~Animation();

	int set(uint8_t id, string &param, unsigned int paramSize);
	void reset();
	
	bool nextStep();
	bool isAnimationRunning();

private:
	void invader();
	void directionFade();
	void screenFade();
	void screenPulse();
	void rotateLine();
	void waterdrop();
	void fadingPixels();
	void dirFallingPixel();
	void plasma();
	
protected:
	bool isFrameNotSkipped(unsigned int skip);

	unsigned int aniSkipIterator;
	animation_t runningAni;
	
	/* parameter
	 * parameters are set from client
	 * during animation these should not be altered
	 */
	string parameter;
	/* internPar
	 * intern parameters are initialized in set(.) and can change during animation
	 * stores the intern state of ananimation
	 */
	string internPar;
};

#endif /* ANIMATION_H_ */
