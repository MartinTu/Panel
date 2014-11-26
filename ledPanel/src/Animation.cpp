/*
 * Animation.cpp
 *
 *  Created on: 06.09.2014
 *      Author: keKs
 */

#include "Animation.h"

Animation::Animation(int _width, int _height) :
        width(_width), height(_height), aniDelay(0), aniDelayIterator(0), runningAni(aniNone), lastAni(aniNone), mixer(
                mixMaybe)
{
    animation_t startingAni = aniDirectionFade;
    parameter.clear();
    parameter.push_back(0x01);  //diff
    parameter.push_back(dirRightBot);  //dir
    parameter.push_back(0xaa);  //value
    parameter.push_back(0xff);  //saturation -- unused

    //set running ani
    this->set(startingAni, parameter, parameter.length());

    this->frame = new Canvas(width, height);
}
Animation::~Animation()
{
    delete frame;
}

int Animation::set(uint8_t ani, string &param, unsigned int paramSize)
{
    int ret = -1;
    lastAni = runningAni;
    if (ani < _aniNUM)
    {
        runningAni = static_cast<animation_t>(ani);
    } else
    {
        cerr << ani << " is no animation_t" << endl;
        runningAni = aniNone;
    }

    switch (runningAni)
    {
    case aniNone:
        cout << "aniNone";
        parameter.clear();
        internPar.clear();
        break;
    case aniInvader:
        cout << "aniInvader";
        if (paramSize == (unsigned int) lenInvader)
        {
            parameter = param.substr(0, paramSize);
            aniDelay = 0;
            ret = 0;
            if (runningAni != lastAni)
            {
                //initialize intern parameter
                internPar.clear();
                internPar.push_back(0x00);
            }
        } else
        {
            cerr << " param out of bounds: " << paramSize << endl;
        }
        break;
    case aniDirectionFade:
        cout << "aniDirectionFade";
        if (paramSize == (unsigned int) lenDirectionFade)
        {
            parameter = param.substr(0, paramSize);
            aniDelay = 3;
            ret = 0;
            if (runningAni != lastAni)
            {
                //initialize intern parameter
                internPar.clear();
                internPar.push_back(0xff);  //red
                internPar.push_back(0x00);  //green
                internPar.push_back(0x00);  //blue
            }
        } else
        {
            cerr << " param out of bounds: " << paramSize << endl;
        }
        break;
    case aniScreenFade:
        cout << "aniScreenFade";
        if (paramSize == (unsigned int) lenScreenFade)
        {
            parameter = param.substr(0, paramSize);
            aniDelay = 0;
            ret = 0;
        } else
        {
            cerr << " param out of bounds: " << paramSize << endl;
        }
        break;
    case aniScreenPulse:
        cout << "aniScreenPulse";
        if (paramSize == (unsigned int) lenScreenPulse)
        {
            parameter = param.substr(0, paramSize);
            aniDelay = 0;
            ret = 0;
        } else
        {
            cerr << " param out of bounds: " << paramSize << endl;
        }
        break;
    case aniRotateLine:
        cout << "aniRotateLine";
        if (paramSize == (unsigned int) lenRotateLine)
        {
            parameter = param.substr(0, paramSize);
            aniDelay = 2;
            ret = 0;
        } else
        {
            cerr << " param out of bounds: " << paramSize << endl;
        }
        break;
    case aniWaterdrop:
        cout << "aniWaterdrop";
        if (paramSize == (unsigned int) lenWaterdrop)
        {
            parameter = param.substr(0, paramSize);
            aniDelay = 2;
            ret = 0;
        } else
        {
            cerr << " param out of bounds: " << paramSize << endl;
        }
        break;
    case aniFadingPixels:
        cout << "aniFadingPixels";
        if (paramSize == (unsigned int) lenFadingPixels)
        {
            parameter = param.substr(0, paramSize);
            parameter[1] = 0;
            aniDelay = 1;
            ret = 0;
        } else
        {
            cerr << " param out of bounds: " << paramSize << endl;
        }
        break;
    default:
        break;
    }
    if (ret == 0)
    {
        if (!param.empty())
        {
            cout << ": param(0x";
            for (unsigned int i = 0; i < parameter.length(); i++)
            {
                cout << hex << setw(3) << (int) parameter[i];
            }
            cout << ") ";
            if (internPar.empty())
            {
                cout << endl;
            }
        }
        if (!internPar.empty())
        {
            cout << "internPar(0x";
            for (unsigned int i = 0; i < internPar.length(); i++)
            {
                cout << hex << setw(3) << (int) internPar[i];
            }
            cout << ")" << endl;
        }
        return 0;
    }
    runningAni = aniNone;
    parameter.clear();
    internPar.clear();
    cout << "animation set() error" << endl;
    return -1;  //error at paramSize
}

void Animation::reset()
{
    lastAni = runningAni;
    runningAni = aniNone;
    parameter.clear();
    internPar.clear();
}

bool Animation::nextStep()
{
    if ((mixer != mixOff) && (aniDelayIterator-- == 0))
    {
//        cout << "aniParam: 0x";
//        for (unsigned int i = 0; i < parameter.length(); i++)
//        {
//            cout << hex << setw(3) << (int) parameter[i];
//        }
//        cout << endl;
        aniDelayIterator = aniDelay;
        switch (runningAni)
        {
        case aniInvader:
            invader();
            break;
        case aniDirectionFade:
            directionFade();
            break;
        case aniScreenFade:
            screenFade();
            break;
        case aniScreenPulse:
            screenPulse();
            break;
        case aniRotateLine:
            rotateLine();
            break;
        case aniWaterdrop:
            waterdrop();
            break;
        case aniFadingPixels:
            fadingPixels();
            break;
        default:
            return 0;
        }
        return 1;
    } else
    {
        return 0;
    }
}

void Animation::setMixer(uint8_t _mixer)
{
    if (_mixer < _mixNUM)
    {
        this->mixer = static_cast<mixer_t>(_mixer);
    } else
    {
        cerr << _mixer << " is no mixer_t" << endl;
    }
}

mixer_t Animation::getMixer()
{
    return mixer;
}

Canvas * Animation::getCanvas()
{
    return frame;
}

void Animation::setAniDelay(unsigned int delay)
{
    aniDelay = delay;
    aniDelayIterator = 0;
}

bool Animation::isAnimationRunning()
{
    if (runningAni)
    {  //if (not noAnimation = 0x00)
        return true;
    }
    return false;
}

void Animation::invader()
{
    /* ani 0x01
     * paramSize: 1
     * [0] actWheel
     */
    color_t color;
    int invaderOK = width * height;
    bool picture;
    uint8_t actWheel = (uint8_t) parameter[0];
    if (actWheel < 128)
    {
        picture = true;
    } else
    {
        picture = false;
    }
    uint8_t pictureWheel;
    //check resolution of DISPLAY and correct config
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            pictureWheel = actWheel++;
            if (invaderOK == 200)
            {  //max
                if (picture)
                {
                    if (bit_invader1_10x20[y][x * 3])
                    {
                        pictureWheel += 128;
                    }
                } else
                {
                    int nx = x - 1;
                    if (nx < 0)
                    {
                        nx = nx + width;
                    }
                    if (bit_invader2_10x20[y][nx * 3])
                    {
                        pictureWheel += 128;
                    }
                }
            } else if (invaderOK == 144)
            {  //martin
                if (bit_invader[y][x * 3])
                {
                    pictureWheel += 128;
                }
            }
            color = ColorMan::wheel(pictureWheel);
            frame->setPixel(x, y, color);
        }
    }
    parameter[0]++;
}

void Animation::directionFade()
{
    /* ani 0x02
     * paramSize: 4
     * [0]: delta
     * [1]: direction //original color comes from reversed direction
     * [2]: colorDimmer
     * [3]: saturation //todo: implement
     * internPar:
     * [0]: red
     * [1]: green
     * [2]: blue
     */

    color_t color((uint8_t) internPar[0], (uint8_t) internPar[1], (uint8_t) internPar[2]);
    color_t calcColor;

    calcColor.red = color.red * (float) (parameter[2] / 255.0);
    calcColor.green = color.green * (float) (parameter[2] / 255.0);
    calcColor.blue = color.blue * (float) (parameter[2] / 255.0);

    uint8_t delta = (uint8_t) parameter[0];
    direction_t dir = static_cast<direction_t>(parameter[1]);
    int xmax = width;
    int ymax = height;
    int w = width - 1;
    int h = height - 1;
    switch (dir)
    {
    case dirTop:
        xmax = height;
        ymax = width;
        break;
    case dirBottom:
        xmax = height;
        ymax = width;
        break;
    case dirLeftBot:
        xmax = height;
        ymax = width;
        xmax += w;
        break;
    case dirRightBot:
        xmax = height;
        ymax = width;
        xmax += w;
        break;
    case dirRightTop:
        xmax += h;
        break;
    case dirLeftTop:
        xmax += h;
        break;
    default:
        break;
    }

    for (uint8_t x = 0; x < xmax; x++)
    {

        for (uint8_t y = 0; y < ymax; y++)
        {
            switch (dir)
            {
            case dirTop:
            {
                frame->setPixel(y, x, calcColor);
                break;
            }
            case dirLeft:
            {
                frame->setPixel(x, y, calcColor);
                break;
            }
            case dirRightBot:
            {
                int xi = x - y;
                if (Utils::isInRange(xi, 0, h))
                    frame->setPixel(w - y, h - xi, calcColor);
                break;
            }
            case dirLeftTop:
            {
                int xi = x - y;
                if (Utils::isInRange(xi, 0, w))
                    frame->setPixel(xi, y, calcColor);
                break;
            }
            case dirBottom:
            {
                frame->setPixel(y, h - x, calcColor);
                break;
            }
            case dirRight:
            {
                frame->setPixel(w - x, y, calcColor);
                break;
            }
            case dirLeftBot:
            {
                int xi = x - y;
                if (Utils::isInRange(xi, 0, h))
                    frame->setPixel(y, xi, calcColor);
                break;
            }
            case dirRightTop:
            {
                int xi = x - y;
                if (Utils::isInRange(xi, 0, w))
                    frame->setPixel(w - xi, y, calcColor);
                break;
            }
            default:
                break;
            }
        }
        color = ColorMan::rgbFade(color, delta);


        calcColor.red = color.red * (float) (parameter[2] / 255.0);
        calcColor.green = color.green * (float) (parameter[2] / 255.0);
        calcColor.blue = color.blue * (float) (parameter[2] / 255.0);


        /* set new start color */
        if (x == 1)
        {
            internPar[0] = color.red;
            internPar[1] = color.green;
            internPar[2] = color.blue;
        }
    }

}

void Animation::screenFade()
{
    /* ani 0x03
     * paramSize: 5
     * [0]: maxBrightness
     * [1]: fadeStatus
     * [2]: red
     * [3]: green
     * [4]: blue
     */
    float maxBrightness = parameter[0] / 255.0;
    uint8_t fadeStatus = (uint8_t) parameter[1];
    color_t color;
    color.red = (uint8_t) parameter[2];
    color.green = (uint8_t) parameter[3];
    color.blue = (uint8_t) parameter[4];
    color = color * maxBrightness;

    frame->setColor(color);

    if (0x0 == fadeStatus)
    {
        color.red--;
        color.green++;

        if (0 == color.red)
            fadeStatus++;
    } else if (0x01 == fadeStatus)
    {
        color.green--;
        color.blue++;

        if (0 == color.green)
            fadeStatus++;
    } else if (0x02 == fadeStatus)
    {
        color.blue--;
        color.red++;

        if (0 == color.blue)
            fadeStatus = 0;
    }
    parameter[1] = fadeStatus;
    parameter[2] = color.red;
    parameter[3] = color.green;
    parameter[4] = color.blue;
}

void Animation::screenPulse()
{
    /* ani 0x04
     * paramSize: 5
     * [0]: brightness
     * [1]: getsBrighter
     * [2]: red
     * [3]: green
     * [4]: blue
     */
    color_t color;

    float val = (parameter[0] * parameter[0]) / (255.0 * 255.0);

    color.red = parameter[2] * val;
    color.green = parameter[3] * val;
    color.blue = parameter[4] * val;

    if (parameter[1])
    {
        if (0xff == ++parameter[0])
            parameter[1] = false;
    } else
    {
        if (0x00 == --parameter[0])
            parameter[1] = true;
    }
    frame->setColor(color);
}

void Animation::rotateLine()
{
    /* ani 0x05
     * paramSize: 7
     * [0]: p_x1
     * [1]: p_y1
     * [2]: p_x2
     * [3]:	p_y2
     * [4]: red
     * [5]:	green
     * [6]:	blue
     */
    color_t color;
    int p_x1 = parameter[0];
    int p_y1 = parameter[1];
    int p_x2 = parameter[2];
    int p_y2 = parameter[3];
    color.red = parameter[4];
    color.green = parameter[5];
    color.blue = parameter[6];

// TODO start koordinaten, animation_delay, strich breite, color reset? !!
    if (p_x1 == width - 1)
        p_y1++;

    if (p_x1 < width - 1)
        p_x1++;

    if (p_y1 == height - 1)
    {
        p_x1 = 0;
        p_y1 = 0;
    }

    p_x2 = Utils::invert(p_x1, width);
    p_y2 = Utils::invert(p_y1, height);

    frame->setColor(color_black); /* color reset */
    frame->drawLine(p_x1, p_y1, p_x2, p_y2, color, 1);

    parameter[0] = (uint8_t) p_x1;
    parameter[1] = (uint8_t) p_y1;
    parameter[2] = (uint8_t) p_x2;
    parameter[3] = (uint8_t) p_y2;
}

void Animation::waterdrop()
{
    /* ani 0x06
     * paramSize: 6
     * [0]: x
     * [1]: y
     * [2]: radius
     * [3]: red
     * [4]: green
     * [5]: blue
     */
    color_t color;
    uint8_t x = parameter[0];
    uint8_t y = parameter[1];
    uint8_t radius = parameter[2];

    color.red = parameter[3] * ((width - ((radius == 0) ? 1 : radius)) / (float) (width + 1));
    color.green = parameter[4] * ((width - ((radius == 0) ? 1 : radius)) / (float) (width + 1));
    color.blue = parameter[5] * ((width - ((radius == 0) ? 1 : radius)) / (float) (width + 1));

    frame->setColor(color_black);
    frame->drawCircle(x, y, radius, color, 1);

    if (radius < sqrt(width * width + height * height))
    {
        parameter[2]++;
    } else
    {
        parameter[0] = rand() % (width - 1);
        parameter[1] = rand() % (height - 1);
        parameter[2] = 0;
    }
}

void Animation::fadingPixels()
{
    /* ani 0x07
     * paramSize: 6
     * [0]: max delay for showing up fading pixels (-> the greater the delay, the less the number of fading pixels)
     * [1]: current delay
     * [2]: current delay counter
     * [3]: red
     * [4]: green
     * [5]: blue
     */
    uint8_t x;
    uint8_t y;
    color_t color;
    color.red = parameter[3];
    color.green = parameter[4];
    color.blue = parameter[5];

    if (parameter[0] == 0)
        parameter[0] = 1;

//     create new pixel after delay
    if (parameter[2] >= parameter[1])
    {
        parameter[1] = rand() % parameter[0];
        parameter[2] = 0;

        x = rand() % width;
        y = rand() % height;
        frame->setPixel(x, y, color);
    } else
    {
        parameter[2]++;
    }

// fade existing pixel
    for (int x = 0; x < frame->getWidth(); x++)
    {
        for (int y = 0; y < frame->getHeight(); y++)
        {
            color_t tmp = frame->getPixel(x, y);
            if (tmp != color_black)
                frame->setPixel(x, y, tmp * 0.97);
        }
    }
}
