/*
 * Color.h
 *
 *  Created on: 31.08.2014
 *      Author: keKs
 */

#ifndef COLOR_H_
#define COLOR_H_

#include <stdint.h>
#include <iostream>
#include <string>
#include <iomanip>
#include "Utils.h"

using namespace std;

class color_t
{
    public:
        uint8_t red;
        uint8_t green;
        uint8_t blue;

        color_t(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0);
        color_t(const color_t& c);

        color_t& operator=(const color_t& a);
        color_t& operator*=(const float& a);
        color_t operator*(const float& a);
        color_t operator/(const float& a);
        color_t operator-(const color_t& a);
        color_t operator+(const color_t& a);
        bool operator==(color_t a) const;
        bool operator!=(color_t a) const;


        void wheel(const uint8_t wheelPos);
        void rgbFade(const uint8_t delta);
        void print(const string s);
        uint8_t max();
        uint8_t min();
        void setSaturation(const uint8_t saturation);
        uint8_t getSaturation();
        void setDimmer(const float& a);
        float getDimmer();
        bool isAChannelVal(uint8_t val);
};

static const color_t color_black = {0, 0, 0};
static const color_t color_red = {0xff, 0, 0};
static const color_t color_green = {0, 0xff, 0};
static const color_t color_blue = {0, 0, 0xff};
static const color_t color_dark_grey = {20, 20, 20};
static const color_t color_dark_blue = {0, 0, 20};

#endif /* COLOR_H_ */
