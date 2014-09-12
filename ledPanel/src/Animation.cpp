/*
 * Animation.cpp
 *
 *  Created on: 06.09.2014
 *      Author: keKs
 */

#include "Animation.h"

Animation::Animation(int _width, int _height) :
        width(_width), height(_height), aniDelay(0), aniDelayIterator(0), runningAni(aniBootScreen), lastAni(aniNone), mixer(
                mixMaybe)
{
    //set up bootScreen parameter
    parameter[0] = 0x00;
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
        this->runningAni = static_cast<animation_t>(ani);
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
        break;
    case aniBootScreen:
        cout << "aniBootScreen";
        if (paramSize == (unsigned int) lenBootScreen)
        {
            parameter = param.substr(0, paramSize);
            aniDelay = 0;
            ret = 0;
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
    default:
        break;
    }
    if (ret == 0)
    {
        cout << ": param(0x";
        for (unsigned int i = 0; i < parameter.length(); i++)
        {
            cout << hex << setw(3) << (int) parameter[i];
        }
        cout << ")" << endl;
        return 0;
    }
    runningAni = lastAni;
    parameter.clear();
    return -1;  //error at paramSize
}

void Animation::reset()
{
    lastAni = runningAni;
    runningAni = aniNone;
    parameter.clear();
}

bool Animation::nextStep()
{
    if ((mixer != mixOff) && (aniDelayIterator-- == 0))
    {
//		cout << "aniParam: 0x";
//		for(unsigned int i = 0; i < parameter.length(); i++){
//			cout << hex <<setw(3)<< (int) parameter[i];
//		}
//		cout << endl;
        aniDelayIterator = aniDelay;
        switch (runningAni)
        {
        case aniBootScreen:
            bootScreen();
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

Canvas* Animation::getCanvas()
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

void Animation::bootScreen()
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
                    if (invader1_10x20[y][x * 3])
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
                    if (invader2_10x20[y][nx * 3])
                    {
                        pictureWheel += 128;
                    }
                }
            } else if (invaderOK == 144)
            {  //martin
                if (invader[y][x * 3])
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
     * paramSize: 5
     * [0]: diff
     * [1]: direction //TODO: implement
     * [2]: red
     * [3]: green
     * [4]: blue
     */
    color_t color;
    uint8_t diff = parameter[0];
    color.red = parameter[2];
    color.green = parameter[3];
    color.blue = parameter[4];

    for (uint8_t x = 0; x < width; x++)
    {
        for (uint8_t y = 0; y < height; y++)
        {
            frame->setPixel(x, y, color);
        }
        if (color.blue < diff)
        {
            if (color.green <= (255 - diff))
                color.green += diff;

            if (color.red >= diff)
                color.red -= diff;
            else
                color.blue += diff;
        }

        if (color.red < diff)
        {
            if (color.blue <= (255 - diff))
                color.blue += diff;

            if (color.green >= diff)
                color.green -= diff;
            else
                color.red += diff;
        }

        if (color.green < diff)
        {
            if (color.red <= (255 - diff))
                color.red += diff;

            if (color.blue >= diff)
                color.blue -= diff;
            else
                color.green += diff;
        }

        /* set new start color */
        if (x == 1)
        {
            parameter[2] = color.red;
            parameter[3] = color.green;
            parameter[4] = color.blue;
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
    float maxBrightness = (uint8_t) parameter[0] / 255.0;
    uint8_t fadeStatus = (uint8_t) parameter[1];
    uint8_t red = (uint8_t) parameter[2];
    uint8_t green = (uint8_t) parameter[3];
    uint8_t blue = (uint8_t) parameter[4];
    color_t color = {(uint8_t) (red * maxBrightness), (uint8_t) (green * maxBrightness),
            (uint8_t) (blue * maxBrightness)};

    frame->setColor(color);

    if (0x0 == fadeStatus)
    {
        red--;
        green++;

        if (0 == red)
            fadeStatus++;
    } else if (0x01 == fadeStatus)
    {
        green--;
        blue++;

        if (0 == green)
            fadeStatus++;
    } else if (0x02 == fadeStatus)
    {
        blue--;
        red++;

        if (0 == blue)
            fadeStatus = 0;
    }
    parameter[0] = maxBrightness * 255;
    parameter[1] = fadeStatus;
    parameter[2] = red;
    parameter[3] = green;
    parameter[4] = blue;
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

    p_x2 = Utile::invert(p_x1, width);
    p_y2 = Utile::invert(p_y1, height);

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
