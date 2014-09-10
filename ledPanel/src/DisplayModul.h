/*
 * DisplayModul.h
 *
 *  Created on: 29.08.2014
 *      Author: keKs
 */

#ifndef DISPLAYMODUL_H_
#define DISPLAYMODUL_H_

#include <vector>
#include <iostream>
#include <iomanip>

#include "Utile.h"

#define LEFT0RIGHT1_MSK 0x1
#define BOTTOM0TOP1_MSK	0x2
#define ND0DEF1_MSK 0x10
#define XY0SNAKE1_MSK 0x20
#define VERT0HORI1_MSK 0x40

using namespace std;

enum correction_t
{
    corrNo = 0x00, corrGamma = 0x01, corrPixel = 0x02, corrAll = 0x03
};

enum addressing_t
{
    //not defined
    undef = 0x00,
    //x/y vertical
    xyVBL = 0x10,  //Start Bottom Left
    xyVBR = 0x11,  //Start Bottom Right
    xyVTL = 0x12,  //Start Top Left
    xyVTR = 0x13,  //Start Top Right
    //x/y horizontal
    xyHBL = 0x50,	//Start Bottom Left
    xyHBR = 0x51,	//Start Bottom Right
    xyHTL = 0x52,	//Start Top Left
    xyHTR = 0x53,	//Start Top Right
    //snake vertical
    snakeVBL = 0x30,	//Start Bottom Left
    snakeVBR = 0x31,	//Start Bottom Right
    snakeVTL = 0x32,	//Start Top Left
    snakeVTR = 0x33,	//Start Top Right
    //snake horizontal
    snakeHBL = 0x70,	//Start Bottom Left
    snakeHBR = 0x71,	//Start Bottom Right
    snakeHTL = 0x72,	//Start Top Left
    snakeHTR = 0x73  //Start Top Right
};

enum orientation_t
{
    rotateNo = 0x00,  //landscape
    rotateLeft = 0x01,  //portrait left
    rotateHalf = 0x02,  //landscape rotated half
    rotateRight = 0x03  //portrait right
};

enum flip_t
{
    flipNo = 0x00, flipX = 0x10,  //landscape flipped in x direction
    flipY = 0x20  //landscape flipped in y direction
};

enum chip_t
{
    LDP6803 = 0x00, WS2801 = 0x01
};

enum angle_t
{
    Grad0 = 0x00, Grad90 = 0x01, Grad180 = 0x02, Grad270 = 0x03
};

struct modul_t
{
        int width;
        int height;
        int xOffset;
        int yOffset;
        int numWireing;
        addressing_t addressing;
        orientation_t orientation;
        flip_t flip;
        chip_t chip;
        correction_t correction;
};

struct position_t
{
        int x;
        int y;
};

class DisplayModul
{
    public:
        DisplayModul(struct modul_t &modul);
        virtual ~DisplayModul();

        int getWidth();
        int getHeight();
        int getNumPix();
        int getXOffset();
        int getYOffset();
        addressing_t getAddressing();
        orientation_t getOrientation();
        flip_t getFlip();
        chip_t getChip();
        correction_t getCorrection();

        void reOrder(const int x, const int y, position_t &newPos);

    private:
        void rotate90(position_t &pos, angle_t angle);
        int rot90sin(angle_t angle);
        int rot90cos(angle_t angle);

        //module parameters
        int width;
        int height;
        int xOffset;
        int yOffset;
        addressing_t addressing;
        orientation_t orientation;
        flip_t flip;
        chip_t chip;
        correction_t correction;
};

#endif /* DISPLAYMODUL_H_ */
