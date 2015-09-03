/*
 * Layer.cpp
 *
 *  Created on: 04.12.2014
 *      Author: keKs
 */

#include "Layer.h"

Layer::Layer(int _width, int _height) :
        width(_width), height(_height), mixer(mixMaybe)
{
	plate = new Plate;
    frame = new Canvas(width, height);
	
	plate->clear();
}

Layer::~Layer()
{
	
	if(plate)
		delete plate;
	else
		cout << "Layer.cpp: plate didnt exist" << endl;
    if (frame)
        delete frame;
    else
        cout << "Layer.cpp: frame didnt exist" << endl;
}

void Layer::setMixer(uint8_t _mixer)
{
    if (_mixer < _mixNUM)
    {
        this->mixer = static_cast<mixer_t>(_mixer);
    } else
    {
        cerr << _mixer << " is no mixer_t" << endl;
    }
}

mixer_t Layer::getMixer()
{
    return mixer;
}

Canvas* Layer::getCanvas()
{
    return frame;
}


void Layer::setGamma(bool _gamma)
{
	this->gamma = _gamma;
}


Plate *Layer::getPlate()
{
	return plate;
}