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

class color_t {
public:
	uint8_t red;
	uint8_t green;
	uint8_t blue;

	color_t(const uint8_t _red = 0, const uint8_t _green = 0,
			const uint8_t _blue = 0);
	color_t(const color_t& c);
	friend void swap(color_t& first, color_t& second) {
		// enable ADL (not necessary in our case, but good practice)
		using std::swap;

		// by swapping the members of two classes,
		// the two classes are effectively swapped
		swap(first.red, second.red);
		swap(first.green, second.green);
		swap(first.blue, second.blue);
	}
	color_t& operator=(color_t a);

	color_t& operator*=(const float& rhs);
	color_t& operator/=(const float& rhs);
	color_t& operator-=(const color_t& rhs);
	color_t& operator+=(const color_t& rhs);

	uint8_t max() const;
	uint8_t min() const;

	void toHSV(uint8_t* hue, uint8_t* sat, uint8_t* val) const;
	void fromHSV(const uint8_t hue, const uint8_t sat, const uint8_t val);

	
	void wheel(const uint8_t wheelPos);
	
	void xFade(const float delta, const color_t c1, const color_t c2);
	void rgbFade(const uint8_t delta);
	
	void print(const string s);
	void setSaturation(const uint8_t saturation);
	void setMinSaturation(const uint8_t saturation);
	uint8_t getSaturation();
	void setDimmer(const float& a);
	float getDimmer();
	bool isAChannelVal(uint8_t val);
};

inline bool operator==(const color_t& lhs, const color_t& rhs) {
	if ((lhs.red == rhs.red) && (lhs.green == rhs.green)
			&& (lhs.blue == rhs.blue))
		return true;
	return false;
}
inline bool operator!=(const color_t& lhs, const color_t& rhs) {
	return !operator==(lhs, rhs);
}

inline color_t operator*(color_t lhs, const float& rhs) {
	lhs *= rhs;
	return lhs;
}
inline color_t operator/(color_t lhs, const float& rhs) {
	lhs /= rhs;
	return lhs;
}
inline color_t operator-(color_t lhs, const color_t& rhs) {
	lhs -= rhs;
	return lhs;
}
inline color_t operator+(color_t lhs, const color_t& rhs) {
	lhs += rhs;
	return lhs;
}

static const color_t color_black 		= {   0,   0,   0};
static const color_t color_white 		= {0xff,0xff,0xff};
static const color_t color_red 			= {0xff,   0,   0};
static const color_t color_green 		= {   0,0xff,   0};
static const color_t color_blue 		= {   0,   0,0xff};
static const color_t color_dark_grey 	= {  20,  20,  20};
static const color_t color_dark_blue 	= {   0,   0,  20};

#endif /* COLOR_H_ */
