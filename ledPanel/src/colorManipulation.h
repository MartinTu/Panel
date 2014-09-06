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
};

static const color_t color_black 		= {  0,  0,  0};
static const color_t color_dark_grey	= { 20, 20, 20};
static const color_t color_dark_blue 	= {  0,  0, 20};

class ColorMan
{
public:
	static color_t wheel(uint8_t wheelPos);
};



#endif /* COLORMANIPULATION_H_ */
