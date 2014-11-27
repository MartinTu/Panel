/*
 * ServerDisplayHandler.h
 *
 *  Created on: 20.10.2013
 *      Author: martin
 */

#ifndef SERVERDISPLAYHANDLER_H_
#define SERVERDISPLAYHANDLER_H_

#define SPCMD_DRAW 			0x01
#define SPCMD__COLOR   		0x01
#define SPCMD__SET_PIXEL	0x02
#define SPCMD__DRAW_LINE	0x03
#define SPCMD__DRAW_RECT	0x04
#define SPCMD__DRAW_CIRC	0x05

#define SPCMD_ANI_SET		0x03
//      SPCMD__ANI_SET_ANIMATION
#define SPCMD_ANI_MIXER		0x04
#define SPCMD_ANI_DELAY		0x05
#define SPCMD_SYSTEM_ADMIN  0xFE

#include <sstream>
#include <string>
#include "Server.h"
#include "Display.h"
#include "colorManipulation.h"
#include "Utils.h"
#include "tpm2.h"
#include "Animation.h"

using namespace std;

class ServerDisplayHandler
{
    public:
        ServerDisplayHandler();
        virtual ~ServerDisplayHandler();

        void run();

    protected:
        void getCommand();

        string executeCommand(string &command);

        string executeTPM2NetProtocol(string &command);
        string executeTpm2SpecialCmd(uint8_t ctl, uint8_t cmd, uint8_t subCmd, string &param, unsigned int paramLen);
        string executeTpm2Cmd(uint8_t ctl, uint8_t cmd, string &param, unsigned int paramLen);
        string makeTpm2NetACPacket(uint8_t response);
        string makeTpm2NetADPacket(uint8_t response, string &data, unsigned int dataLen);

        Animation* animation;
        Server* server;
        Display* panel;

        string lastCommand;

};
#endif /* SERVERDISPLAYHANDLER_H_ */
