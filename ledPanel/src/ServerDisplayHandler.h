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

using namespace std;

class ServerDisplayHandler
{
public:
	ServerDisplayHandler();
	virtual ~ServerDisplayHandler();

	void run();

protected:
	string makeHeader(string message);
	string getCommand();

	string executeCommand(string command);
	string executeMyProtocol(string command);
	string executeTPM2NetProtocol(string command);
	string checkMyPMessageSize(string command);

	string drawColor(string command);
	string pulseColor(string command);
	string fadeColor(string command);
	string drawPicture(string command);
	string drawLine(string command);
	string drawCircle(string command);
	string bootScreen(string command);

	string doAnimation(string command);

	string rotateLine(string command);
	string waterdrop(string command);
	string verticalFade(string command);

	Server* server;
	Display* panel;

	string lastCommand;

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
