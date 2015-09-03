/*
 * Color.cpp
 *
 *  Created on: 31.08.2014
 *      Author: keKs
 */

#include "Color.h"
using namespace std;

color_t::color_t(const uint8_t _red, const uint8_t _green, const uint8_t _blue) :
		red(_red), green(_green), blue(_blue) {
}

color_t::color_t(const color_t& c) :
		red(c.red), green(c.green), blue(c.blue) {
}

color_t& color_t::operator=(color_t a) {
	swap(*this, a);
	return *this;
}

color_t& color_t::operator/=(const float& rhs) {
	// actual multiplication of rhs to *this
	red /= rhs;
	green /= rhs;
	blue /= rhs;
	return *this;
}

color_t& color_t::operator*=(const float& rhs) {
	// actual multiplication of rhs to *this
	red *= rhs;
	green *= rhs;
	blue *= rhs;
	return *this;
}

color_t& color_t::operator-=(const color_t& rhs) {
	// actual substraction of rhs to *this
	red = Utils::addInBounds((uint8_t) red, -(int) rhs.red, 0, 255);
	green = Utils::addInBounds((uint8_t) green, -(int) rhs.green, 0, 255);
	blue = Utils::addInBounds((uint8_t) blue, -(int) rhs.blue, 0, 255);
	return *this;
}

color_t& color_t::operator+=(const color_t& rhs) {
	// actual addition of rhs to *this
	red = Utils::addInBounds((uint8_t) red, rhs.red, 0, 255);
	green = Utils::addInBounds((uint8_t) green, rhs.green, 0, 255);
	blue = Utils::addInBounds((uint8_t) blue, rhs.blue, 0, 255);
	return *this;
}

void color_t::toHSV(uint8_t* hue, uint8_t* sat, uint8_t* val) const {
	uint8_t rgbMin, rgbMax,h,s,v;

	rgbMin = min();
	rgbMax = max();

	v = rgbMax;
	if (v == 0)
	{
		h = 0;
		s = 0;
	}
	else{
		s = 255 * long(rgbMax - rgbMin) / v;
		if (s == 0)
		{
			h = 0;
		}
		else{
			if (rgbMax == red)
				h = 0 + 43 * (green - blue) / (rgbMax - rgbMin);
			else if (rgbMax == green)
				h = 85 + 43 * (blue - red) / (rgbMax - rgbMin);
			else
				h = 171 + 43 * (red - green) / (rgbMax - rgbMin);
		}

	}
	*hue = h;
	*sat = s;
	*val = v;
}

void color_t::fromHSV(const uint8_t hue, const uint8_t sat, const uint8_t val) {
	uint8_t region, remainder, p, q, t;

	if (sat == 0) {
		red = val;
		green = val;
		blue = val;
		return;
	}

	region = hue / 43;
	remainder = (hue - (region * 43)) * 6;

	p = (val * (255 - sat)) >> 8;
	q = (val * (255 - ((sat * remainder) >> 8))) >> 8;
	t = (val * (255 - ((sat * (255 - remainder)) >> 8))) >> 8;

	switch (region) {
	case 0:
		red = val;
		green = t;
		blue = p;
		break;
	case 1:
		red = q;
		green = val;
		blue = p;
		break;
	case 2:
		red = p;
		green = val;
		blue = t;
		break;
	case 3:
		red = p;
		green = q;
		blue = val;
		break;
	case 4:
		red = t;
		green = p;
		blue = val;
		break;
	default:
		red = val;
		green = p;
		blue = q;
		break;
	}
}

void color_t::xFade(const float delta, const color_t c1, const color_t c2)
{
	if((delta < 0)||(delta > 1)){
		red = 0;
		green = 0;
		blue = 0;
		return;
	}
	float m;
	m     = (float)  (c2.red - c1.red);
	red   = (uint8_t)(m * delta + c1.red);
	m     = (float)  (c2.green - c1.green);
	green = (uint8_t)(m * delta + c1.green);
	m     = (float)  (c2.blue - c1.blue);
	blue  = (uint8_t)(m * delta + c1.blue);
}

void color_t::rgbFade(const uint8_t delta) {
	if (blue < delta) {
		if (green <= (255 - delta))
			green += delta;

		if (red >= delta)
			red -= delta;
		else
			blue += delta;
	}

	if (red < delta) {
		if (blue <= (255 - delta))
			blue += delta;

		if (green >= delta)
			green -= delta;
		else
			red += delta;
	}

	if (green < delta) {
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
void color_t::wheel(const uint8_t wheelPos) {
	uint8_t _wp = wheelPos;
	if (wheelPos < 85) {
		red = _wp * 3;
		green = 255 - _wp * 3;
		blue = 0;
		return;
	} else if (_wp < 170) {
		_wp -= 85;
		red = 255 - _wp * 3;
		green = 0;
		blue = _wp * 3;
		return;
	} else {
		_wp -= 170;
		red = 0;
		green = _wp * 3;
		blue = 255 - _wp * 3;
		return;
	}
}

void color_t::print(const string s) {
	cout << s << " r: " << hex << setw(3) << (int) red << " g: " << hex
			<< setw(3) << (int) green << " b: " << hex << setw(3) << (int) blue
			<< endl;
}

uint8_t color_t::max() const {
	return red > green ? (red > blue ? red : blue) : (green > blue ? green : blue);
}

uint8_t color_t::min() const {
	return red < green ? (red < blue ? red : blue) : (green < blue ? green : blue);
}

void color_t::setSaturation(const uint8_t saturation) {
	uint8_t hue = 0, sat = 0, val = 0;
	toHSV(&hue, &sat, &val);
	fromHSV(hue, saturation, val);
}

void color_t::setMinSaturation(const uint8_t saturation) {
	uint8_t hue = 0, sat = 0, val = 0;
	toHSV(&hue, &sat, &val);
	if(sat > saturation)
		fromHSV(hue,saturation, val);
}

uint8_t color_t::getSaturation() {
	return (uint8_t) (this->max() - this->min()) * 255.0 / this->max();
}

void color_t::setDimmer(const float& a) {
	*this *= a;
}

float color_t::getDimmer() {
	return (float) (this->max() / 255.0);
}

bool color_t::isAChannelVal(uint8_t val) {
	if ((red == val) || (green == val) || (blue == val))
		return true;
	return false;
}
