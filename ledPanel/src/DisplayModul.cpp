/*
 * DisplayModul.cpp
 *
 *  Created on: 29.08.2014
 *      Author: keKs
 */


#include "DisplayModul.h"


DisplayModul::DisplayModul(struct modul_t &modul)
		:	width(modul.width),
		 	height(modul.height),
		 	xOffset(modul.xOffset),
		 	yOffset(modul.yOffset),
		 	addressing(modul.addressing),
		 	orientation(modul.orientation),
		 	flip(modul.flip),
		 	chip(modul.chip)
{

}

DisplayModul::~DisplayModul()
{

}

int 	DisplayModul::getWidth()
{
	return this->width;
}
int 	DisplayModul::getHeight()
{
	return this->height;
}
int 	DisplayModul::getNumPix()
{
	return (this->width*this->height);
}
int 	DisplayModul::getXOffset()
{
	return this->xOffset;
}
int 	DisplayModul::getYOffset()
{
	return this->yOffset;
}
addressing_t 	DisplayModul::getAddressing()
{
	return this->addressing;
}
orientation_t 	DisplayModul::getOrientation()
{
	return this->orientation;
}
flip_t 	DisplayModul::getFlip()
{
	return this->flip;
}
chip_t 			DisplayModul::getChip()
{
	return this->chip;
}

void DisplayModul::reOrder(const int x, const int y, position_t &newPos)
{
	newPos.x = x - xOffset;
	newPos.y = y - yOffset;

	switch (addressing)
	{
	case undef:
		break;

	//vertical
	//bottom
	//left
	case snakeVBL:
		if(Utile::isOdd(newPos.y))
			newPos.x = Utile::invert(newPos.x, width);
		break;
	case xyVBL:
		break;
	//right
	case snakeVBR:
		if(Utile::isEven(newPos.y))
			newPos.x = Utile::invert(newPos.x, width);
		break;
	case xyVBR:
		newPos.x = Utile::invert(newPos.x, width);
		break;
	//top
	//left
	case snakeVTL:
		newPos.y = Utile::invert(newPos.y, height);
		//like snakeVBR
		if(Utile::isOdd(newPos.y))
			newPos.x = Utile::invert(newPos.x, width);
		break;
	case xyVTL:
		newPos.y = Utile::invert(newPos.y, height);
		break;
	//right
	case snakeVTR:
		newPos.y = Utile::invert(newPos.y, height);
		//like snakeVBR
		if(Utile::isEven(newPos.y))
			newPos.x = Utile::invert(newPos.x, width);
		break;
	case xyVTR:
		newPos.y = Utile::invert(newPos.y, height);
		newPos.x = Utile::invert(newPos.x, width);
		break;
	//horizontal
	case snakeHBL:
		rotate90(newPos,Grad90);
		//like snakeVBR
		if(Utile::isEven(newPos.y))
			newPos.x = Utile::invert(newPos.x, width);
		break;
	case xyHBL:
		rotate90(newPos,Grad90);
		//like xyVBR
		newPos.x = Utile::invert(newPos.x, width);
		break;
	case snakeHBR:
		rotate90(newPos,Grad270);
		//like snakeVBL
		if(Utile::isOdd(newPos.y))
			newPos.x = Utile::invert(newPos.x, width);
		break;
	case xyHBR:
		rotate90(newPos,Grad270);
		//like xyVBL
		break;
	case snakeHTL:
		rotate90(newPos,Grad90);
		//like snakeVBL
		if(Utile::isOdd(newPos.y))
			newPos.x = Utile::invert(newPos.x, width);
		break;
	case xyHTL:
		rotate90(newPos,Grad90);
		//like xyVBL
		break;
	case snakeHTR:
		rotate90(newPos,Grad270);
		//like snakeVBR
		if(Utile::isEven(newPos.y))
			newPos.x = Utile::invert(newPos.x, width);
		break;
	case xyHTR:
		rotate90(newPos,Grad270);
		//like xyVBR
		newPos.x = Utile::invert(newPos.x, width);
		break;
	default:
		cerr << "addressing has a wrong id: "<< addressing << endl;
		return;
	}

	switch (orientation)
	{
	case rotateNo:
		break;
	case rotateLeft:
		rotate90(newPos,Grad270);
		break;
	case rotateHalf:
		rotate90(newPos,Grad180);
		break;
	case rotateRight:
		rotate90(newPos,Grad90);
		break;
	default:
		cerr << "orientation has a wrong id: "<< orientation << endl;
		return;
	}

	switch(flip)
	{
	case flipNo:
		break;
	case flipX:
		newPos.x = Utile::invert(newPos.x, width);
		break;
	case flipY:
		newPos.y = Utile::invert(newPos.y, height);
		break;
	default:
		cerr << "flip has a wrong id: "<< flip << endl;
		return;
	}

	newPos.x = newPos.x + xOffset;
	newPos.y = newPos.y + yOffset;

//	cout << "x "<< setw(2) << x << " y "<< setw(2) << y <<" newx "<< setw(2)<< newPos.x << " newy "<< setw(2)<< newPos.y << " xO "<< setw(2)<< xOffset << " yO "<< setw(2)<< yOffset;
}




void 		DisplayModul::rotate90(position_t &pos, angle_t angle)
{
	if(Grad0 == angle)
		return;

	int midX = width/2;
	int midY = height/2;

	pos.x = midX + ((pos.x - midX) * rot90cos(angle) - (pos.y - midY) * rot90sin(angle));
	pos.y = midY + ((pos.y - midY) * rot90cos(angle) + (pos.x - midX) * rot90sin(angle));
}

int			DisplayModul::rot90sin(angle_t angle)
{
	switch(angle)
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

int			DisplayModul::rot90cos(angle_t angle)
{
	switch(angle)
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


