/*
 * colorManipulation.cpp
 *
 *  Created on: 31.08.2014
 *      Author: keKs
 */

#include "colorManipulation.h"

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
color_t ColorMan::wheel(uint8_t wheelPos)
{
    color_t result;
    if (wheelPos < 85)
    {
        result.red = wheelPos * 3;
        result.green = 255 - wheelPos * 3;
        result.blue = 0;
        return result;
    } else if (wheelPos < 170)
    {
        wheelPos -= 85;
        result.red = 255 - wheelPos * 3;
        result.green = 0;
        result.blue = wheelPos * 3;
        return result;
    } else
    {
        wheelPos -= 170;
        result.red = 0;
        result.green = wheelPos * 3;
        result.blue = 255 - wheelPos * 3;
        return result;
    }
}

