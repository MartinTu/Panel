/*
 * ColorPlate.h
 *
 *  Created on: 18.08.2015
 *      Author: keKs
 */

#ifndef COLOR_PLATE_H_
#define COLOR_PLATE_H_

#include <vector>
#include <math.h>
#include "Color.h"

using namespace std;

class Plate {
public:
	Plate();
	
	void clear(void);
	void update(color_t c);
	void append(color_t c);
	color_t getColor(float p);
	color_t getPrimeColor(unsigned int i);
	color_t getLastColor(void);
	color_t get2LastColor(void);
	unsigned int getSize(void);
	
private:
	vector<color_t> plate;
};

static const color_t color_default1 		= color_black;
static const color_t color_default2 		= color_dark_blue;

#endif /* COLOR_PLATE_H_ */