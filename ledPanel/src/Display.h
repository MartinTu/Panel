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
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

#include "Utile.h"
#include "spi.h"
#include "DisplayModul.h"
#include "BlockingQueue.h"
#include "colorManipulation.h"
#include "rapidxml.hpp"

#include "bitmaps.h"

using namespace std;

class Display
{
    public:
        Display();
        virtual ~Display();

        unsigned int getWidth();
        unsigned int getHeight();
        unsigned int getNumFramePix();
        unsigned int getNumPix();
        unsigned int getNumModules();

        unsigned int getModule(uint8_t x, uint8_t y);

        void turnLEDsOff();
        void drawRandom();

        void setPixel(uint8_t x, uint8_t y, struct color_t);
        void setColor(struct color_t color);
        void drawLine(uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end, color_t color, uint8_t width);
        void drawCircle(uint8_t x0, uint8_t y0, uint8_t radius, color_t color, uint8_t width);

        //void drawFrame(uint32_t dataLength, const uint8_t * data);

        void draw();


        //void draw(uint32_t dataLength, const uint8_t * data);

        void showBootLogo();

        void *spiThreadTask();

    protected:
        void drawLDP6803();
        void drawWS2801();
        int initModulesWithConfigFile();

        int width;//display width
        int height;//display height
        orientation_t orientation;


        int numModules;
        unsigned int buffersize;
        unsigned int buffoffset;
        int actModul;


        bool spiThreadIsRunning;
        pthread_t spiThread;
        SPI *spi;

        std::vector<DisplayModul *> modul;
        BlockingQueue<std::vector<uint8_t > > q;
        std::vector<std::vector<struct color_t > > pixel;//x/y Display picture
        std::vector<uint8_t > buffer;//hardware Display picture
};

#endif /* DISPLAY_H_ */
