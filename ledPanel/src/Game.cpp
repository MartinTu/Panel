/*
 * Game.cpp
 *
 *  Created on: 22.02.2015
 *      Author: keKs
 */


#include "Game.h"

using namespace std;


Game::Game(int _width, int _height) :
        Layer(_width, _height), gameSkipIterator(0), runningGame(gameNone)
{
    game_t startingGame = gameSnake;
    parameter.clear();
    parameter.push_back(0);
    //set starting game
    this->set(startingGame, parameter, parameter.length());
}

Game::~Game()
{
}


bool Game::isFrameNotSkipped(unsigned int skip)
{
    if (gameSkipIterator == 0)
    {
        //update with new skipvalue depending on game
        gameSkipIterator = skip;
        return true;
    }
    gameSkipIterator--;
    return false;
}

int Game::set(uint8_t id, string &param, unsigned int paramSize)
{
    int ret = -1;
    //backup last gameID
    game_t lastGame = runningGame;
    if (id < _gameNUM)
    {
        runningGame = static_cast<game_t>(id);
    } else
    {
        cerr << id << " is no game_t" << endl;
        runningGame = gameNone;
    }

    if (runningGame != lastGame)
    {
        //dont skip next frame
        gameSkipIterator = 0;
    }

    switch (runningGame)
    {
    case aniNone:
        cout << "gameNone";
        reset();
        break;
    case gameSnake:
        cout << "[INFO] gameSnake";
        if (paramSize == (unsigned int) lenSnake)
        {
            parameter = param.substr(0, paramSize);
            ret = 0;
            if (runningGame != lastGame)
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
    //reset to game from before
    runningGame = lastGame;
    cerr << "game set() error" << endl;
    return -1;  //error at paramSize
}

void Game::reset()
{
    runningGame = gameNone;
    gameSkipIterator = 0;
    parameter.clear();
    internPar.clear();
}

bool Game::nextStep()
{
    if (mixer != mixOff)
    {
//        cout << "aniParam: 0x";
//        for (unsigned int i = 0; i < parameter.length(); i++)
//        {
//            cout << hex << setw(3) << (int) parameter[i];
//        }
//        cout << endl;
        switch (runningGame)
        {
        case gameSnake:
            snake();
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


void Game::snake()
{
    /* game 0x01
     * paramSize : 3
     * [0]: skipFrame
     * [1]: direction
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
                color.wheel(pictureWheel);
                color.setDimmer(colorDimmer);
                color.setSaturation(saturation);
                frame->setPixel(x, y, color * (float) (parameter[1] / 255.0),1);
            }
        }
        internPar[0]++;
    }

}
