/*
 * Canvas.cpp
 *
 *  Created on: 07.09.2014
 *      Author: keKs
 */

#include "Canvas.h"

using namespace std;

Canvas::Canvas(int _width, int _height) :
        width(_width), height(_width)
{
    pixel.resize(width);
    for (int i = 0; i < width; i++)
    {
        pixel[i].resize(height);
    }
    setColor(color_black);
    //seed
    srand((unsigned int) Utile::getTime());
}

Canvas::~Canvas()
{
}

void Canvas::setPixel(int x, int y, color_t color)
{
    if ((x < 0) || (x >= width))
    {
        cerr << "setPixel() x out of bounds" << endl;
        return;
    }
    if ((y < 0) || (x >= height))
    {
        cerr << "setPixel() y out of bounds" << endl;
        return;
    }
    pixel[x][y] = color;
}

void Canvas::setColor(color_t color)
{
    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++)
            pixel[x][y] = color;
}

void Canvas::drawFrame(const vector<vector<color_t> >& vec)
{
    if ((vec.size() == pixel.size()) && (vec[0].size() == pixel[0].size()))
    {
        //copy first
        vector<vector<color_t> > neu(vec);
        pixel.swap(neu);
    } else
    {
        cerr << "drawFrame() does not got correct vector size" << endl;
    }
}

void Canvas::drawFrame(int dataLength, const uint8_t * data)
{
    if (dataLength == width * height * 3)
    {
        int i = 0;
        //0-0 = topleft
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                pixel[x][y].red = data[i++];
                pixel[x][y].green = data[i++];
                pixel[x][y].blue = data[i++];
            }
        }
    } else
    {
        cerr << "drawFrame() does not got correct data length" << endl;
    }
}

void Canvas::drawSector(int dataLength, const uint8_t* data, int width, int height, int xOffset, int yOffset)
{
    if (((height + yOffset) * (width + xOffset) * 3 == dataLength) && (height + yOffset <= this->height)
            && (width + xOffset <= this->width))
    {
        int i = 0;
        for (int x = xOffset; x < xOffset + width; x++)
        {
            for (int y = xOffset; y < yOffset + height; y++)
            {
                pixel[x][y].red = data[i++];
                pixel[x][y].green = data[i++];
                pixel[x][y].blue = data[i++];
            }
        }
    }

}

void Canvas::drawRandomPixel(int x, int y)
{
    if ((x < 0) || (x >= width))
    {
        cerr << "drawRandomPixel() x out of bounds" << endl;
        return;
    }
    if ((y < 0) || (x >= height))
    {
        cerr << "drawRandomPixel() y out of bounds" << endl;
        return;
    }
    this->pixel[x][y].red = (uint8_t) rand() & 0xff;
    this->pixel[x][y].green = (uint8_t) rand() & 0xff;
    this->pixel[x][y].blue = (uint8_t) rand() & 0xff;
}

void Canvas::drawRandomPicture()
{
    for (int y = 0; y < this->height; y++)
    {
        for (int x = 0; x < this->width; x++)
        {
            this->pixel[x][y].red = (uint8_t) rand() & 0xff;
            this->pixel[x][y].green = (uint8_t) rand() & 0xff;
            this->pixel[x][y].blue = (uint8_t) rand() & 0xff;
        }
    }
}

void Canvas::drawLine(int x_start, int y_start, int x_end, int y_end, color_t color, int width)
{
    int x_diff;
    int y_diff;
    int start;
    int end;

    int n;
    float m;

//    cout << "draw line: (" << (unsigned int) x_start << "," << (unsigned int) y_start << ") (" << (unsigned int) x_end << ","
//            << (unsigned int) y_end << ") " << (unsigned int) color.red << " " << (unsigned int) color.green << " "
//            << (unsigned int) color.blue << " " << endl;

    x_diff = x_end - x_start;
    y_diff = y_end - y_start;

    /* don't draw anything if the 2 points are equal */
    if ((x_diff == 0) && (y_diff == 0))
        return;

    if (Utile::absolute(x_diff) >= Utile::absolute(y_diff))
    {
        uint8_t y;

        m = y_diff / ((float) x_diff);
        n = round(y_start - (m * x_start));

        if (x_start < x_end)
        {
            start = Utile::resize(x_start, 0, this->width - 1);
            end = Utile::resize(x_end, 0, this->width - 1);
        } else
        {
            end = Utile::resize(x_start, 0, this->width - 1);
            start = Utile::resize(x_end, 0, this->width - 1);
        }

//        cout << "m " << m << " n " << n << endl;
//        cout << "loop start: " << start << "; loop end: " << end << endl;

        for (uint8_t x = start; x <= end; x++)
        {
            y = round(m * x + n);

            if ((y < this->width) && (y >= 0))
                pixel[x][y] = color;

//            cout << "x " << (unsigned int) x << " y " << (unsigned int) y << endl;
        }
    } else /* y_diff > x_diff */
    {
        uint8_t x;

        m = x_diff / ((float) y_diff);
        n = round(x_start - (m * y_start));

        if (y_start < y_end)
        {
            start = Utile::resize(y_start, 0, this->width - 1);
            end = Utile::resize(y_end, 0, this->width - 1);
        } else
        {
            end = Utile::resize(y_start, 0, this->width - 1);
            start = Utile::resize(y_end, 0, this->width - 1);
        }

//        cout << "m " << m << " n " << n << endl;

        for (uint8_t y = y_start; y <= y_end; y++)
        {
            x = round(m * y + n);

            if ((x < this->width) && (x >= 0))
                pixel[x][y] = color;

//            cout << "x " << (unsigned int) x << " y " << (unsigned int) y << endl;
        }
    }
}

void Canvas::drawCircle(int x0, int y0, int radius, color_t color, uint8_t width)
{
    int y;

    uint8_t start = Utile::resize(x0 - radius, 0, this->width - 1);
    uint8_t end = Utile::resize(x0 + radius, 0, this->width - 1);

    for (uint8_t x = start; x <= end; x++)
    {
        /* lower half circle */
        y = round(sqrt(Utile::pow(radius, 2) - Utile::pow(x - x0, 2))) + y0;
//        cout << x << " " << y << endl;

        if ((y < this->height) && (y >= 0) && (x < this->width) && (x >= 0))
            pixel[x][y] = color;

        /* upper half circle */
        y = round(-sqrt(Utile::pow(radius, 2) - Utile::pow(x - x0, 2))) + y0;

        if ((y < this->height) && (y >= 0) && (x < this->width) && (x >= 0))
            pixel[x][y] = color;
    }

    start = Utile::resize(y0 - radius, 0, this->height - 1);
    end = Utile::resize(y0 + radius, 0, this->height - 1);

    for (uint8_t x = start; x <= end; x++)
    {
        /* right half circle */
        y = round(sqrt(Utile::pow(radius, 2) - Utile::pow(x - y0, 2))) + x0;

        if ((y < this->width) && (y >= 0) && (x < this->height) && (x >= 0))
            pixel[y][x] = color;

        /* left half circle */
        y = round(-sqrt(Utile::pow(radius, 2) - Utile::pow(x - y0, 2))) + x0;

        if ((y < this->width) && (y >= 0) && (x < this->height) && (x >= 0))
            pixel[y][x] = color;
    }
}

color_t Canvas::getPixel(int x, int y)
{
    if ((x < 0) || (x >= width))
    {
        cerr << "getPixel() x out of bounds" << endl;
        return color_black;
    }
    if ((y < 0) || (x >= height))
    {
        cerr << "getPixel() y out of bounds" << endl;
        return color_black;
    }
    return pixel[x][y];
}

vector<vector<color_t> >& Canvas::getPicture()
{
    vector<vector<color_t> >& pr = pixel;
    return pr;
}

int Canvas::getWidth()
{
    return width;
}

int Canvas::getHeight()
{
    return height;
}
