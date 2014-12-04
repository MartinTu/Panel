/*
 * Canvas.h
 *
 *  Created on: 07.09.2014
 *      Author: keKs
 */

#ifndef CANVAS_H_
#define CANVAS_H_

#include <vector>
#include <cmath>
#include "Color.h"
#include "Utils.h"

using namespace std;

class Canvas
{
    public:
        Canvas(int _width, int _height);
        virtual ~Canvas();

        void setPixel(int x, int y, color_t color);
        void setPixel(int num, color_t color);
        void setColor(color_t color);
        void drawFrame(const vector<vector<color_t> >& vec);
        void drawFrame(int dataLength, const uint8_t * data);
        void drawSector(int dataLength, const uint8_t* data, int width, int height, int xOffset, int yOffset);
        void drawRandomPixel(int x, int y);
        void drawRandomPicture();

        void drawLine(int x_start, int y_start, int x_end, int y_end, color_t color, int width);
        void drawCircle(int x0, int y0, int radius, color_t color, uint8_t width);

        color_t getPixel(int x, int y);
        color_t getPixel(int num);
        vector<vector<color_t> >& getPicture();
        int getWidth();
        int getHeight();

    protected:
        int width;
        int height;
        vector<vector<color_t> > pixel;  //x/y
};

#endif /* CANVAS_H_ */
