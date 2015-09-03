/*
 * ServerDisplayHandler.h
 *
 *  Created on: 20.10.2013
 *      Author: martin
 */

#ifndef SERVERDISPLAYHANDLER_H_
#define SERVERDISPLAYHANDLER_H_


#include <sstream>
#include <string>
#include "Server.h"
#include "Display.h"
#include "Color.h"
#include "Utils.h"
#include "tpm2.h"
#include "Animation.h"
#include "Painting.h"

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
        uint8_t checkTPM2Frame(string &command, unsigned int* dataSize);
        string executeTPM2Data(string &command, uint8_t packetNum, unsigned int dataSize);
        string executeTPM2Command(string &command, uint8_t packetNum, uint8_t dataSize);

        void systemAdministration(uint8_t cmd, unsigned int paramLen, string &param);
		void manipulateColorPlate(uint8_t cmd, unsigned int paramLen, string &param);


//different layers
        Animation* animation;
        Painting* painting;
        //Painting* externalStream;

        Server* server;
        Display* panel;

        string lastCommand;
};
#endif /* SERVERDISPLAYHANDLER_H_ */
