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
#include "Display.h"
#include "colorManipulation.h"
#include "Utils.h"
#include "bitmaps.h"

using namespace std;

enum animation_t
{
    aniNone = 0x00,
    aniInvader = 0x01,
    aniDirectionFade = 0x02,
    aniScreenFade = 0x03,
    aniScreenPulse = 0x04,
    aniRotateLine = 0x05,
    aniWaterdrop = 0x06,
    aniFadingPixels = 0x07,
    _aniNUM  //numbers of ani (if enum values are not doubled or spaced)
};

enum aniParamlen_t
{
    lenNone = 0x00,
    lenInvader = 0x01,
    lenDirectionFade = 0x04,
    lenScreenFade = 0x05,
    lenScreenPulse = 0x05,
    lenRotateLine = 0x07,
    lenWaterdrop = 0x06,
    lenFadingPixels = 0x06
};

enum mixer_t
{
    mixOff = 0x0,	//animation wont update
    mixMaybe = 0x1,  //another canvas may supress the animation from updating, animation is still continuing
    mixAdd = 0x2,	//canvas is drawn with add
    mixMult = 0x3,	//canvas is drawn with mult
    mixOnly = 0x4,	//only this canvas is drawn
    _mixNUM
};

enum direction_t
{
    dirLeft = 0x00,
    dirLeftBot = 0x01,
    dirBottom = 0x02,
    dirRightBot = 0x03,
    dirRight = 0x04,
    dirRightTop = 0x05,
    dirTop = 0x06,
    dirLeftTop = 0x07
};

class Animation
{
    public:
        Animation(int _width, int _height);
        virtual ~Animation();

        int set(uint8_t ani, string &param, unsigned int paramSize);
        void reset();
        bool nextStep();

        void setMixer(uint8_t _mixer);
        mixer_t getMixer();
        Canvas* getCanvas();
        void setAniDelay(unsigned int delay);
        bool isAnimationRunning();

        void invader();
        void directionFade();
        void screenFade();
        void screenPulse();
        void rotateLine();
        void waterdrop();
        void fadingPixels();

    protected:
        int width;
        int height;
        unsigned int aniDelay;
        unsigned int aniDelayIterator;
        animation_t runningAni;
        animation_t lastAni;
        mixer_t mixer;
        //parameters are set from client
        string parameter;
        //intern parameters are initialized in set(.), if the animation does not alter
        string internPar;
        Canvas* frame;	//x/y Animation screen
};

#endif /* ANIMATION_H_ */
