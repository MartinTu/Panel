/*
 * DisplayModul.cpp
 *
 *  Created on: 29.08.2014
 *      Author: keKs
 */

#include "DisplayModul.h"

DisplayModul::DisplayModul(int modulWidth, int modulHeight, int modulXOffset, int modulYOffset, addressing_t modulAddressing, orientation_t modulOrientation, flip_t modulFlip, chip_t modulChip, correction_t modulCorrection) :
        width(modulWidth), height(modulHeight), xOffset(modulXOffset), yOffset(modulYOffset), addressing(
                modulAddressing), orientation(modulOrientation), flip(modulFlip), chip(modulChip), correction(
                modulCorrection)
{

}

DisplayModul::~DisplayModul()
{

}

unsigned int DisplayModul::getWidth()
{
    return this->width;
}

unsigned int DisplayModul::getHeight()
{
    return this->height;
}

unsigned int DisplayModul::getNumPix()
{
    return (this->width * this->height);
}

unsigned int DisplayModul::getXOffset()
{
    return this->xOffset;
}

unsigned int DisplayModul::getYOffset()
{
    return this->yOffset;
}

addressing_t DisplayModul::getAddressing()
{
    return this->addressing;
}

orientation_t DisplayModul::getOrientation()
{
    return this->orientation;
}

flip_t DisplayModul::getFlip()
{
    return this->flip;
}

chip_t DisplayModul::getChip()
{
    return this->chip;
}

correction_t DisplayModul::getCorrection()
{
    return this->correction;
}

void DisplayModul::reOrder(const int x, const int y, int &newPosX, int &newPosY)
{
    newPosX = x - xOffset;
    newPosY = y - yOffset;

    switch (addressing)
    {
    //horizontal
    case xyHTL:
        break;
    case snakeHTL:
        if (Utils::isEven(newPosY))
            newPosX = Utils::invert(newPosX, width);
        break;
    case xyHTR:
        newPosX = Utils::invert(newPosX, width);
        break;
    case snakeHTR:
        if (Utils::isOdd(newPosY))
            newPosX = Utils::invert(newPosX, width);
        break;
    case xyHBL:
        newPosY = Utils::invert(newPosY, height);
        break;
    case snakeHBL:
        newPosY = Utils::invert(newPosY, height);
        //like snakeHTL
        if (Utils::isEven(newPosY))
            newPosX = Utils::invert(newPosX, width);
        break;
    case xyHBR:
        newPosY = Utils::invert(newPosY, height);
        newPosX = Utils::invert(newPosX, width);
        break;
    case snakeHBR:
        newPosY = Utils::invert(newPosY, height);
        //like snakeHTR
        if (Utils::isOdd(newPosY))
            newPosX = Utils::invert(newPosX, width);
        break;
        //vertical
    case xyVTL:
        rotate90(newPosX,newPosY, angleGrad270);
        //like xyHTR
        newPosX = Utils::invert(newPosX, width);
        break;
    case snakeVTL:
        rotate90(newPosX,newPosY, angleGrad270);
        //like snakeHTR
        if (Utils::isOdd(newPosY))
            newPosX = Utils::invert(newPosX, width);
        break;
    case xyVTR:
        rotate90(newPosX,newPosY, angleGrad90);
        //like xyHTL
        break;
    case snakeVTR:
        rotate90(newPosX,newPosY, angleGrad90);
        //like snakeHTL
        if (Utils::isEven(newPosY))
            newPosX = Utils::invert(newPosX, width);
        break;
    case xyVBL:
        rotate90(newPosX,newPosY, angleGrad270);
        //like xyHTL
        break;
    case snakeVBL:
        rotate90(newPosX,newPosY, angleGrad270);
        //like snakeHTL
        if (Utils::isEven(newPosY))
            newPosX = Utils::invert(newPosX, width);
        break;
    case xyVBR:
        rotate90(newPosX,newPosY, angleGrad90);
        //like xyHTR
        newPosX = Utils::invert(newPosX, width);
        break;
    case snakeVBR:
        rotate90(newPosX,newPosY, angleGrad90);
        //like snakeHTR
        if (Utils::isOdd(newPosY))
            newPosX = Utils::invert(newPosX, width);
        break;
    default:
        cerr << "addressing has a wrong id: " << addressing << endl;
        return;
    }

    switch (orientation)
    {
    case rotateNo:
        break;
    case rotateLeft:
        rotate90(newPosX,newPosY, angleGrad270);
        break;
    case rotateHalf:
        rotate90(newPosX,newPosY, angleGrad180);
        break;
    case rotateRight:
        rotate90(newPosX,newPosY, angleGrad90);
        break;
    default:
        cerr << "orientation has a wrong id: " << orientation << endl;
        return;
    }

    switch (flip)
    {
    case flipNo:
        break;
    case flipX:
        newPosX = Utils::invert(newPosX, width);
        break;
    case flipY:
        newPosY = Utils::invert(newPosY, height);
        break;
    default:
        cerr << "flip has a wrong id: " << flip << endl;
        return;
    }

    newPosX = newPosX + xOffset;
    newPosY = newPosY + yOffset;

//	cout << "x "<< setw(2) << x << " y "<< setw(2) << y <<" newx "<< setw(2)<< newPosX << " newy "<< setw(2)<< newPosY << " xO "<< setw(2)<< xOffset << " yO "<< setw(2)<< yOffset;
}

void DisplayModul::rotate90( int &newPosX, int &newPosY, unsigned char angle)
{
    if (angleGrad0 == angle)
        return;

    int midX = width / 2;
    int midY = height / 2;

    newPosX = midX + ((newPosX - midX) * rot90cos(angle) - (newPosY - midY) * rot90sin(angle));
    newPosY = midY + ((newPosY - midY) * rot90cos(angle) + (newPosX - midX) * rot90sin(angle));
}

int DisplayModul::rot90sin(unsigned char angle)
{
    switch (angle)
    {
    case angleGrad90:
        return 1;
    case angleGrad270:
        return -1;
    default:
        break;
    }
    return 0;
}

int DisplayModul::rot90cos(unsigned char angle)
{
    switch (angle)
    {
    case angleGrad0:
        return 1;
    case angleGrad180:
        return -1;
    default:
        break;
    }
    return 0;
}

