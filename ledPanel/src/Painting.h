/*
 * Painting.h
 *
 *  Created on: 04.12.2014
 *      Author: keKs
 */

#ifndef PAINTING_H_
#define PAINTING_H_

#include <string>
#include <iostream>
#include <sstream>
#include "Utils.h"
#include "Canvas.h"
#include "Color.h"
#include "Layer.h"
#include "tpm2_spcmd.h"

class Painting : public Layer
{
    public:
        Painting(int _width, int _height);
        virtual ~Painting();

        int set(uint8_t id, string &param, unsigned int paramSize);
        void reset();

    private:
	static const unsigned char	lenParamSetColor 	=0x03;
	static const unsigned char	lenParamSetPixel 	=0x05;
	static const unsigned char	lenParamDrawLine 	=0x08;
	static const unsigned char	lenParamDrawCircle 	=0x06;
	static const unsigned char	lenParamDrawRect 	=0x07;
	
        void setColor(unsigned int paramLen, string& param);
        void setPixel(unsigned int paramLen, string& param);
        void drawLine(unsigned int paramLen, string& param);
        void drawCircle(unsigned int paramLen, string& param);
        void drawRect(unsigned int paramLen, string& param);
};

#endif /* PAINTING_H_ */
