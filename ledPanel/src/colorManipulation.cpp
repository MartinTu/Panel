/*
 * colorManipulation.cpp
 *
 *  Created on: 31.08.2014
 *      Author: keKs
 */

#include "colorManipulation.h"
using namespace std;

color_t ColorMan::rgbFade(color_t previous, uint8_t delta)
{
    if (previous.blue < delta)
    {
        if (previous.green <= (255 - delta))
            previous.green += delta;

        if (previous.red >= delta)
            previous.red -= delta;
        else
            previous.blue += delta;
    }

    if (previous.red < delta)
    {
        if (previous.blue <= (255 - delta))
            previous.blue += delta;

        if (previous.green >= delta)
            previous.green -= delta;
        else
            previous.red += delta;
    }

    if (previous.green < delta)
    {
        if (previous.red <= (255 - delta))
            previous.red += delta;

        if (previous.blue >= delta)
            previous.blue -= delta;
        else
            previous.green += delta;
    }

    return previous;
}

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

void ColorMan::printColor(string s, color_t color)
{
    cout << s+" r: " << hex << setw(3) << (int) color.red << " g: " << hex << setw(3) << (int) color.green
            << " b: " << hex << setw(3) << (int) color.blue << endl;
}
