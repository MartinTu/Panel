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

#include "Utils.h"
#include "Spi.h"
#include "DisplayModul.h"
#include "BlockingQueue.h"
#include "Color.h"
#include "Canvas.h"

#include "rapidxml.hpp"//xml file reading

using namespace std;

class Display
{
    public:
        Display();
        virtual ~Display();

        int getWidth();
        int getHeight();
        Canvas* getCanvas();
        int getNumFramePix();
        int getNumPix();
        int getNumModules();

        void drawFrameModule(unsigned int moduleNum, unsigned int dataLength, uint8_t* data);
        bool getModulDrawn();
        void resetModulDrawn();

        void draw();

        void *spiThreadTask();

    protected:

        void setModulDrawn(unsigned int modulNum);
        void drawLDP6803();
        void drawWS2801();
        int initModulesWithConfigFile();

        unsigned int width;  //display width
        unsigned int height;  //display height
        orientation_t orientation;

        unsigned int numModules;

        unsigned int buffersize;
        unsigned int buffoffset;
        unsigned int actModul;
        double gamma;

        bool spiThreadIsRunning;
        pthread_t spiThread;
        SPI* spi;
        Canvas* master;
        std::vector<bool> modulDrawn;
        std::vector<DisplayModul *> modul;
        BlockingQueue<std::vector<uint8_t> > q;
        std::vector<uint8_t> buffer;  //hardware Display picture
};

#endif /* DISPLAY_H_ */
