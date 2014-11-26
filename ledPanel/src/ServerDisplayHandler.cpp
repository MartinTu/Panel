/*
 * ServerDisplayHandler.cpp
 *
 *  Created on: 20.10.2013
 *      Author: martin
 */

#include "ServerDisplayHandler.h"

using namespace std;

ServerDisplayHandler::ServerDisplayHandler()
{
    Utils::ledInit();
    Utils::ledOn();
    int rt;
    struct sched_param param;

    server = new Server(UDP, 0xFFE2);
//	server = new Server(TCP, 0xaffe);
    cout << "Server done" << endl;
    panel = new Display();
    cout << "Display done" << endl;
    animation = new Animation(panel->getWidth(), panel->getHeight());
    cout << "Animation done" << endl;

    param.sched_priority = 50; 	//sched_get_priority_max(SCHED_FIFO);
    rt = pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
    if (0 != rt)
    {
        throw MyException("ServerDisplayHandler: unable to change schedule parameters: " + string(strerror(rt)));
    }
    Utils::printStars();
    cout << "* ServerDisplayHandler thread: " << endl;
    Utils::display_thread_sched_attr();
}

ServerDisplayHandler::~ServerDisplayHandler()
{
    delete server;
    delete panel;
    delete animation;
}

void ServerDisplayHandler::run()
{
    unsigned long long refTime;
    string rt;

    try
    {
        while (1)
        {
            while (1)
            {
                refTime = Utils::getTime();

                if (server->hasMessage())
                {
                    //get new command
                    getCommand(); 	//getCommand sets lastCommand
                    rt = executeCommand(lastCommand);
                    if (!rt.empty())
                    {
                        cout << "response:" << rt << endl;
                    }
                    server->putMessage(rt);
                }
                if (!server->isAlive())
                    throw MyException("Server is dead");

                if (animation->isAnimationRunning())
                {
                    if (animation->nextStep())
                    {
                        panel->getCanvas()->drawFrame(animation->getCanvas()->getPicture());
                        panel->draw();
                    }
                }
                usleep(16667 - (Utils::getTime() - refTime));
                Utils::ledOff();
            }

        }
    } catch (MyException &e)
    {
        cerr << e.what() << endl;
    }

    cout << "shutdown" << endl;
    server->shutdown();
}

void ServerDisplayHandler::getCommand()
{

    lastCommand = server->takeMessage();

    /************************************************************/
//	cout << "ServerDisplayHandler: getCommand() received " << message.size()
//			<< "bytes: [" << hex;
//	for (unsigned int i = 0; i < message.size(); i++) {
//		cout << (unsigned int) message[i] << ",";
//	}
//	cout << dec << "]" << endl;
    /************************************************************/

    return;
}

/**
 * interprets the protocol and executes it
 */
string ServerDisplayHandler::executeCommand(string &command)
{
    switch ((uint8_t) command[0])
    {
    case 0x9C:
        return executeTPM2NetProtocol(command);
    default:
        cerr << "unknown Protocol; frame: 0x" << endl;
        for (unsigned int i = 0; i < command.length(); i++)
        {
            cerr << hex << setw(3) << (int) command[i];
        }
        cerr << endl;
        return "";
    }
}

/**
 *  Pixel matrix protocol
 *  see: http://www.ledstyles.de/ftopic18969.html for more information (german)
 */
string ServerDisplayHandler::executeTPM2NetProtocol(string &command)
{

//	cout << "TPM2NET frame received" << endl;

    uint8_t blockType = command[TPM2_BLOCK_TYPE_P];
    switch (blockType)
    {
    case TPM2_BLOCK_TYPE_DATA:
//		cout << "data"<< endl;
        break;
    case TPM2_BLOCK_TYPE_CMD:
//		cout << "command"<< endl;
        break;
    case TPM2_BLOCK_TYPE_ACK:
        cout << "ack req" << endl;
        break;
    case TPM2_BLOCK_TYPE_ACK_DATA:
        cout << "ack data" << endl;
        break;
    default:
        cerr << "unknown TPM2 frame type: " << hex << blockType << endl;
        return "";  //no tpm2 ack
    }

    //test on block-end byte
    if (command[command.length() - 1] != TPM2_BLOCK_END_BYTE)
    {
        cerr << "ServerDisplayHandler: wrong TPM2 end byte" << endl;
        return "";  //no tpm2 ack
    }

    unsigned int dataSize = (((unsigned int) command[TPM2_FRAME_SIZE_HIGH_P]) << 8) | command[TPM2_FRAME_SIZE_LOW_P];

    if (dataSize != command.length() - TPM2_NET_HEADER_SIZE - TPM2_FOOTER_SIZE)
    {
        cerr << "ServerDisplayHandler: TPM2 frame is too short: " << dataSize << " " << command.length() << endl;
        return "";  //no tpm2 ack
    }

    uint8_t packetNum = command[TPM2_NET_PACKET_NUM_P];
    uint8_t numPackets = command[TPM2_NET_PACKET_TOTAL_PACK_NUM_P];

    if (packetNum > numPackets)
    {
        cout << "packetNum/numPackets: " << packetNum << "/" << numPackets << endl;
    }

//  cout << "\n\n\nBlock-Art: " << hex << (int) blockType << "\n";
//	cout << "Packetnumber: " << (int) packetNum << "\n";
//	cout << "num Packets: " << (int) numPackets << endl;

    switch (blockType)
    {
    case TPM2_BLOCK_TYPE_DATA:
    {
        //FIXME muss besser geloest werden
        animation->reset();
        uint8_t* data = (uint8_t*) (command.c_str() + TPM2_NET_DATA_P);
        panel->drawFrameModule(packetNum, dataSize, data);
        if (panel->getModulDrawn())
        {
            panel->resetModulDrawn();
            panel->draw();
        }
        return "";  //no tpm2 ack
    }
    case TPM2_BLOCK_TYPE_CMD:
    {
        uint8_t cmdCtl = command[TPM2_NET_CMD_CONTROL_P];
        uint8_t cmdType = command[TPM2_NET_CMD_TYPE_P];
        unsigned int paramSize = 0;
        if (cmdType == TPM2_CMD_SPECIAL_CMD)
        {
            uint8_t cmdSpType = command[TPM2_NET_SP_CMD_P];
            uint8_t cmdSpSubType = command[TPM2_NET_SP_CMD_P + 1];
            paramSize = dataSize - 4;
            string cmdParam(command.substr(TPM2_NET_SP_CMD_DATA_P + 1, command.length() - 2));
            return executeTpm2SpecialCmd(cmdCtl, cmdSpType, cmdSpSubType, cmdParam, paramSize);
        }
        else
        {
            string cmdParam(command.substr(TPM2_NET_CMD_DATA_P, command.length() - 1));
            paramSize = dataSize - 2;
            return executeTpm2Cmd(cmdCtl, cmdType, cmdParam, paramSize);
        }
    }

    case TPM2_BLOCK_TYPE_ACK:
    {
        break;
    }

    case TPM2_BLOCK_TYPE_ACK_DATA:
    {
        break;
    }

    default:
    {
        return "";
    }

    }
    return "";
}

string ServerDisplayHandler::executeTpm2SpecialCmd(uint8_t ctl, uint8_t cmd, uint8_t subCmd, string &param,
        unsigned int paramLen)
{
    uint8_t ackBit = (ctl >> TPM2_CMD_ACK_BIT) & 0x01;
    uint8_t response = TPM2_ACK_OK;

    switch (cmd)
    {
    case SPCMD_DRAW:
    {
        //FIXME muss besser geloest werden
        animation->reset();
        cout << "draw: ";
        switch (subCmd)
        {
        case SPCMD__COLOR:
        {
            cout << "Color";
            cout << ": param(0x";
            for (unsigned int i = 0; i < param.length(); i++)
            {
                cout << hex << setw(3) << (int) param[i];
            }
            cout << ")" << endl;

            if (paramLen == 3)
            {
                color_t color;
                color.red = param[0];
                color.green = param[1];
                color.blue = param[2];
                panel->getCanvas()->setColor(color);
                panel->draw();
            }
            else
            {
                cerr << "paramLen out of bounds(3) " << paramLen << endl;
            }
            break;
        }
        case SPCMD__SET_PIXEL:
        {
            cout << "Pixel";
            cout << ": param(0x";
            for (unsigned int i = 0; i < param.length(); i++)
            {
                cout << hex << setw(3) << (int) param[i];
            }
            cout << ")" << endl;

            if (paramLen == 5)
            {
                color_t color;
                color.red = param[2];
                color.green = param[3];
                color.blue = param[4];
                panel->getCanvas()->setPixel(param[0], param[1], color);
                panel->draw();
            }
            else
            {
                cerr << "paramLen out of bounds(5) " << paramLen << endl;
            }
            break;
        }
        case SPCMD__DRAW_LINE:
        {
            cout << "Line";
            cout << ": param(0x";
            for (unsigned int i = 0; i < param.length(); i++)
            {
                cout << hex << setw(3) << (int) param[i];
            }
            cout << ")" << endl;

            if (paramLen == 7)
            {
                color_t color;
                color.red = param[4];
                color.green = param[5];
                color.blue = param[6];
                //FIXME line width
                panel->getCanvas()->drawLine(param[0], param[1], param[2], param[3], color, 1);
                panel->draw();
            }
            else
            {
                cerr << "paramLen out of bounds(7) " << paramLen << endl;
            }
            break;
        }
        case SPCMD__DRAW_RECT:
        {
            cerr << "SPCMD__DRAW_RECT not supported" << endl;
            break;
        }
        case SPCMD__DRAW_CIRC:
        {
            cout << "Circle";
            cout << ": param(0x";
            for (unsigned int i = 0; i < param.length(); i++)
            {
                cout << hex << setw(3) << (int) param[i];
            }
            cout << ")" << endl;

            if (paramLen == 6)
            {
                color_t color;
                color.red = param[3];
                color.green = param[4];
                color.blue = param[5];
                //FIXME circle width
                panel->getCanvas()->drawCircle(param[0], param[1], param[2], color, 1);
                panel->draw();
            }
            else
            {
                cerr << "paramLen out of bounds(6) " << paramLen << endl;
            }
            break;
        }
        default:
        {
            cerr << "SpSubCommand 0x" << hex << subCmd << "not supported" << endl;
            break;
        }
        }
        break;  //spCmd

    }
    case SPCMD_ANI_SET:
    {
        cout << "set animation: ";
        animation->set(subCmd, param, paramLen);
        break;
    }
    case SPCMD_ANI_MIXER:
    {
        cout << "animation mixer" << endl;
        animation->setMixer(subCmd);
        break;
    }
    case SPCMD_ANI_DELAY:
    {
        cout << "animation delay" << endl;
        if (paramLen == 4)
        {
            unsigned int delay = (param[0] << 24) | (param[1] << 16) | (param[2] << 8) | param[3];
            animation->setAniDelay(delay);
        }
        else
        {
            cerr << "paramLen out of bounds(4) " << paramLen << endl;
        }

        break;
    }
    case SPCMD_SYSTEM_ADMIN:
    {
        cout << " system administration" << endl;
        if (paramLen == 4)
        {
            // some redundancy since its just UDP
            unsigned int syscmd = (param[0] << 24) | (param[1] << 16) | (param[2] << 8) | param[3];
            switch(syscmd)
            {
            case 0x012EB007: // 0xreboot
                system("sudo reboot");
                break;

            case 0x4A17:    // 0xhalt
                system("sudo halt");
                break;

            default:
                cerr << "SysCommand 0x" << hex << cmd << "unknown" << endl;
            }
        }
        else
        {
            cerr << "paramLen out of bounds(4) " << paramLen << endl;
        }

        break;
    }
    default:
    {
        cerr << "SpCommand 0x" << hex << (int) cmd << "not supported" << endl;
        break;
    }
    }

    if (ackBit)
    {
        return makeTpm2NetACPacket(response);
    }
    return "";
}
string ServerDisplayHandler::executeTpm2Cmd(uint8_t ctl, uint8_t cmd, string &param, unsigned int paramLen)
{
    cout << "tpm2 Commands not yet supported" << endl;
    return "";
}

string ServerDisplayHandler::makeTpm2NetACPacket(uint8_t response)
{
    string ret("");
    ret.append((char) TPM2_NET_BLOCK_START_BYTE + (char) TPM2_BLOCK_TYPE_ACK_DATA, 2);
    ret.append((char) 0x00 + (char) 0x01, 2);
    ret.append((char) 0x00 + (char) 0x01, 2);
    ret.append((char) response, 1);
    ret.append((char) TPM2_BLOCK_END_BYTE, 1);
    return ret;
}

string ServerDisplayHandler::makeTpm2NetADPacket(uint8_t response, string &data, unsigned int dataLen)
{
    string ret("");
    ret.append((char) TPM2_NET_BLOCK_START_BYTE + (char) TPM2_BLOCK_TYPE_ACK_DATA, 2);
    ret.append((char) ((dataLen >> 3) & 0xff) + (char) ((dataLen) & 0xff), 2);
    ret.append((char) 0x00 + (char) 0x01, 2);
    ret.append((char) response, 1);
    ret += data;
    ret.append((char) TPM2_BLOCK_END_BYTE, 1);
    return ret;
}
