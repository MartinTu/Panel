/*
 * Painting.cpp
 *
 *  Created on: 04.12.2014
 *      Author: keKs
 */

#include "Painting.h"

Painting::Painting(int _width, int _height) :
        Layer(_width, _height)
{

}

Painting::~Painting()
{
}

int Painting::set(uint8_t id, string &param, unsigned int paramSize)
{
    switch (id)
    {
    case SPCMD__COLOR:
        setColor(paramSize, param);
        break;

    case SPCMD__SET_PIXEL:
        setPixel(paramSize, param);
        break;

    case SPCMD__DRAW_LINE:
        drawLine(paramSize, param);
        break;

    case SPCMD__DRAW_RECT:
    	drawRect(paramSize, param);
        break;

    case SPCMD__DRAW_CIRC:
        drawCircle(paramSize, param);
        break;

    default:
        cerr << "SpSubCommand 0x" << hex << id << "not supported" << endl;
        break;
    }
    return 0;
}

void Painting::reset()
{
    frame->setColor(color_black);
}

void Painting::setColor(unsigned int paramLen, string &param)
{
    cout << "[INFO] color";
    cout << ": param(0x";
    for (unsigned int i = 0; i < param.length(); i++)
    {
        cout << hex << setw(3) << (int) param[i];
    }
    cout << ")" << endl;

    if (paramLen == lenParamSetColor)
    {
		color_t c((uint8_t) param[0],(uint8_t) param[1],(uint8_t) param[2]);
		plate->clear();
		plate->append(c);
        color_t color {plate->getLastColor()};
        frame->setColor(color);
    } else
    {
        cerr << "paramLen out of bounds(" << lenParamSetColor << ") " << paramLen << endl;
    }
}

void Painting::setPixel(unsigned int paramLen, string &param)
{
    cout << "[INFO] pixel";
    cout << ": param(0x";
    for (unsigned int i = 0; i < param.length(); i++)
    {
        cout << hex << setw(3) << (int) param[i];
    }
    cout << ")" << endl;

    if (paramLen == lenParamSetPixel)
    {
		color_t c((uint8_t) param[2],(uint8_t) param[3],(uint8_t) param[4]);
		plate->append(c);
        color_t color {plate->getLastColor()};
        frame->setPixel(param[0], param[1], color, 1);
    } else
    {
        cerr << "paramLen out of bounds(" << lenParamSetPixel << ") " << paramLen << endl;
    }
}

void Painting::drawLine(unsigned int paramLen, string& param)
{
    cout << "[INFO] line";
    cout << ": param(0x";
    for (unsigned int i = 0; i < param.length(); i++)
    {
        cout << hex << setw(3) << (int) (param[i]);
    }
    cout << ")" << endl;
    if (paramLen == lenParamDrawLine)
    {
		color_t c((uint8_t) param[4],(uint8_t) param[5],(uint8_t) param[6]);
		plate->append(c);
        color_t color {plate->getLastColor()};
        //FIXME line width
        frame->drawLine(param[0], param[1], param[2], param[3], color, 1);
    } else
    {
        cerr << "paramLen out of bounds(" << lenParamDrawLine << ") " << paramLen << endl;
    }
}

void Painting::drawCircle(unsigned int paramLen, string& param)
{
    cout << "[INFO] circle";
    cout << ": param(0x";
    for (unsigned int i = 0; i < param.length(); i++)
    {
        cout << hex << setw(3) << (int) (param[i]);
    }
    cout << ")" << endl;
    if (paramLen == lenParamDrawCircle)
    {
		color_t c((uint8_t) param[3],(uint8_t) param[4],(uint8_t) param[5]);
		plate->append(c);
        color_t color {plate->getLastColor()};
        //FIXME circle width
        frame->drawCircle(param[0], param[1], param[2], color, 1);
    } else
    {
        cerr << "paramLen out of bounds(" << lenParamDrawCircle << ") " << paramLen << endl;
    }
}

void Painting::drawRect(unsigned int paramLen, string& param)
{
    cout << "[INFO] rect";
    cout << ": param(0x";
    for (unsigned int i = 0; i < param.length(); i++)
    {
        cout << hex << setw(3) << (int) (param[i]);
    }
    cout << ")" << endl;
    if (paramLen == lenParamDrawRect)
    {
		color_t c((uint8_t) param[4],(uint8_t) param[5],(uint8_t) param[6]);
		plate->append(c);
    	color_t color {plate->getLastColor()};
    	//FIXME line width
    	frame->drawRect(param[0], param[1], param[2], param[3], color);
    } else
    {
        cerr << "paramLen out of bounds(" << lenParamDrawRect << ") " << paramLen << endl;
    }
}
