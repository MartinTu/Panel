/*
 * ServerDisplayHandler.h
 *
 *  Created on: 20.10.2013
 *      Author: martin
 */

#ifndef SERVERDISPLAYHANDLER_H_
#define SERVERDISPLAYHANDLER_H_

#include "Server.h"
#include "Display.h"
#include "colorManipulation.h"
#include <sstream>
#include "Utile.h"



    class ServerDisplayHandler
    {
        public:
            ServerDisplayHandler();
            virtual ~ServerDisplayHandler();

            void run();


        protected:
            std::string makeHeader(std::string message);
            std::string getCommand();

            std::string executeCommand(std::string command);
            std::string executeMyProtocol(std::string command);
            std::string executeTPM2Protocol(std::string command);
            std::string checkMyPMessageSize(std::string command);

            std::string drawColor(std::string command);
            std::string pulseColor(std::string command);
            std::string fadeColor(std::string command);
            std::string drawPicture(std::string command);
            std::string drawLine(std::string command);
            std::string drawCircle(std::string command);
            std::string bootScreen(std::string command);

            std::string doAnimation(std::string command);

            std::string rotateLine(std::string command);
            std::string waterdrop(std::string command);
            std::string verticalFade(std::string command);

            Server* server;
            Display* panel;

            std::string lastCommand;

            int32_t brightness;
            bool pulse_getBrighter;
            color_t fade_color;
            uint8_t fade_status;

            uint8_t animation_x1;
            uint8_t animation_x2;
            uint8_t animation_y1;
            uint8_t animation_y2;

            unsigned int animation_delay;

    };
#endif /* SERVERDISPLAYHANDLER_H_ */
