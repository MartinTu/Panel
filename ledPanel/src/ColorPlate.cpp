/*
 * ColorPlate.cpp
 *
 *  Created on: 18.08.2015
 *      Author: keKs
 */
 
#include "ColorPlate.h"
using namespace std;

Plate::Plate()
{
	plate.clear();
}

void Plate::clear()
{
	plate.clear();
}

void Plate::append(color_t c)
{
	plate.push_back(c);
}

void Plate::update(color_t c)
{
	if(!plate.size()){
		//append color to empty plate
		plate.push_back(c);
		return;
	}
	plate[plate.size()-1] = c;
}



color_t Plate::getColor(float p)
{
	color_t r = color_default1;
	int number = plate.size();
	if(number == 0){
		return r;
	}
	if(number == 1)
		return plate[0];
	if(p < 0)
		p = -p +trunc(p);//-5,3 --> 5,3 -5 = 0,3
	if(p >= 1)
		p -= trunc(p);
	if((p == 0) || (p == 1))
		return plate[0];
	int pivot = 1;
	float distance = (float)(1.0 / number);
	for(pivot = 1; pivot <= number; pivot++)
		if(pivot*distance >= p)
		{
			break;
		}
	color_t l, u;
	float rel = (p-(pivot-1)*distance)*number;
	l = plate[pivot-1];
	if(pivot == number)
		pivot = 0;
	u = plate[pivot];
	r.xFade(rel, l,u);
	return r;
}

color_t Plate::getPrimeColor(unsigned int i)
{
	if(i < plate.size())
		return plate[i];
	color_t c(color_default1);
	return c;
}

color_t Plate::getLastColor(void)
{
	if(plate.size())
		return plate.back();
	color_t c(color_default1);
	return c;
}

color_t Plate::get2LastColor(void)
{
	if(plate.size() > 1)
		return plate[plate.size()-2];
	color_t c(color_default2);
	return c;
}

unsigned int Plate::getSize(void)
{
	return plate.size();
}