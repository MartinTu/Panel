/*
 * Display.cpp
 *
 *  Created on: 31.01.2013
 *      Author: martin
 */

#include "Display.h"

using namespace std;

static void* spiThreadTaskStatic(void* o)
{
    return static_cast<Display*>(o)->spiThreadTask();
}

void *Display::spiThreadTask()
{
    vector<uint8_t> buffer;

    while (spiThreadIsRunning)
    {
        buffer = q.take();
        spi->sendData(&buffer[0], buffer.size());
    }
    return NULL;
}

Display::Display()
        :  width(0),
           height(0),
           orientation(rotateNo),
           numModules(0),
           buffersize(0),
           buffoffset(0),
           actModul(0),
           spiThreadIsRunning(true)
{
    this->spi = new SPI();
    this->spi->init();

    //file read
    numModules = 2;
    this->modul.resize(numModules);

    modul_t par;
    for(int k = 0; k < numModules; k++)
    {
    	//find next module depending on NumWireing and fill struct
    	par.addressing = snakeVBL;
    	par.chip = WS2801;
    	par.flip = flipNo;
    	par.orientation = rotateNo;
    	par.height = 10;
    	par.width = 10;
    	par.xOffset = k*10;
    	par.yOffset = 0;

    	this->modul[k] = new DisplayModul(par);
    	switch(par.chip)
    	{
    	case WS2801:
    		this->buffersize += par.width * par.height * 3;
    		break;
    	case LDP6803:
    	    /* there are 8 bytes necessary for the LDP6803 chip: 4 header bytes, 2 additional for
    	     * the first led (of 25) which is black and 2 additional at the end for LDP6803*/
    		this->buffersize += par.width * par.height * 2 + 8;
    		break;
    	}

    	width = Utile::max(width ,par.width +par.xOffset);
    	height= Utile::max(height,par.height+par.yOffset);
    }
    //cout << "buffer size: " << buffersize << endl;
    buffer.resize(buffersize);
    cout << "pixel: " << width * height << endl;
    pixel.resize(width);
    for (int i = 0; i < width; i++)
    {
        pixel[i].resize(height);
    }

    int rt = pthread_create(&spiThread, NULL, &spiThreadTaskStatic, this);
    if (rt != 0)
    {
        cout << "unable to create thread: " << strerror(errno) << endl;
        printf("Error: pthread_create() failed\n");
        exit(EXIT_FAILURE);
    }
}

Display::~Display()
{
    this->turnLEDsOff();
    usleep(200);

    this->spiThreadIsRunning = false;
    pthread_cancel(spiThread);
}

unsigned int Display::getWidth()
{
    return this->width;
}

unsigned int Display::getHeight()
{
    return this->height;
}

unsigned int Display::getNumFramePix()
{
	return (this->width * this->height);
}

unsigned int Display::getNumPix()
{
	unsigned int num = 0;
	for(int i = 0; i < numModules; i++)
	{
		num += modul[i]->getNumPix();
	}
	return num;
}

unsigned int Display::getNumModules()
{
	return this->numModules;
}

void Display::setPixel(uint8_t x, uint8_t y, struct color_t color)
{
	x = (uint8_t) Utile::resize(x,0,width);
	y = (uint8_t) Utile::resize(y,0,height);
    pixel[x][y] = color;
}

/**
 * set all pixel to the given color
 */
void Display::setColor(struct color_t color)
{
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
            pixel[i][j] = color;
}

void Display::drawLine(uint8_t x_start, uint8_t y_start, uint8_t x_end, uint8_t y_end, color_t color, uint8_t width)
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
        }
        else
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
    }
    else /* y_diff > x_diff */
    {
        uint8_t x;

        m = x_diff / ((float) y_diff);
        n = round(x_start - (m * y_start));

        if (y_start < y_end)
        {
            start = Utile::resize(y_start, 0, this->width - 1);
            end = Utile::resize(y_end, 0, this->width - 1);
        }
        else
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

/**
 * draws a cricle
 */
void Display::drawCircle(uint8_t x0, uint8_t y0, uint8_t radius, color_t color, uint8_t width)
{
    int y;

    uint8_t start = Utile::resize(x0 - radius, 0, this->width - 1);
    uint8_t end = Utile::resize(x0 + radius, 0, this->width - 1);

    for (uint8_t x = start; x <= end; x++)
    {
        /* lower half circle */
        y = round(sqrt(Utile::pow(radius, 2) - Utile::pow(x - x0, 2))) + y0;
//        cout << x << " " << y << endl;

        if ((y < (int) this->height) && (y >= 0) && (x < (int) this->width) && (x >= 0))
            pixel[x][y] = color;

        /* upper half circle */
        y = round(-sqrt(Utile::pow(radius, 2) - Utile::pow(x - x0, 2))) + y0;

        if ((y < (int) this->height) && (y >= 0) && (x < (int) this->width) && (x >= 0))
            pixel[x][y] = color;
    }

    start = Utile::resize(y0 - radius, 0, this->height - 1);
    end = Utile::resize(y0 + radius, 0, this->height - 1);

    for (uint8_t x = start; x <= end; x++)
    {
        /* right half circle */
        y = round(sqrt(Utile::pow(radius, 2) - Utile::pow(x - y0, 2))) + x0;

        if ((y < (int) this->width) && (y >= 0) && (x < (int) this->height) && (x >= 0))
            pixel[y][x] = color;

        /* left half circle */
        y = round(-sqrt(Utile::pow(radius, 2) - Utile::pow(x - y0, 2))) + x0;

        if ((y < (int) this->width) && (y >= 0) && (x < (int) this->height) && (x >= 0))
            pixel[y][x] = color;
    }
}

/**
 * draw to the LDP6801 register
 * UNTESTED!!
 */
void Display::drawLDP6803()
{
    unsigned int red, green, blue;

    /* there are 8 bytes necessary for the LDP6803 chip: 4 header bytes, 2 additional for
     * the first led (of 25) which is black and 2 additional at the end for LDP6803*/


    // first 4 bytes needs to be 0 (header) followed by a black LED (buffer[4] = 0x80 and buffer[5] = 0x00)
    memset(&buffer[buffoffset+0], 0, 6);
    buffer[buffoffset+4] = 0x80;

    buffoffset += 6;

    // convert pixel information to an LDP6803 conform byte array
    position_t pos;
    int x,y;
	for (int i = 0; i < modul[actModul]->getHeight(); i++)
	{
		for (int j = 0; j < modul[actModul]->getWidth(); j++)
		{
			x = j + modul[actModul]->getXOffset();
			y = i + modul[actModul]->getYOffset();
			// bit width conversion

			modul[actModul]->reOrder(x,y,pos);
			red   = pixel[pos.x][pos.y].red   / 256.0 * 32.0;
			blue  = pixel[pos.x][pos.y].blue  / 256.0 * 32.0;
			green = pixel[pos.x][pos.y].green / 256.0 * 32.0;

			buffer[buffoffset++] = 0x80 |((red   & 0x1f) << 2) | ((green & 0x18) >> 3);
			buffer[buffoffset++] =       ((green & 0x07) << 5) | ((blue  & 0x1f));
		}
	}

    buffer[buffoffset++] = 0x80; // 2 additional "black" bytes for LDP6803
    buffer[buffoffset++] = 0x0;

    q.put(buffer);
}

/**
 * draw to the WS2801 register
 */
void Display::drawWS2801()
{
//    cout << hex;
//    for (unsigned int i = 0; i < height; i++)
//    {
//        for (unsigned int j = 0; j < width; j++)
//        {
//            cout << "["<<(unsigned int) pixel[j][i].red << ","<<(unsigned int) pixel[j][i].green<<","<<(unsigned int) pixel[j][i].blue<<"]\t";
//        }
//        cout << "\n";
//    }
//    cout << dec << "\n" << endl;

	// convert pixel information to an WS2801 conform byte array
	position_t pos;
	int x,y;
	for (int i = 0; i < modul[actModul]->getHeight(); i++)
	{
		for (int j = 0; j < modul[actModul]->getWidth(); j++)
		{
			x = j + modul[actModul]->getXOffset();
			y = i + modul[actModul]->getYOffset();

			modul[actModul]->reOrder(x,y,pos);
//			cout << setw(4) << x;
//			cout << setw(4) << y;
//			cout << setw(4) << pos->x;
//			cout << setw(4) << pos->y << endl;

//			pixel[x][y].red = x*20;
//			pixel[x][y].green = y*10;
//			pixel[x][y].blue = actModul *255;

			buffer[buffoffset++] = pixel[pos.x][pos.y].red;
			buffer[buffoffset++] = pixel[pos.x][pos.y].green;
			buffer[buffoffset++] = pixel[pos.x][pos.y].blue;
		}
	}
}

/**
 * draw the internal pixel data
 */
void Display::draw()
{
	buffoffset = 0;
	for (actModul = 0; actModul < numModules; actModul++)
	{
		switch(modul[actModul]->getChip())
		{
		case WS2801:
			this->drawWS2801();
			break;
		case LDP6803:
			this->drawLDP6803();
			break;
		default:
			break;
		}
	}
	q.put(buffer);
}

/*
 * turns the lights off
 */
void Display::turnLEDsOff()
{
    for (int i = 0; i < this->width; i++)
    {
        for (int j = 0; j < this->height; j++)
        {
            this->pixel[i][j].red = 0;
            this->pixel[i][j].green = 0;
            this->pixel[i][j].blue = 0;
        }
    }
    this->draw();
}

void Display::showBootLogo()
{
    float brightness = 0;
    for (unsigned int k = 0; k < 100; k++)
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                pixel[j][i].red = (invader[i][j * 3] * brightness);
                pixel[j][i].green = (invader[i][j * 3 + 1] * brightness);
                pixel[j][i].blue = (invader[i][j * 3 + 2] * brightness);
            }
        }

        this->draw();

        brightness = (k * k) / (100.0 * 100.0);
        usleep(40e3);
    }
}

void Display::drawRandom()
{
    //seed
    srand((unsigned int) Utile::getTime());

    for (int i = 0; i < this->height; i++)
    {
        for (int j = 0; j < this->width; j++)
        {
            this->pixel[j][i].red = (uint8_t) rand() & 0xff;
            this->pixel[j][i].green = (uint8_t) rand() & 0xff;
            this->pixel[j][i].blue = (uint8_t) rand() & 0xff;
        }
    }
    this->draw();
}


