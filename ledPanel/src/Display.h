/*
 * Display.h
 *
 *  Created on: 31.01.2013
 *      Author: martin
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <vector>
#include <cmath>
#include <pthread.h>

#include "spi.h"
#include "BlockingQueue.h"
#include "Utile.h"

#include "bitmaps.h"

struct color_t
{
        uint8_t red;
        uint8_t green;
        uint8_t blue;
};

enum chip_t
{
    LDP6803 = 0, WS2801
};

static const color_t color_black = { 0, 0, 0 };
static const color_t color_dark_blue = { 0, 0, 20 };

class Display
{
    public:
        Display(unsigned int width, unsigned int height, chip_t chip);
        virtual ~Display();

        unsigned int getWidth();
        unsigned int getHeight();

        void pulseColor(struct color_t color, unsigned long delay, uint8_t minValue = 0x00, uint8_t maxValue = 0xff);
        void turnLEDsOff();
        void drawRandom();

        void setPixel(uint8_t x, uint8_t y, struct color_t);
        void setColor(struct color_t color);
        void drawLine(uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end, color_t color, uint8_t width);
        void drawCircle(uint8_t x0, uint8_t y0, uint8_t radius, color_t color, uint8_t width);
        void draw();
        void draw(uint32_t dataLength, const uint8_t * data);

        void showBootLogo();

        void *spiThreadTask();

    protected:
        void drawLDP6803();
        void drawWS2801();

        SPI *spi;
        chip_t chip;
        unsigned int width;
        unsigned int height;
        unsigned int orientation;
        pthread_t spiThread;
        bool spiThreadIsRunning;
        BlockingQueue<std::vector<uint8_t> > q;
        std::vector<std::vector<struct color_t> > pixel;

};

#endif /* DISPLAY_H_ */
