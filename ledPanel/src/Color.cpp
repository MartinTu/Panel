/*
 * Color.cpp
 *
 *  Created on: 31.08.2014
 *      Author: keKs
 */

#include "Color.h"
using namespace std;

color_t::color_t(uint8_t red, uint8_t green, uint8_t blue) :
        red(red), green(green), blue(blue)
{
}

color_t::color_t(const color_t& c) :
        red(c.red), green(c.green), blue(c.blue)
{
}

color_t& color_t::operator=(const color_t& a)
{
    //suppress selfcopy
    red = a.red;
    green = a.green;
    blue = a.blue;
    return *this;
}

color_t& color_t::operator*=(const float& a)
{
    red = a * red;
    green = a * green;
    blue = a * blue;
    //return reference
    return *this;
}

color_t color_t::operator*(const float& a)
{
    color_t c(a * red, a * green, a * blue);
    //return copy
    return c;
}

color_t color_t::operator/(const float& a)
{
    color_t c(red / a, green / a, blue / a);
    //return copy
    return c;
}
color_t color_t::operator-(const color_t& a)
{
    color_t c(Utils::addInBounds((uint8_t) red, -((int) a.red), 0, 255),
            Utils::addInBounds((uint8_t) green, -((int) a.green), 0, 255),
            Utils::addInBounds((uint8_t) blue, -((int) a.blue), 0, 255));
    //return copy
    return c;
}
color_t color_t::operator+(const color_t& a)
{
    color_t c(Utils::addInBounds((uint8_t) red, a.red, 0, 255),
            Utils::addInBounds((uint8_t) green, a.green, 0, 255),
            Utils::addInBounds((uint8_t) blue, a.blue, 0, 255));
    //return copy
    return c;
}

bool color_t::operator==(color_t a) const
{
    if ((a.red == red) && (a.green == green) && (a.blue == blue))
        return true;
    return false;
}

bool color_t::operator!=(color_t a) const
{
    if ((a.red != red) || (a.green != green) || (a.blue != blue))
        return true;
    return false;
}

void color_t::rgbFade(const uint8_t delta)
{
    if (blue < delta)
    {
        if (green <= (255 - delta))
            green += delta;

        if (red >= delta)
            red -= delta;
        else
            blue += delta;
    }

    if (red < delta)
    {
        if (blue <= (255 - delta))
            blue += delta;

        if (green >= delta)
            green -= delta;
        else
            red += delta;
    }

    if (green < delta)
    {
        if (red <= (255 - delta))
            red += delta;

        if (blue >= delta)
            blue -= delta;
        else
            green += delta;
    }
}

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
void color_t::wheel(const uint8_t wheelPos)
{
    uint8_t _wp = wheelPos;
    if (wheelPos < 85)
    {
        red = _wp * 3;
        green = 255 - _wp * 3;
        blue = 0;
        return;
    } else if (_wp < 170)
    {
        _wp -= 85;
        red = 255 - _wp * 3;
        green = 0;
        blue = _wp * 3;
        return;
    } else
    {
        _wp -= 170;
        red = 0;
        green = _wp * 3;
        blue = 255 - _wp * 3;
        return;
    }
}

void color_t::print(const string s)
{
    cout << s + " r: " << hex << setw(3) << (int) red << " g: " << hex << setw(3) << (int) green << " b: " << hex
            << setw(3) << (int) blue << endl;
}

uint8_t color_t::max()
{
    uint8_t i = Utils::max(red, green);
    i = Utils::max(i, blue);
    return i;
}

uint8_t color_t::min()
{
    uint8_t i = Utils::min(red, green);
    i = Utils::min(i, blue);
    return i;
}


//todo saturation muss multiplikativ berechnet werden, nicht absoult
void color_t::setSaturation(const uint8_t saturation)
{
    red = Utils::max(saturation, red);
    green = Utils::max(saturation, green);
    blue = Utils::max(saturation, blue);
}

uint8_t color_t::getSaturation()
{
    return this->min();
}

void color_t::setDimmer(const float& a)
{
    red = a * red;
    green = a * green;
    blue = a * blue;
}

float color_t::getDimmer()
{
    return (float) (this->min() / 255.0);
}

bool color_t::isAChannelVal(uint8_t val)
{
    if ((red == val) || (green == val) || (blue == val))
        return true;
    return false;
}
