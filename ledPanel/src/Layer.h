/*
 * Layer.h
 *
 *  Created on: 04.12.2014
 *      Author: keKs
 */

#ifndef LAYER_H_
#define LAYER_H_

#include <string>
#include "Utils.h"
#include "Canvas.h"
#include "Color.h"

enum mixer_t
{
    mixOff = 0x0,   //layer wont update
    mixMaybe = 0x1,  //another layer may supress the animation from updating, animation is still continuing
    mixAdd = 0x2,   //canvas is drawn with add
    mixMult = 0x3,  //canvas is drawn with mult
    mixOnly = 0x4,  //only this canvas is drawn
    _mixNUM
};

class Layer
{
    public:
        Layer(int _width, int _height);
        virtual ~Layer();

        virtual int set(uint8_t id, string &param, unsigned int paramSize) = 0;
        virtual void reset() = 0;

        void setMixer(uint8_t _mixer);
        mixer_t getMixer();
        Canvas* getCanvas();

    protected:
        int width;
        int height;
        mixer_t mixer;
        Canvas* frame;  //x/y drawing screen
};

#endif /* LAYER_H_ */
