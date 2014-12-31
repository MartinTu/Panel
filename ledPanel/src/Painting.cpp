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
        cerr << "SPCMD__DRAW_RECT not supported" << endl;
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
    cout << "Color";
    cout << ": param(0x";
    for (unsigned int i = 0; i < param.length(); i++)
    {
        cout << hex << setw(3) << (int) param[i];
    }
    cout << ")" << endl;

    if (paramLen == 3)
    {
        color_t color {(uint8_t) param[0], (uint8_t) param[1], (uint8_t) param[2]};
        frame->setColor(color);
    } else
    {
        cerr << "paramLen out of bounds(3) " << paramLen << endl;
    }
}

void Painting::setPixel(unsigned int paramLen, string &param)
{
    cout << "Pixel";
    cout << ": param(0x";
    for (unsigned int i = 0; i < param.length(); i++)
    {
        cout << hex << setw(3) << (int) param[i];
    }
    cout << ")" << endl;

    if (paramLen == 5)
    {
        color_t color {(uint8_t) param[2], (uint8_t) param[3], (uint8_t) param[4]};
        frame->setPixel(param[0], param[1], color);
    } else
    {
        cerr << "paramLen out of bounds(5) " << paramLen << endl;
    }
}

void Painting::drawLine(unsigned int paramLen, string& param)
{
    cout << "Line";
    cout << ": param(0x";
    for (unsigned int i = 0; i < param.length(); i++)
    {
        cout << hex << setw(3) << (int) (param[i]);
    }
    cout << ")" << endl;
    if (paramLen == 7)
    {
        color_t color {(uint8_t) param[4], (uint8_t) param[5], (uint8_t) param[6]};
        //FIXME line width
        frame->drawLine(param[0], param[1], param[2], param[3], color, 1);
    } else
    {
        cerr << "paramLen out of bounds(7) " << paramLen << endl;
    }
}

void Painting::drawCircle(unsigned int paramLen, string& param)
{
    cout << "Circle";
    cout << ": param(0x";
    for (unsigned int i = 0; i < param.length(); i++)
    {
        cout << hex << setw(3) << (int) (param[i]);
    }
    cout << ")" << endl;
    if (paramLen == 6)
    {
        color_t color {(uint8_t) param[3], (uint8_t) param[4], (uint8_t) param[5]};
        //FIXME circle width
        frame->drawCircle(param[0], param[1], param[2], color, 1);
    } else
    {
        cerr << "paramLen out of bounds(6) " << paramLen << endl;
    }
}
