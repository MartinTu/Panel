/*
 * colorManipulation.h
 *
 *  Created on: 31.08.2014
 *      Author: keKs
 */

#ifndef COLORMANIPULATION_H_
#define COLORMANIPULATION_H_

#include <stdint.h>

struct color_t
{
        uint8_t red;
        uint8_t green;
        uint8_t blue;

        color_t(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0) :
                red(red), green(green), blue(blue)
        {
        }

        color_t& operator=(const color_t& a)
        {
            red = a.red;
            green = a.green;
            blue = a.blue;
            return *this;
        }

        color_t& operator*(const float& a)
        {
            red = a * red;
            green = a * green;
            blue = a * blue;
            return *this;
        }

        bool operator==(color_t a) const
        {
            if ((a.red == red) && (a.green == green) && (a.blue == blue))
                return true;
            return false;
        }

        bool operator!=(color_t a) const
        {
            if ((a.red != red) || (a.green != green) || (a.blue != blue))
                return true;
            return false;
        }
};

static const color_t color_black = {0, 0, 0};
static const color_t color_dark_grey = {20, 20, 20};
static const color_t color_dark_blue = {0, 0, 20};

class ColorMan
{
    public:
        static color_t wheel(uint8_t wheelPos);
};

#endif /* COLORMANIPULATION_H_ */
