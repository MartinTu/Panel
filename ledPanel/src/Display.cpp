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

Display::Display() :
        width(1), height(1), orientation(rotateNo), numModules(1), buffersize(0), buffoffset(0), 
		actModul(0), startingModul(0), nextModul(0), gamma(true), spiThreadIsRunning(true)
{
    this->spi = new SPI();
    this->spi->init();

    initModulesWithConfigFile();

    //cout << "buffer size: " << buffersize << endl;
    buffer.resize(buffersize);
    //cout << "pixel: " << width * height << endl;
    this->master = new Canvas(width, height);
    resetNextModul();

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
    master->setColor(color_black);
    draw();
    sleep(200);
    //is this the correct deletion?
    for (unsigned int i = numModules; i > numModules; i--)
    {
        if (modul[i])
        {
            delete modul[i];
        } else
        {
            cout << "Display.cpp: modul[" << i << "] didnt exist" << endl;
        }
    }
    if (master)
    {
        delete master;
    } else
    {
        cout << "Display.cpp: master didnt exist" << endl;
    }
    this->spiThreadIsRunning = false;
    pthread_cancel(spiThread);
    if (spi)
    {
        delete spi;
    } else
    {
        cout << "Display.cpp: spi didnt exist" << endl;
    }

}

int Display::getWidth()
{
    return this->width;
}

int Display::getHeight()
{
    return this->height;
}

Canvas* Display::getCanvas()
{
    return master;
}

int Display::getNumFramePix()
{
    return (this->width * this->height);
}

int Display::getNumPix()
{
    unsigned int num = 0;
    for (unsigned int i = 0; i < numModules; i++)
    {
        num += modul[i]->getNumPix();
    }
    return num;
}

int Display::getNumModules()
{
    return this->numModules;
}

int Display::getNextModul()
{
	return this->nextModul;
}

int Display::getStartingModul()
{
	return this->startingModul;
}

void Display::resetNextModul()
{
    nextModul = startingModul;
}

int Display::drawFrameModule(unsigned int moduleNum, unsigned int dataLength, uint8_t* data)
{
    if (moduleNum == nextModul)
    {
        if (dataLength == modul[moduleNum]->getNumPix() * 3)
        {

            int i = 0;
            color_t color;
            //0-0 = topleft
            unsigned int x0 = modul[moduleNum]->getXOffset();
            unsigned int y0 = modul[moduleNum]->getYOffset();
            unsigned int x1 = x0 + modul[moduleNum]->getWidth();
            unsigned int y1 = y0 + modul[moduleNum]->getHeight();
            for (unsigned int y = y0; y < y1; y++)
            {
                for (unsigned int x = x0; x < x1; x++)
                {
                    color.red = data[i++];
                    color.green = data[i++];
                    color.blue = data[i++];
                    master->setPixel(x, y, color,1);
                }
            }
            incNextModul();
            return 0;
        } else
        {
            cerr << "drawFrameModule() does not got correct data length" << endl;
            return 1;
        }
    } else
    {
        cerr << "drawFrameModule() moduleNum " << moduleNum << " is not nextModul" << nextModul << endl;
        return 2;
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
        switch (modul[actModul]->getChip())
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

void Display::incNextModul()
{
    nextModul++;
    if (nextModul == numModules)
    {
    	resetNextModul();
     	return;
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
    memset(&buffer[buffoffset + 0], 0, 6);
    buffer[buffoffset + 4] = 0x80;

    buffoffset += 6;

    // convert pixel information to an LDP6803 conform byte array
    int posx,posy;
    int x, y;
    color_t color;
    //pixel[0][0] = topleft corner
    for (unsigned int i = 0; i < modul[actModul]->getHeight(); i++)
    {
        for (unsigned int j = 0; j < modul[actModul]->getWidth(); j++)
        {
            x = j + modul[actModul]->getXOffset();
            y = i + modul[actModul]->getYOffset();
            // bit width conversion

            modul[actModul]->reOrder(x, y, posx, posy);
            color = master->getPixel(posx, posy);
            red = color.red / 256.0 * 32.0;
            blue = color.blue / 256.0 * 32.0;
            green = color.green / 256.0 * 32.0;

            buffer[buffoffset++] = 0x80 | ((red & 0x1f) << 2) | ((green & 0x18) >> 3);
            buffer[buffoffset++] = ((green & 0x07) << 5) | ((blue & 0x1f));
        }
    }

    buffer[buffoffset++] = 0x80;  // 2 additional "black" bytes for LDP6803
    buffer[buffoffset++] = 0x0;
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
    int posx,posy;
    int x, y;
    //pixel[0][0] = topleft corner
    //buffer starts at bottomleft corner
    for (unsigned int i = 0; i < modul[actModul]->getHeight(); i++)
    {
        for (unsigned int j = 0; j < modul[actModul]->getWidth(); j++)
        {
            x = j + modul[actModul]->getXOffset();
            y = i + modul[actModul]->getYOffset();

            modul[actModul]->reOrder(x, y, posx, posy);
//			cout << setw(4) << x;
//			cout << setw(4) << y;
//			cout << setw(4) << posx;
//			cout << setw(4) << posy << endl;
            if((modul[actModul]->getCorrection() == corrGamma) || (modul[actModul]->getCorrection() == corrAll))
            {
			//corrPixel
//				buffer[buffoffset++] = round(pixel[posx][posy].red	* corrPixMatrix[numModul][posx][posy].[0]);
//				buffer[buffoffset++] = round(pixel[posx][posy].green	* corrPixMatrix[numModul][posx][posy].[1]);
//				buffer[buffoffset++] = round(pixel[posx][posy].blue	* corrPixMatrix[numModul][posx][posy].[2]);
			}
			if(((modul[actModul]->getCorrection() == corrGamma) || (modul[actModul]->getCorrection() == corrAll)) && gamma)
			{
			//corrGamma
                buffer[buffoffset++] = gamma_samples[master->getPixel(posx, posy).red];
                buffer[buffoffset++] = gamma_samples[master->getPixel(posx, posy).green];
                buffer[buffoffset++] = gamma_samples[master->getPixel(posx, posy).blue];
			}
			if(modul[actModul]->getCorrection() == corrNo)
			{
			//corrNo
                buffer[buffoffset++] = master->getPixel(posx, posy).red;
                buffer[buffoffset++] = master->getPixel(posx, posy).green;
                buffer[buffoffset++] = master->getPixel(posx, posy).blue;
            }
        }
    }
}

int Display::initModulesWithConfigFile()
{
    //file read
	
	int modulWidth;
	int modulHeight;
	int modulXOffset;
	int modulYOffset;
	int modulNumWireing;
	addressing_t modulAddressing;
	orientation_t modulOrientation;
	flip_t modulFlip;
	chip_t modulChip;
	correction_t modulCorrection;

    xml_document<> doc;    // character type defaults to char
    xml_node<> * root_node;
    string filePath = Utils::getSelfPath();
    string file("panel_config.xml");
    ifstream myFile(string(filePath + file).c_str(), ifstream::in);

    //++fileParsing
    if (myFile.good())
    {
        Utils::printStars();
        cout << "* program started from: \n* " << filePath << endl;
        cout << "* reading: " << file << endl;
        Utils::printStars();
        vector<char> myFileBuffer((istreambuf_iterator<char>(myFile)), istreambuf_iterator<char>());
        myFileBuffer.push_back('\0');
        // Parse the buffer using the xml file parsing library into doc
        doc.parse<0>(&myFileBuffer[0]);
        // Find our root node
        root_node = doc.first_node("panel");
        if (!root_node)
        {
            cerr << "* can not find <panel> tag" << endl;
        } else
        {
            xml_node<> * section_node;
            xml_node<> * key_node;

            section_node = root_node->first_node("version");
            if (!section_node)
            {
                cout << "* can not find <version> tag" << endl;
            } else
            {
                cout << "* file version: " << section_node->value() << endl;
            }
            section_node = root_node->first_node("display");
            if (!section_node)
            {
                cerr << "* can not find <display> tag" << endl;
            } else
            {
                //++read numModules
                key_node = section_node->first_node("numModules");
                if (!key_node)
                {
                    cout << "* can not find <numModules> tag" << endl;
                } else
                {
                    stringstream ss(key_node->value());
                    if (!(ss >> numModules))
                    {
                        numModules = 1;
                    }
                    if (numModules < 1)
                    {
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
                for (key_node = section_node->first_node("wireingModule"); key_node; key_node =
                        key_node->next_sibling())
                {
                    int actual = 0x7FFFFFFF;
                    stringstream ss(key_node->value());
                    if (!(ss >> actual))
                    {
                        cerr << "* wireingModule value is not a number" << endl;
                    }
                    wireingOrder.push_back(actual);
                }
                if (wireingOrder.size() < (unsigned int) numModules)
                {
                    cerr << "* not at least " << numModules << "wireingModule tag(s) found" << endl;
                } else
                {
                    wireingOrder.sort();
                    //--get wireingOrder

                    //++get idOrder
                    for (unsigned int i = 0; i < numModules; i++)
                    {
                        int neededVal = wireingOrder.front();
                        wireingOrder.pop_front();
                        for (key_node = section_node->first_node("wireingModule"); key_node;
                                key_node = key_node->next_sibling())
                        {
                            int actual = 0x7FFFFFFF;
                            stringstream ss(key_node->value());
                            if (!(ss >> actual))
                            {
                                cerr << "* wireingModule value is not a number" << endl;
                            }
                            if (neededVal == actual)
                            {
                                string actualId = "";
                                //find next in order Module
                                stringstream ss(key_node->first_attribute("id")->value());
                                if (!(ss >> actualId))
                                {
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
                    unsigned int checker = 0;
                    for (unsigned int i = 0; i < numModules; i++)
                    {
                        Utils::printStars();
                        string neededVal = idOrder.front();
                        idOrder.pop_front();
                        for (section_node = root_node->first_node("module"); section_node;
                                section_node = section_node->next_sibling())
                        {
                            string actual = "";
                            stringstream ss(section_node->first_attribute("id")->value());
                            if (!(ss >> actual))
                            {
                                cerr << "* \"id\" value is not a string" << endl;
                            }
                            if (neededVal == actual)
                            {
                                cout << "* reading module " << actual << endl;
                                //++read moduleData

                                //width
                                modulWidth = 1;
                                key_node = section_node->first_node("width");
                                if (!key_node)
                                {
                                    cerr << "* can not find <width> tag" << endl;
                                } else
                                {
                                    stringstream ss(key_node->value());
                                    if (!(ss >> modulWidth))
                                    {
                                        cerr << "* width value is not a number" << endl;
                                    }
                                    if (modulWidth < 1)
                                    {
                                        modulWidth = 1;
                                    }
                                }
                                cout << "* width " << dec << modulWidth << endl;
                                //height
                                modulHeight = 1;
                                key_node = section_node->first_node("height");
                                if (!key_node)
                                {
                                    cerr << "* can not find <height> tag" << endl;
                                } else
                                {
                                    stringstream ss(key_node->value());
                                    if (!(ss >> modulHeight))
                                    {
                                        cerr << "* height value is not a number" << endl;
                                    }
                                    if (modulHeight < 1)
                                    {
                                        modulHeight = 1;
                                    }
                                }
                                cout << "* height " << modulHeight << endl;
                                //xOffset
                                modulXOffset = 0;
                                key_node = section_node->first_node("xOffset");
                                if (!key_node)
                                {
                                    cerr << "* can not find <xOffset> tag" << endl;
                                } else
                                {
                                    stringstream ss(key_node->value());
                                    if (!(ss >> modulXOffset))
                                    {
                                        cerr << "* xOffset value is not a number" << endl;
                                    }
                                    if (modulXOffset < 0)
                                    {
                                        modulXOffset = 0;
                                    }
                                }
                                cout << "* xOffset " << modulXOffset << endl;
                                //yOffset
                                modulYOffset = 0;
                                key_node = section_node->first_node("yOffset");
                                if (!key_node)
                                {
                                    cerr << "* can not find <yOffset> tag" << endl;
                                } else
                                {
                                    stringstream ss(key_node->value());
                                    if (!(ss >> modulYOffset))
                                    {
                                        cerr << "* yOffset value is not a number" << endl;
                                    }
                                    if (modulYOffset < 0)
                                    {
                                        modulYOffset = 0;
                                    }
                                }
                                cout << "* yOffset " << modulYOffset << endl;

                                //addressing
                                modulAddressing = undef;
                                key_node = section_node->first_node("addressing");
                                if (!key_node)
                                {
                                    cerr << "* can not find <addressing> tag" << endl;
                                } else
                                {
                                    string val = key_node->value();
                                    if (!(val.compare("xyVBL")))
                                    {
                                        modulAddressing = xyVBL;
                                    } else if (!(val.compare("xyVBR")))
                                    {
                                        modulAddressing = xyVBR;
                                    } else if (!(val.compare("xyVTL")))
                                    {
                                        modulAddressing = xyVTL;
                                    } else if (!(val.compare("xyVTR")))
                                    {
                                        modulAddressing = xyVTR;
                                    } else if (!(val.compare("xyHBL")))
                                    {
                                        modulAddressing = xyHBL;
                                    } else if (!(val.compare("xyHBR")))
                                    {
                                        modulAddressing = xyHBR;
                                    } else if (!(val.compare("xyHTL")))
                                    {
                                        modulAddressing = xyHTL;
                                    } else if (!(val.compare("xyHTR")))
                                    {
                                        modulAddressing = xyHTR;
                                    } else
                                    //snake
                                    if (!(val.compare("snakeVBL")))
                                    {
                                        modulAddressing = snakeVBL;
                                    } else if (!(val.compare("snakeVBR")))
                                    {
                                        modulAddressing = snakeVBR;
                                    } else if (!(val.compare("snakeVTL")))
                                    {
                                        modulAddressing = snakeVTL;
                                    } else if (!(val.compare("snakeVTR")))
                                    {
                                        modulAddressing = snakeVTR;
                                    } else if (!(val.compare("snakeHBL")))
                                    {
                                        modulAddressing = snakeHBL;
                                    } else if (!(val.compare("snakeHBR")))
                                    {
                                        modulAddressing = snakeHBR;
                                    } else if (!(val.compare("snakeHTL")))
                                    {
                                        modulAddressing = snakeHTL;
                                    } else if (!(val.compare("snakeHTR")))
                                    {
                                        modulAddressing = snakeHTR;
                                    } else
                                    {
                                        cerr << "* addressing value is not correct" << endl;
                                    }
                                }
                                cout << "* addressing 0x" << hex << (int) modulAddressing << endl;

                                //orientation
                                modulOrientation = rotateNo;
                                key_node = section_node->first_node("orientation");
                                if (!key_node)
                                {
                                    cerr << "* can not find <orientation> tag" << endl;
                                } else
                                {
                                    string val = key_node->value();
                                    if (!(val.compare("rotateNo")))
                                    {
                                        modulOrientation = rotateNo;
                                    } else if (!(val.compare("rotateLeft")))
                                    {
                                        modulOrientation = rotateLeft;
                                    } else if (!(val.compare("rotateHalf")))
                                    {
                                        modulOrientation = rotateHalf;
                                    } else if (!(val.compare("rotateRight")))
                                    {
                                        modulOrientation = rotateRight;
                                    } else
                                    {
                                        cerr << "* orientation value is not correct" << endl;
                                    }
                                }
                                cout << "* orientation 0x" << hex << (int) modulOrientation << endl;

                                //flip
                                modulFlip = flipNo;
                                key_node = section_node->first_node("flip");
                                if (!key_node)
                                {
                                    cerr << "* can not find <flip> tag" << endl;
                                } else
                                {
                                    string val = key_node->value();
                                    if (!(val.compare("flipNo")))
                                    {
                                        modulFlip = flipNo;
                                    } else if (!(val.compare("flipX")))
                                    {
                                        modulFlip = flipX;
                                    } else if (!(val.compare("flipY")))
                                    {
                                        modulFlip = flipY;
                                    } else
                                    {
                                        cerr << "* flip value is not correct" << endl;
                                    }
                                }
                                cout << "* flip 0x" << hex << (int) modulFlip << endl;

                                //chip
                                modulChip = WS2801;
                                key_node = section_node->first_node("chip");
                                if (!key_node)
                                {
                                    cerr << "* can not find <chip> tag" << endl;
                                } else
                                {
                                    string val = key_node->value();
                                    if (!(val.compare("WS2801")))
                                    {
                                        modulChip = WS2801;
                                    } else if (!(val.compare("LDP6803")))
                                    {
                                        modulChip = LDP6803;
                                    } else
                                    {
                                        cerr << "* chip value is not correct" << endl;
                                    }
                                }
                                cout << "* chip 0x" << hex << (int) modulChip << endl;

                                //correction
                                modulCorrection = corrNo;
                                key_node = section_node->first_node("correction");
                                if (!key_node)
                                {
                                    cerr << "* can not find <correction> tag" << endl;
                                } else
                                {
                                    string val = key_node->value();
                                    if (!(val.compare("corrNo")))
                                    {
                                        modulCorrection = corrNo;
                                    } else if (!(val.compare("corrGamma")))
                                    {
                                        modulCorrection = corrGamma;
                                    } else if (!(val.compare("corrPixel")))
                                    {
                                        modulCorrection = corrPixel;
                                    } else if (!(val.compare("corrAll")))
                                    {
                                        modulCorrection = corrAll;
                                    } else
                                    {
                                        cerr << "* correction value is not correct" << endl;
                                    }
                                }
                                cout << "* correction 0x" << hex << (int) modulCorrection << endl;

                                //--read moduleData

                                //append new module
                                this->modul[i] = new DisplayModul(modulWidth, modulHeight, modulXOffset, modulYOffset, modulAddressing, modulOrientation, modulFlip, modulChip, modulCorrection);
                                checker++;
                                switch (modulChip)
                                {
                                case WS2801:
                                    // get memory for 3 byte per pixel
                                    this->buffersize += modulWidth * modulHeight * 3;
                                    break;
                                case LDP6803:
                                    // get memory for 2 byte per pixel
                                    // there are 8 bytes necessary for the LDP6803 chip: 4 header bytes, 2 additional for
                                    // the first led (of 25) which is black and 2 additional at the end for LDP6803
                                    this->buffersize += modulWidth * modulHeight * 2 + 8;
                                    break;
                                }

                                this->width = Utils::max(width, modulWidth + modulXOffset);
                                this->height = Utils::max(height, modulHeight + modulYOffset);

                                break;							//next module
                            }
                        }
                    }
                    if (numModules == checker)
                    {
                        return 0;
                    } else
                    {
                        cerr << "* did not found every wired module" << endl;
                    }
                    //--read module definitions
                }
            }
        }
    }
	else
	{
        cerr << "Unable to find file: " << file << ", in dir: " << filePath << endl;
    }
    //--fileParsing
    for (unsigned int i = numModules; i > numModules; i--)
    {
        if (modul[i])
        {
            delete modul[i];
        }
        else
		{
            cout << "Display.cpp: modul[" << i << "] didnt exist" << endl;
        }
    }
    numModules = 1;
    this->modul.resize(numModules);
	
	modulWidth 			= 1;
	modulHeight			= 1;
	modulXOffset		= 0;
	modulYOffset		= 0;
	modulAddressing		= xyVBL;
	modulOrientation	= rotateNo;
	modulFlip			= flipNo;
	modulChip			= chipNo;
	modulCorrection		= corrNo;

    this->modul[0] = new DisplayModul(modulWidth, modulHeight, modulXOffset, modulYOffset, modulAddressing, modulOrientation, modulFlip, modulChip, modulCorrection);
    switch (modulChip)
    {
    case WS2801:
        // get memory for 3 byte per pixel
        this->buffersize += modulWidth * modulHeight * 3;
        break;
    case LDP6803:
        // get memory for 2 byte per pixel
        // there are 8 bytes necessary for the LDP6803 chip: 4 header bytes, 2 additional for
        // the first led (of 25) which is black and 2 additional at the end for LDP6803
        this->buffersize += modulWidth * modulHeight * 2 + 8;
        break;
    }

    this->width  = Utils::max(modulWidth, modulWidth + xOffset);
    this->height = Utils::max(modulHeight, modulHeight + yOffset);

    return 1;
}
