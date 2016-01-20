/*
 * Game.h
 *
 *  Created on: 22.02.2015
 *      Author: keKs
 */

#ifndef GAME_H_
#define GAME_H_

#include <string>
#include <vector>
#include "Display.h"
#include "Color.h"
#include "Utils.h"
#include "Bitmaps.h"
#include "Layer.h"

using namespace std;

enum game_t
{
    gameNone = 0x00,
    gameSnake = 0x01,
    _gameNUM  //numbers of ani (if enum values are not doubled or spaced)
};

class Game : public Layer
{
    public:
        Game(int _width, int _height);
        virtual ~Game();

        int set(uint8_t id, string &param, unsigned int paramSize);
        void reset();

        bool nextStep();
        bool isGameRunning();

    private:
        void snake();
		
		static const unsigned char lenParamNone   =0x00;
		static const unsigned char lenParamSnake  =0x01;

    protected:
        bool isFrameNotSkipped(unsigned int skip);

        unsigned int gameSkipIterator;
        game_t runningGame;

        /* parameter
         * parameters are set from client
         * during animation these should not be altered
         */
        string parameter;
        /* internPar
         * intern parameters are initialized in set(.) and can change during animation
         * stores the intern state of ananimation
         */
        string internPar;
};

#endif /* GAME_H_ */
