/*
 * Display.cpp
 *
 *  Created on: 31.01.2013
 *      Author: martin
 */

#include "Display.h"

using namespace rapidxml;
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
        :  width(1),
           height(1),
           orientation(rotateNo),
           numModules(1),
           buffersize(0),
           buffoffset(0),
           actModul(0),
           spiThreadIsRunning(true)
{
    this->spi = new SPI();
    this->spi->init();

    initModulesWithConfigFile();

    //cout << "buffer size: " << buffersize << endl;
    buffer.resize(buffersize);
    //cout << "pixel: " << width * height << endl;
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
    //is this the correct deletion?
    for(int i = numModules; i > numModules; i--){
    	delete modul[i];
    }

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

int Display::initModulesWithConfigFile(){
	//file read

	modul_t par;

	xml_document<> doc;    // character type defaults to char
	xml_node<> * root_node;
	string fileName("panel_config.xml");
	ifstream myFile (fileName);

	//++fileParsing
	if(myFile.good()){
		Utile::printStars();
		cout << "* reading: " << fileName << endl;
		vector<char> myFileBuffer((istreambuf_iterator<char>(myFile)), istreambuf_iterator<char>());
		myFileBuffer.push_back('\0');
		// Parse the buffer using the xml file parsing library into doc
		doc.parse<0>(&myFileBuffer[0]);
		// Find our root node
		root_node = doc.first_node("panel");
		if(!root_node){
			cerr << "* can not find <panel> tag" << endl;
		}
		else{
			xml_node<> * section_node;
			xml_node<> * key_node;

			section_node = root_node->first_node("version");
			if (!section_node){
				cout << "* can not find <version> tag" << endl;
			}
			else{
				cout << "* file version: " << section_node->value() << endl;
			}
			section_node = root_node->first_node("display");
			if (!section_node){
				cerr << "* can not find <display> tag" << endl;
			}
			else
			{
				//++read numModules
				key_node = section_node->first_node("numModules");
				if(!key_node){
					cout << "* can not find <numModules> tag" << endl;
				}
				else{
					stringstream ss(key_node->value());
					if ( !(ss >> numModules) ){
						numModules = 1;
					}
					if(numModules < 1){
						numModules = 1;
					}
					cout << "* number of modules: " << key_node->value() << endl;
				}
				//--read numModules

				this->modul.resize(numModules);
				list<int> wireingOrder;
				list<string> idOrder;
				//++get wireingOrder

				//iterate through all wireingOrder Tags
				for(key_node = section_node->first_node("wireingModule"); key_node; key_node = key_node->next_sibling()){
					int actual = 0x7FFFFFFF;
					stringstream ss(key_node->value());
					if( !(ss >> actual) ){
						cerr << "* wireingModule value is not a number" << endl;
					}
					wireingOrder.push_back(actual);
				}
				if(wireingOrder.size() < (unsigned int) numModules){
					cerr << "* not at least "<< numModules << "wireingModule tag(s) found" << endl;
				}
				else{
					wireingOrder.sort();
					//--get wireingOrder

					//++get idOrder
					for(int i = 0; i < numModules; i++){
						int neededVal = wireingOrder.front();
						wireingOrder.pop_front();
						for(key_node = section_node->first_node("wireingModule"); key_node; key_node = key_node->next_sibling()){
							int actual = 0x7FFFFFFF;
							stringstream ss(key_node->value());
							if( !(ss >> actual) ){
								cerr << "* wireingModule value is not a number" << endl;
							}
							if (neededVal == actual){
								string actualId = "";
								//find next in order Module
								stringstream ss(key_node->first_attribute("id")->value());
								if( !(ss >> actualId) ){
									cerr << "* \"id\" value is not a string" << endl;
								}
								idOrder.push_back(actualId);
								cout << "* module " << i << ": " << actualId << endl;
								break;
							}
						}
					}
					//--get idOrder

					//++read module definitions
					int checker = 0;
					for(int i = 0; i < numModules; i++){
						Utile::printStars();
						string neededVal = idOrder.front();
						idOrder.pop_front();
						for(section_node = root_node->first_node("module"); section_node; section_node = section_node->next_sibling()){
							string actual = "";
							stringstream ss(section_node->first_attribute("id")->value());
							if( !(ss >> actual) ){
								cerr << "* \"id\" value is not a string" << endl;
							}
							if (neededVal == actual){
								cout << "* reading module " << actual << endl;
								//++read moduleData

								//width
								par.width = 1;
								key_node = section_node->first_node("width");
								if(!key_node){
									cerr << "* can not find <width> tag" << endl;
								}
								else{
									stringstream ss(key_node->value());
									if( !(ss >> par.width) ){
										cerr << "* width value is not a number" << endl;
									}
									if(par.width < 1){
										par.width = 1;
									}
								}
								cout << "* width " << par.width << endl;
								//height
								par.height = 1;
								key_node = section_node->first_node("height");
								if(!key_node){
									cerr << "* can not find <height> tag" << endl;
								}
								else{
									stringstream ss(key_node->value());
									if( !(ss >> par.height) ){
										cerr << "* height value is not a number" << endl;
									}
									if(par.height < 1){
										par.height = 1;
									}
								}
								cout << "* height " << par.height << endl;
								//xOffset
								par.xOffset = 0;
								key_node = section_node->first_node("xOffset");
								if(!key_node){
									cerr << "* can not find <xOffset> tag" << endl;
								}
								else{
									stringstream ss(key_node->value());
									if( !(ss >> par.xOffset) ){
										cerr << "* xOffset value is not a number" << endl;
									}
									if(par.xOffset < 0){
										par.xOffset = 0;
									}
								}
								cout << "* xOffset " << par.xOffset << endl;
								//yOffset
								par.yOffset = 0;
								key_node = section_node->first_node("yOffset");
								if(!key_node){
									cerr << "* can not find <yOffset> tag" << endl;
								}
								else{
									stringstream ss(key_node->value());
									if( !(ss >> par.yOffset) ){
										cerr << "* yOffset value is not a number" << endl;
									}
									if(par.yOffset < 0){
										par.yOffset = 0;
									}
								}
								cout << "* yOffset " << par.yOffset << endl;

								//addressing
								par.addressing = undef;
								key_node = section_node->first_node("addressing");
								if(!key_node){
									cerr << "* can not find <addressing> tag" << endl;
								}
								else{
									string val = key_node->value();
									if( !(val.compare("xyVBL")) ){
										par.addressing = xyVBL;
									}
									else
									if( !(val.compare("xyVBR")) ){
										par.addressing = xyVBR;
									}
									else
									if( !(val.compare("xyVTL")) ){
										par.addressing = xyVTL;
									}
									else
									if( !(val.compare("xyVTR")) ){
										par.addressing = xyVTR;
									}
									else
									if( !(val.compare("xyHBL")) ){
										par.addressing = xyHBL;
									}
									else
									if( !(val.compare("xyHBR")) ){
										par.addressing = xyHBR;
									}
									else
									if( !(val.compare("xyHTL")) ){
										par.addressing = xyHTL;
									}
									else
									if( !(val.compare("xyHTR")) ){
										par.addressing = xyHTR;
									}
									else
									//snake
									if( !(val.compare("snakeVBL")) ){
										par.addressing = snakeVBL;
									}
									else
									if( !(val.compare("snakeVBR")) ){
										par.addressing = snakeVBR;
									}
									else
									if( !(val.compare("snakeVTL")) ){
										par.addressing = snakeVTL;
									}
									else
									if( !(val.compare("snakeVTR")) ){
										par.addressing = snakeVTR;
									}
									else
									if( !(val.compare("snakeHBL")) ){
										par.addressing = snakeHBL;
									}
									else
									if( !(val.compare("snakeHBR")) ){
										par.addressing = snakeHBR;
									}
									else
									if( !(val.compare("snakeHTL")) ){
										par.addressing = snakeHTL;
									}
									else
									if( !(val.compare("snakeHTR")) ){
										par.addressing = snakeHTR;
									}
									else{
										cerr << "* addressing value is not correct" << endl;
									}
								}
								cout << "* addressing " << par.addressing << endl;

								//orientation
								par.orientation = rotateNo;
								key_node = section_node->first_node("orientation");
								if(!key_node){
									cerr << "* can not find <orientation> tag" << endl;
								}
								else{
									string val = key_node->value();
									if( !(val.compare("rotateNo")) ){
										par.orientation = rotateNo;
									}
									else
									if( !(val.compare("rotateLeft")) ){
										par.orientation = rotateLeft;
									}
									else
									if( !(val.compare("rotateHalf")) ){
										par.orientation = rotateHalf;
									}
									else
									if( !(val.compare("rotateRight")) ){
										par.orientation = rotateRight;
									}
									else{
										cerr << "* orientation value is not correct" << endl;
									}
								}
								cout << "* orientation " << par.orientation << endl;

								//flip
								par.flip = flipNo;
								key_node = section_node->first_node("flip");
								if(!key_node){
									cerr << "* can not find <flip> tag" << endl;
								}
								else{
									string val = key_node->value();
									if( !(val.compare("flipNo")) ){
										par.flip = flipNo;
									}
									else
									if( !(val.compare("flipX")) ){
										par.flip = flipX;
									}
									else
									if( !(val.compare("flipY")) ){
										par.flip = flipY;
									}
									else{
										cerr << "* flip value is not correct" << endl;
									}
								}
								cout << "* flip " << par.flip << endl;

								//chip
								par.chip = WS2801;
								key_node = section_node->first_node("chip");
								if(!key_node){
									cerr << "* can not find <chip> tag" << endl;
								}
								else{
									string val = key_node->value();
									if( !(val.compare("WS2801")) ){
										par.chip = WS2801;
									}
									else
									if( !(val.compare("LDP6803")) ){
										par.chip = LDP6803;
									}
									else{
										cerr << "* chip value is not correct" << endl;
									}
								}
								cout << "* chip " << par.chip << endl;

								//correction
								par.correction = corrNo;
								key_node = section_node->first_node("correction");
								if(!key_node){
									cerr << "* can not find <correction> tag" << endl;
								}
								else{
									string val = key_node->value();
									if( !(val.compare("corrNo")) ){
										par.correction = corrNo;
									}
									else
									if( !(val.compare("corrGamma")) ){
										par.correction = corrGamma;
									}
									else
									if( !(val.compare("corrPixel")) ){
										par.correction = corrPixel;
									}
									else
									if( !(val.compare("corrAll")) ){
										par.correction = corrAll;
									}
									else{
										cerr << "* correction value is not correct" << endl;
									}
								}
								cout << "* correction " << par.correction << endl;

								//--read moduleData

								//append new module
								this->modul[i] = new DisplayModul(par);
								checker++;
								switch(par.chip)
								{
								case WS2801:
									// get memory for 3 byte per pixel
									this->buffersize += par.width * par.height * 3;
									break;
								case LDP6803:
									// get memory for 2 byte per pixel
									// there are 8 bytes necessary for the LDP6803 chip: 4 header bytes, 2 additional for
									// the first led (of 25) which is black and 2 additional at the end for LDP6803
									this->buffersize += par.width * par.height * 2 + 8;
									break;
								}

								width = Utile::max(width ,par.width +par.xOffset);
								height= Utile::max(height,par.height+par.yOffset);

								break;//next module
							}
						}
					}
					if(numModules == checker){
						return 0;
					}
					else{
						cerr << "* did not found every wired module" << endl;
					}
					//--read module definitions
				}
			}
		}
	}
	else{
		cerr << "Unable to find file"<< fileName << endl;
	}
	//--fileParsing
	for(int i = numModules; i > numModules; i--){
		delete modul[i];
	}
	numModules = 1;
	this->modul.resize(numModules);

	//fill struct with default parameters
	par.addressing = xyVBL;
	par.chip = WS2801;
	par.flip = flipNo;
	par.orientation = rotateNo;
	par.correction = corrNo;
	par.height = 1;
	par.width = 1;
	par.xOffset = 0;
	par.yOffset = 0;

	this->modul[0] = new DisplayModul(par);
	switch(par.chip)
	{
	case WS2801:
		// get memory for 3 byte per pixel
		this->buffersize += par.width * par.height * 3;
		break;
	case LDP6803:
		// get memory for 2 byte per pixel
		// there are 8 bytes necessary for the LDP6803 chip: 4 header bytes, 2 additional for
		// the first led (of 25) which is black and 2 additional at the end for LDP6803
		this->buffersize += par.width * par.height * 2 + 8;
		break;
	}

	width = Utile::max(width ,par.width +par.xOffset);
	height= Utile::max(height,par.height+par.yOffset);

	return 1;
}
