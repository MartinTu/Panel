/*
 * DisplayModul.cpp
 *
 *  Created on: 29.08.2014
 *      Author: keKs
 */

#include "DisplayModul.h"

DisplayModul::DisplayModul(struct modul_t &modul) :
        width(modul.width), height(modul.height), xOffset(modul.xOffset), yOffset(modul.yOffset), addressing(
                modul.addressing), orientation(modul.orientation), flip(modul.flip), chip(modul.chip), correction(
                modul.correction)
{

}

DisplayModul::~DisplayModul()
{

}

int DisplayModul::getWidth()
{
    return this->width;
}
int DisplayModul::getHeight()
{
    return this->height;
}
int DisplayModul::getNumPix()
{
    return (this->width * this->height);
}
int DisplayModul::getXOffset()
{
    return this->xOffset;
}
int DisplayModul::getYOffset()
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

void DisplayModul::reOrder(const int x, const int y, position_t &newPos)
{
    newPos.x = x - xOffset;
    newPos.y = y - yOffset;

    switch (addressing)
    {
    //horizontal
    case xyHTL:
        break;
    case snakeHTL:
        if (Utils::isEven(newPos.y))
            newPos.x = Utils::invert(newPos.x, width);
        break;
    case xyHTR:
        newPos.x = Utils::invert(newPos.x, width);
        break;
    case snakeHTR:
        if (Utils::isOdd(newPos.y))
            newPos.x = Utils::invert(newPos.x, width);
        break;
    case xyHBL:
        newPos.y = Utils::invert(newPos.y, height);
        break;
    case snakeHBL:
        newPos.y = Utils::invert(newPos.y, height);
        //like snakeHTL
        if (Utils::isEven(newPos.y))
            newPos.x = Utils::invert(newPos.x, width);
        break;
    case xyHBR:
        newPos.y = Utils::invert(newPos.y, height);
        newPos.x = Utils::invert(newPos.x, width);
        break;
    case snakeHBR:
        newPos.y = Utils::invert(newPos.y, height);
        //like snakeHTR
        if (Utils::isOdd(newPos.y))
            newPos.x = Utils::invert(newPos.x, width);
        break;
        //vertical
    case xyVTL:
        rotate90(newPos, Grad270);
        //like xyHTR
        newPos.x = Utils::invert(newPos.x, width);
        break;
    case snakeVTL:
        rotate90(newPos, Grad270);
        //like snakeHTR
        if (Utils::isOdd(newPos.y))
            newPos.x = Utils::invert(newPos.x, width);
        break;
    case xyVTR:
        rotate90(newPos, Grad90);
        //like xyHTL
        break;
    case snakeVTR:
        rotate90(newPos, Grad90);
        //like snakeHTL
        if (Utils::isEven(newPos.y))
            newPos.x = Utils::invert(newPos.x, width);
        break;
    case xyVBL:
        rotate90(newPos, Grad270);
        //like xyHTL
        break;
    case snakeVBL:
        rotate90(newPos, Grad270);
        //like snakeHTL
        if (Utils::isEven(newPos.y))
            newPos.x = Utils::invert(newPos.x, width);
        break;
    case xyVBR:
        rotate90(newPos, Grad90);
        //like xyHTR
        newPos.x = Utils::invert(newPos.x, width);
        break;
    case snakeVBR:
        rotate90(newPos, Grad90);
        //like snakeHTR
        if (Utils::isOdd(newPos.y))
            newPos.x = Utils::invert(newPos.x, width);
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
        rotate90(newPos, Grad270);
        break;
    case rotateHalf:
        rotate90(newPos, Grad180);
        break;
    case rotateRight:
        rotate90(newPos, Grad90);
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
        newPos.x = Utils::invert(newPos.x, width);
        break;
    case flipY:
        newPos.y = Utils::invert(newPos.y, height);
        break;
    default:
        cerr << "flip has a wrong id: " << flip << endl;
        return;
    }

    newPos.x = newPos.x + xOffset;
    newPos.y = newPos.y + yOffset;

//	cout << "x "<< setw(2) << x << " y "<< setw(2) << y <<" newx "<< setw(2)<< newPos.x << " newy "<< setw(2)<< newPos.y << " xO "<< setw(2)<< xOffset << " yO "<< setw(2)<< yOffset;
}

void DisplayModul::rotate90(position_t &pos, angle_t angle)
{
    if (Grad0 == angle)
        return;

    int midX = width / 2;
    int midY = height / 2;

    pos.x = midX + ((pos.x - midX) * rot90cos(angle) - (pos.y - midY) * rot90sin(angle));
    pos.y = midY + ((pos.y - midY) * rot90cos(angle) + (pos.x - midX) * rot90sin(angle));
}

int DisplayModul::rot90sin(angle_t angle)
{
    switch (angle)
    {
    case Grad90:
        return 1;
    case Grad270:
        return -1;
    default:
        break;
    }
    return 0;
}

int DisplayModul::rot90cos(angle_t angle)
{
    switch (angle)
    {
    case Grad0:
        return 1;
    case Grad180:
        return -1;
    default:
        break;
    }
    return 0;
}

