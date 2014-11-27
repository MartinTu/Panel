/*
 * Animation.cpp
 *
 *  Created on: 06.09.2014
 *      Author: keKs
 */

#include "Animation.h"

Animation::Animation(int _width, int _height) :
        width(_width), height(_height), aniSkipIterator(0), runningAni(aniNone), mixer(mixMaybe)
{
    animation_t startingAni = aniFadingPixels;
    parameter.clear();
    parameter.push_back(0x00);  //skipframe
    parameter.push_back(0x00);
    parameter.push_back(200);
    parameter.push_back(0x2);
    parameter.push_back(0x00);
    parameter.push_back(0xff);
    parameter.push_back(0x00);

    //set running ani
    this->set(startingAni, parameter, parameter.length());

    this->frame = new Canvas(width, height);
}
Animation::~Animation()
{
    delete frame;
}

bool Animation::isFrameNotSkipped(unsigned int skip)
{
    if (aniSkipIterator == 0)
    {
        //update with new skipvalue depending on animation
        aniSkipIterator = skip;
        return true;
    }
    aniSkipIterator--;
    return false;
}

int Animation::set(uint8_t ani, string &param, unsigned int paramSize)
{
    int ret = -1;
    //dont skip next frame
    aniSkipIterator = 0;
    //backup last animationID
    animation_t lastAni = runningAni;
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
        reset();
        break;
    case aniInvader:
        cout << "[INFO] aniInvader";
        if (paramSize == (unsigned int) lenInvader)
        {
            parameter = param.substr(0, paramSize);
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
        cout << "[INFO] aniDirectionFade";
        if (paramSize == (unsigned int) lenDirectionFade)
        {
            parameter = param.substr(0, paramSize);
            //frameSkip = 3;
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
        cout << "[INFO] aniScreenFade";
        if (paramSize == (unsigned int) lenScreenFade)
        {
            parameter = param.substr(0, paramSize);
            ret = 0;
            if (runningAni != lastAni)
            {
                //initialize intern parameter
                internPar.clear();
                internPar.push_back(0x00);  //fadeStatus
                internPar.push_back(0xff);  //red
                internPar.push_back(0x00);  //green
                internPar.push_back(0x00);  //blue
            }
        } else
        {
            cerr << " param out of bounds: " << paramSize << endl;
        }
        break;
    case aniScreenPulse:
        cout << "[INFO] aniScreenPulse";
        if (paramSize == (unsigned int) lenScreenPulse)
        {
            parameter = param.substr(0, paramSize);
            ret = 0;
            if (runningAni != lastAni)
            {
                //initialize intern parameter
                internPar.clear();
                internPar.push_back(0xff);  //brightness = 255
                internPar.push_back(false);  //getsBrighter = false
            }
        } else
        {
            cerr << " param out of bounds: " << paramSize << endl;
        }
        break;
    case aniRotateLine:
        cout << "[INFO] aniRotateLine";
        if (paramSize == (unsigned int) lenRotateLine)
        {
            parameter = param.substr(0, paramSize);
            //frameSkip = 2;
            ret = 0;
            if (runningAni != lastAni)
            {
                //initialize intern parameter
                internPar.clear();
                internPar.push_back(0x00);  //p_x1
                internPar.push_back(0x00);  //p_y1
            }
        } else
        {
            cerr << " param out of bounds: " << paramSize << endl;
        }
        break;
    case aniWaterdrop:
        cout << "[INFO] aniWaterdrop";
        if (paramSize == (unsigned int) lenWaterdrop)
        {
            parameter = param.substr(0, paramSize);
            //frameSkip = 2;
            ret = 0;
            if (runningAni != lastAni)
            {
                //initialize intern parameter
                internPar.clear();
                internPar.push_back((uint8_t) rand() % (width - 1));  //x
                internPar.push_back((uint8_t) rand() % (height - 1));  //y
                internPar.push_back(0x00);  //radius
            }
        } else
        {
            cerr << " param out of bounds: " << paramSize << endl;
        }
        break;
    case aniFadingPixels:
        cout << "[INFO] aniFadingPixels";
        if (paramSize == (unsigned int) lenFadingPixels)
        {
            parameter = param.substr(0, paramSize);
            //frameskip = 1;
            ret = 0;
            if (runningAni != lastAni)
            {
                //initialize intern parameter
                internPar.clear();
                internPar.push_back(0x00);  //targetPixOldHigh
                internPar.push_back(0x00);  //targetPixOldLow
            }
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
    //reset to animation from before
    runningAni = lastAni;
    cerr << "animation set() error" << endl;
    return -1;  //error at paramSize
}

void Animation::reset()
{
    runningAni = aniNone;
    aniSkipIterator = 0;
    parameter.clear();
    internPar.clear();
}

bool Animation::nextStep()
{
    if (mixer != mixOff)
    {
//        cout << "aniParam: 0x";
//        for (unsigned int i = 0; i < parameter.length(); i++)
//        {
//            cout << hex << setw(3) << (int) parameter[i];
//        }
//        cout << endl;
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
     * paramSize : 3
     * [0]: skipFrame
     * [1]: colorDimmer
     * [2]: saturation
     * internPar: 1
     * [0]: actWheel//0x00
     */
    if (isFrameNotSkipped(parameter[0]))
    {
        color_t color;
        float colorDimmer = (float) (parameter[1] / 255.0);
        uint8_t saturation = (uint8_t) parameter[2];
        int invaderOK = width * height;
        bool picture;
        uint8_t actWheel = (uint8_t) internPar[0];
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
                        //int nx = x - 1;
                        int nx = x;
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
                color *= colorDimmer;
                color &= saturation;
                frame->setPixel(x, y, color * (float) (parameter[1] / 255.0));
            }
        }
        internPar[0]++;
    }

}

void Animation::directionFade()
{
    /* ani 0x02
     * paramSize: 5
     * [0]: skipFrame
     * [1]: delta
     * [2]: direction //original color comes from reversed direction
     * [3]: colorDimmer
     * [4]: saturation
     * internPar: 3
     * [0]: red//0xff
     * [1]: green//0x00
     * [2]: blue//0x00
     */
    if (isFrameNotSkipped(parameter[0]))
    {
        color_t color((uint8_t) internPar[0], (uint8_t) internPar[1], (uint8_t) internPar[2]);
        uint8_t delta = (uint8_t) parameter[1];
        direction_t dir = static_cast<direction_t>(parameter[2]);
        float colorDimmer = parameter[3] / 255.0;
        uint8_t saturation = (uint8_t) parameter[4];
        color_t calcColor(color);
        calcColor *= colorDimmer;
        calcColor &= saturation;

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
            calcColor = color;
            calcColor *= colorDimmer;
            calcColor &= saturation;

            /* set new start color */
            if (x == 1)
            {
                internPar[0] = color.red;
                internPar[1] = color.green;
                internPar[2] = color.blue;
            }
        }
    }
}

void Animation::screenFade()
{
    /* ani 0x03
     * paramSize: 4
     * [0]: skipFrame
     * [1]: delta
     * [2]: colorDimmer
     * [3]: saturation
     * internPar: 4
     * [0]: fadeStatus//0x00
     * [1]: red//0xff
     * [2]: green//0x00
     * [3]: blue//0x00
     */
    if (isFrameNotSkipped(parameter[0]))
    {
        int delta = parameter[1];
        float maxBrightness = parameter[2] / 255.0;
        uint8_t fadeStatus = (uint8_t) internPar[0];
        int saturation = parameter[3];
        color_t color(internPar[1], internPar[2], internPar[3]);
        color_t screenColor(color);
        screenColor *= maxBrightness;
        screenColor &= saturation;
        frame->setColor(screenColor);
        if (!delta)
        {
            delta = 1;
        }
        if (0x0 == fadeStatus)
        {
            color.red = Utils::addInBounds(color.red, -delta);
            color.green = Utils::addInBounds(color.green, delta);
            if (0 == color.red)
                fadeStatus++;
        } else if (0x01 == fadeStatus)
        {
            color.green = Utils::addInBounds(color.green, -delta);
            color.blue = Utils::addInBounds(color.blue, delta);
            if (0 == color.green)
                fadeStatus++;
        } else if (0x02 == fadeStatus)
        {
            color.blue = Utils::addInBounds(color.blue, -delta);
            color.red = Utils::addInBounds(color.red, delta);
            if (0 == color.blue)
                fadeStatus = 0;
        }
        internPar[0] = fadeStatus;
        internPar[1] = color.red;
        internPar[2] = color.green;
        internPar[3] = color.blue;
    }
}

void Animation::screenPulse()
{
    /* ani 0x04
     * paramSize: 5
     * [0]: skipFrame
     * [1]: delta//should be 1
     * [2]: red
     * [3]: green
     * [4]: blue
     * internPar: 2
     * [0]: brightness//0xff
     * [1]: getsBrighter//false
     */
    if (isFrameNotSkipped(parameter[0]))
    {
        float val = (internPar[0] * internPar[0]) / (255.0 * 255.0);
        color_t color((uint8_t) parameter[2], (uint8_t) parameter[3], (uint8_t) parameter[4]);
        int delta = parameter[1];
        if (!delta)
        {
            delta = 1;
        }
        color *= val;

        if (internPar[1])
        {
            internPar[0] = Utils::addInBounds(internPar[0], delta);
            if (0xff == internPar[0])
                internPar[1] = false;
        } else
        {
            internPar[0] = Utils::addInBounds(internPar[0], -delta);
            if (0x00 == internPar[0])
                internPar[1] = true;
        }
        frame->setColor(color);
    }
}

void Animation::rotateLine()
{
    /* ani 0x05
     * paramSize: 11
     * [ 0]: frameSkip
     * [ 1]: antiAliasing//TODO implement
     * [ 2]: width
     * [ 3]: rotate_x//TODO implement
     * [ 4]: rotate_y//TODO implement
     * [ 5]: rotorRed
     * [ 6]: rotorGreen
     * [ 7]: rotorBlue
     * [ 8]: backgroundRed
     * [ 9]: backgroundGreen
     * [10]: backgroundBlue
     * internPar: 2
     * [0]: p_x1//0x00
     * [1]: p_y1//0x00
     */
    if (isFrameNotSkipped(parameter[0]))
    {
        color_t color((uint8_t) parameter[5], (uint8_t) parameter[6], (uint8_t) parameter[7]);
        color_t background((uint8_t) parameter[8], (uint8_t) parameter[9], (uint8_t) parameter[10]);
        int p_x1 = internPar[0];
        int p_y1 = internPar[1];
        int lineWidth = parameter[2];

        if (p_x1 == width - 1)
            p_y1++;

        if (p_x1 < width - 1)
            p_x1++;

        if (p_y1 == height - 1)
        {
            p_x1 = 0;
            p_y1 = 0;
        }

        int p_x2 = Utils::invert(p_x1, width);
        int p_y2 = Utils::invert(p_y1, height);

        frame->setColor(background);
        frame->drawLine(p_x1, p_y1, p_x2, p_y2, color, lineWidth);

        internPar[0] = (uint8_t) p_x1;
        internPar[1] = (uint8_t) p_y1;
    }
}

void Animation::waterdrop()
{
    /* ani 0x06
     * paramSize: 9
     * [0]: frameSkip
     * [1]: antiAliasing//TODO implement
     * [2]: width
     * [3]: dropRed
     * [4]: dropGreen
     * [5]: dropBlue
     * [6]: backgroundRed
     * [7]: backgroundGreen
     * [8]: backgroundBlue
     * internPar: 3
     * [0]: x//rand % width
     * [1]: y//rand % height
     * [2]: radius//0x00
     */
    if (isFrameNotSkipped(parameter[0]))
    {
        color_t color((uint8_t) parameter[3], (uint8_t) parameter[4], (uint8_t) parameter[5]);
        color_t background((uint8_t) parameter[6], (uint8_t) parameter[7], (uint8_t) parameter[8]);
        int lineWidth = parameter[2];
        uint8_t x = internPar[0];
        uint8_t y = internPar[1];
        uint8_t radius = internPar[2];

        color *= ((width - ((radius == 0) ? 1 : radius)) / (float) (width + 1));

        frame->setColor(background);
        frame->drawCircle(x, y, radius, color, lineWidth);

        if (radius < sqrt(width * width + height * height))
        {
            //next time radius bigger
            internPar[2]++;
        } else
        {
            internPar[0] = rand() % (width - 1);
            internPar[1] = rand() % (height - 1);
            internPar[2] = 0;
        }
    }
}

void Animation::fadingPixels()
{
    /* ani 0x07
     * paramSize: 7
     * [0]: frameSkip
     * [1]: targetPixHigh
     * [2]: targetPixLow
     * [3]: fadeSpeed
     * [4]: pixRed
     * [5]: pixGreen
     * [6]: pixBlue
     * internPar: 2
     * [0]: targetPixOldHigh//0x00
     * [1]: targetPixOldLow//0x00
     */
    if (isFrameNotSkipped(parameter[0]))
    {
        color_t color((uint8_t) parameter[4], (uint8_t) parameter[5], (uint8_t) parameter[6]);
        if (color == color_black)
            color = color_green;
        int targetPix = (parameter[1] << 8) | parameter[2];
        int targetPixOld = (internPar[0] << 8) | internPar[1];
        //set fadeSpeed at least 1
        int fadeSpeed = parameter[3];
        if (fadeSpeed == 0x00)
        {
            fadeSpeed = 0x01;
        }
        float fadeFactor = (255 - fadeSpeed) / 255.0;
        int maximumPixel = width * height;
        if (targetPix > maximumPixel)
        {
            //trim targetPixel to maximumPixel
            targetPix = maximumPixel;
        }
        if (targetPix != targetPixOld)
        {
            //initialize screen with targetPix Pixels
            frame->setColor(color_black);
            for (int i = 0; i < targetPix; i++)
            {
                int x;
                int y;
                bool noPixel = true;
                color_t newPixelColor(color);
                if (color.red)
                    newPixelColor.red = (uint8_t) (rand() % color.red);
                if (color.green)
                    newPixelColor.green = (uint8_t) (rand() % color.green);
                if (color.blue)
                    newPixelColor.blue = (uint8_t) (rand() % color.blue);
                while (noPixel)
                {
                    x = rand() % width;
                    y = rand() % height;
                    if (frame->getPixel(x, y) == color_black)
                    {
                        noPixel = false;
                        frame->setPixel(x, y, newPixelColor);
                    }
                }
            }
            internPar[0] = (targetPix >> 8) & 0xff;
            internPar[1] = targetPix & 0xff;
        }
        //fade existing pixel
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < width; y++)
            {
                color_t tmp(frame->getPixel(x, y));
                if (tmp != color_black)
                    frame->setPixel(x, y, tmp * fadeFactor);
            }
        }
        //get actual amount of pixels
        int actualPix = 0;
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                if (frame->getPixel(x, y) != color_black)
                    actualPix++;
            }
        }
        //create targetPixel-actualPix Pixels
        int pix = targetPix - actualPix;
        for (int i = 0; i < pix; i++)
        {
            int x;
            int y;
            bool noPixel = true;
            while (noPixel)
            {
                x = rand() % width;
                y = rand() % height;
                if (frame->getPixel(x, y) == color_black)
                {
                    noPixel = false;
                    frame->setPixel(x, y, color);
                }
            }
        }
    }
}
