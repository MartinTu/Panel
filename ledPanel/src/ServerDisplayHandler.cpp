/*
 * ServerDisplayHandler.cpp
 *
 *  Created on: 20.10.2013
 *      Author: martin
 */

#include "ServerDisplayHandler.h"

#define COMMAND_DO_NOTHING      0x00
#define COMMAND_DRAW_COLOR      0x01
#define COMMAND_FADE_COLOR      0x02
#define COMMAND_PULSE_COLOR     0x03
#define COMMAND_DRAW_PICTURE    0x04
#define COMMAND_DRAW_PIXEL      0x05
#define COMMAND_DRAW_LINE       0x06
#define COMMAND_DRAW_RECT       0x07
#define COMMAND_DRAW_CIRCLE     0x08

#define COMMAND_DO_ANIMATION    0xa0
#define ANIMATION_MOVE_VLINE    0x01
#define ANIMATION_MOVE_HLINE    0x02
#define ANIMATION_ROTATE_LINE   0x03

using namespace std;

const uint16_t pwmtable_8D[32] = { 0, 1, 2, 2, 2, 3, 3, 4, 5, 6, 7, 8, 10, 11,
		13, 16, 19, 23, 27, 32, 38, 45, 54, 64, 76, 91, 108, 128, 152, 181, 215,
		255 };

ServerDisplayHandler::ServerDisplayHandler() {
	int rt;
	struct sched_param param;

//	server = new Server(UDP,0x1936);
	server = new Server(TCP, 0xaffe);
	panel = new Display(16, 9, WS2801);

	brightness = 0x01;
	pulse_getBrighter = false;

	fade_color.red = 0xff;
	fade_color.green = 0;
	fade_color.blue = 0;
	fade_status = 0x0;

	animation_x1 = 0;
	animation_x2 = 0;
	animation_y1 = 0;
	animation_y2 = 0;
	animation_delay = 2;

	param.__sched_priority = 50; 	//sched_get_priority_max(SCHED_FIFO);
	rt = pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
	if (0 != rt) {
		throw MyException(
				"ServerDisplayHandler: unable to change schedule parameters: "
						+ string(strerror(rt)));
	}

	cout << "ServerDisplayHandler thread: ";
	Utile::display_thread_sched_attr();
}

ServerDisplayHandler::~ServerDisplayHandler() {
	delete server;
	delete panel;
}

/**
 * adds a header to the given message in the format of:
 *   _____________________________________________________
 *  |myp-ID | num_bytes | message_message_message_message |
 *  |_0x42__|0_|1_|2_|3_|______x bytes____________________|
 *
 *  where num_bytes is 4-byte binary and represents the length of the message (excluding num_byte)!
 *
 */
string ServerDisplayHandler::makeHeader(string message) {
	string header = "xxxx";

	message = header + message;

	message[0] = 0x42;
	message[1] = ((message.size() - 4) >> 24) & 0xff;
	message[2] = ((message.size() - 4) >> 16) & 0xff;
	message[3] = ((message.size() - 4) >> 8) & 0xff;
	message[4] = (message.size() - 4) & 0xff;

//    cout << "header: " << (int) message[0] << (int) message[1] << (int) message[2] << (int) message[3] << endl;

	return message;
}

/**
 * returns a command out of various server message in the format of:
 *   ___________________________________________
 *  | num_bytes | message_message_message_message |
 *  |0_|1_|2_|3_|______x bytes____________________|
 *
 *  where num_bytes is 4-byte binary and represents the length of the message (excluding num_byte)!
 *
 */
string ServerDisplayHandler::getCommand() {
	string message = "";

	message = server->takeMessage();

	/************************************************************ /
	cout << "ServerDisplayHandler: getCommand() received " << message.size()
			<< "bytes: [" << hex;
	for (unsigned int i = 0; i < message.size(); i++) {
		cout << (unsigned int) message[i] << ",";
	}
	cout << dec << "]" << endl;
	/************************************************************/

	return message;
}

/**
 * parses the given command string and sets the specified color to all pixels
 */
string ServerDisplayHandler::drawColor(string command) {
	if (command.size() == 4) {
		color_t color;
		color.red = command[1];
		color.green = command[2];
		color.blue = command[3];

		panel->setColor(color);
		panel->draw();

		lastCommand[0] = 0x0; // stop doing anything
	} else {
		stringstream st;
		st << "DRAW_COLOR FAILED: wrong command size: received "
				<< command.size() << " but expected 4";
		return st.str();
	}
	return "OK";
}

/**
 * parses the given command string and pulses in the specified color
 */
string ServerDisplayHandler::pulseColor(string command) {
	color_t color;

//        if (animation_delay-- == 0)
//        {
//            animation_delay = 1;

	if (command.size() == 4) {

		float val = (brightness * brightness) / (255.0 * 255.0);
//            float val = brightness / 255.0;
//            float val = pwmtable_8D[brightness] / 255.0;

		color.red = command[1] * val;
		color.green = command[2] * val;
		color.blue = command[3] * val;

		if (pulse_getBrighter) {

//                if (31 == ++brightness)
			if (0xff == ++brightness)
				pulse_getBrighter = false;
		} else {
			if (0 == --brightness)
				pulse_getBrighter = true;
		}

		panel->setColor(color);
		panel->draw();
	} else {
		stringstream st;
		st << "PULSE_COLOR FAILED: wrong command size: received "
				<< command.size() << " but expected 4";
		return st.str();
	}
//    }
	return "OK";
}

/**
 * parses the given command string and fades through all colors
 */
string ServerDisplayHandler::fadeColor(string command) {
	if (command.size() == 2) {
		float maxBrightness = command[1] / 255.0;
		color_t color = { (uint8_t) (fade_color.red * maxBrightness),
				(uint8_t) (fade_color.green * maxBrightness),
				(uint8_t) (fade_color.blue * maxBrightness) };

		panel->setColor(color);
		panel->draw();

		if (0x0 == fade_status) {
			fade_color.red--;
			fade_color.green++;

			if (0 == fade_color.red)
				fade_status++;
		} else if (0x01 == fade_status) {
			fade_color.green--;
			fade_color.blue++;

			if (0 == fade_color.green)
				fade_status++;
		} else if (0x02 == fade_status) {
			fade_color.blue--;
			fade_color.red++;

			if (0 == fade_color.blue)
				fade_status = 0;
		}
	} else {
		stringstream st;
		st << "FADE_COLOR FAILED: wrong command size: received "
				<< command.size() << " but expected 2";
		return st.str();
	}

	return "OK";
}

string ServerDisplayHandler::drawPicture(string command) {
	if (command.size() == 433) {
		panel->draw(command.size() - 1,
				reinterpret_cast<const uint8_t*>(&command[1]));
	} else {
		stringstream st;
		st << "DRAW_PICTURE FAILED: wrong command size: received "
				<< command.size() << " but expected 433";
		return st.str();
	}
	return "OK";
}

string ServerDisplayHandler::drawLine(string command) {
	color_t color;
	uint8_t reset;
	uint8_t x1;
	uint8_t x2;
	uint8_t y1;
	uint8_t y2;

	if (command.size() == 9) {
		x1 = command[1];
		y1 = command[2];
		x2 = command[3];
		y2 = command[4];
		color.red = command[5];
		color.green = command[6];
		color.blue = command[7];
		reset = command[8];

		if (reset)
			this->panel->setColor(color_black);

		this->panel->drawLine(x1, y1, x2, y2, color, 1);
		this->panel->draw();

		lastCommand[0] = 0;
	} else {
		stringstream st;
		st << "DRAW_LINE FAILED: wrong command size: received "
				<< command.size() << " but expected 9";
		cerr << st.str() << endl;
		return st.str();
	}
	return "OK";
}

/**
 * draws a circle
 */
string ServerDisplayHandler::drawCircle(std::string command) {
	uint8_t x0;
	uint8_t y0;
	uint8_t radius;
	color_t color;
	uint8_t reset;

	if (command.size() == 8) {
		x0 = command[1];
		y0 = command[2];
		radius = command[3];
		color.red = command[4];
		color.green = command[5];
		color.blue = command[6];
		reset = command[7];

		if (reset)
			this->panel->setColor(color_black);

		this->panel->drawCircle(x0, y0, radius, color, 1);
		this->panel->draw();

		lastCommand[0] = 0;
	} else {
		stringstream st;
		st << "DRAW_CIRCLE FAILED: wrong command size: received "
				<< command.size() << " but expected 8";
		cerr << st.str() << endl;
		return st.str();
	}
	return "OK";
}

string ServerDisplayHandler::rotateLine(std::string command) {
	color_t color;

	if (animation_delay-- == 0) {
		animation_delay = 2;

		if (command.size() != 5) {
			lastCommand[0] = 0x0;
			stringstream st;
			st << "ROTATE_LINE: wrong command size: received " << command.size()
					<< " but expected 5";
			return st.str();
		}

		color.red = command[2];
		color.green = command[3];
		color.blue = command[4];

		// TODO start koordinaten, animation_delay, strich breite, color reset? !!
		if (animation_x1 == this->panel->getWidth() - 1)
			animation_y1++;

		if (animation_x1 < this->panel->getWidth() - 1)
			animation_x1++;

		if (animation_y1 == this->panel->getHeight() - 1) {
			animation_x1 = 0;
			animation_y1 = 0;
		}

		animation_x2 = panel->getWidth() - 1 - animation_x1;
		animation_y2 = panel->getHeight() - 1 - animation_y1;

		panel->setColor(color_black); /* color reset */
		panel->drawLine(animation_x1, animation_y1, animation_x2, animation_y2,
				color, 1);
		panel->draw();
	}
	return "OK";
}

std::string ServerDisplayHandler::waterdrop(std::string command) {
	color_t color;

	if (animation_delay-- == 0) {
		animation_delay = 3;

		if (command.size() != 8) {
			lastCommand[0] = 0x0;
			stringstream st;
			st << "WATERDROP: wrong command size: received " << command.size()
					<< " but expected 8";
			return st.str();
		}

		uint8_t x = command[2];
		uint8_t y = command[3];
		uint8_t radius = command[4];

		color.red = command[5]
				* ((panel->getWidth() - ((radius == 0) ? 1 : radius))
						/ (float) (panel->getWidth() + 1));
		color.green = command[6]
				* ((panel->getWidth() - ((radius == 0) ? 1 : radius))
						/ (float) (panel->getWidth() + 1));
		color.blue = command[7]
				* ((panel->getWidth() - ((radius == 0) ? 1 : radius))
						/ (float) (panel->getWidth() + 1));

		this->panel->setColor(color_black);
		this->panel->drawCircle(x, y, radius, color, 1);
		this->panel->draw();

		if (radius < panel->getWidth()) {
			lastCommand[9]++;
		} else {
			lastCommand[7] = rand() % (panel->getWidth() - 1);
			lastCommand[8] = rand() % (panel->getHeight() - 1);
			lastCommand[9] = 0;
		}
	}
	return "OK";
}

string ServerDisplayHandler::verticalFade(string command) {
	color_t color;
	uint8_t diff = 4;

	if (animation_delay-- == 0) {
		animation_delay = 3;

		if (command.size() != 5) {
			lastCommand[0] = 0x0;
			stringstream st;
			st << "VERTICAL_FADE: wrong command size: received "
					<< command.size() << " but expected 5";
			return st.str();
		}

		color.red = command[2];
		color.green = command[3];
		color.blue = command[4];

		for (uint8_t x = 0; x < panel->getWidth(); x++) {
			for (uint8_t y = 0; y < panel->getHeight(); y++) {
				panel->setPixel(x, y, color);
			}

			if (color.blue < diff) {
				if (color.green <= (255 - diff))
					color.green += diff;

				if (color.red >= diff)
					color.red -= diff;
				else
					color.blue += diff;
			}

			if (color.red < diff) {
				if (color.blue <= (255 - diff))
					color.blue += diff;

				if (color.green >= diff)
					color.green -= diff;
				else
					color.red += diff;
			}

			if (color.green < diff) {
				if (color.red <= (255 - diff))
					color.red += diff;

				if (color.blue >= diff)
					color.blue -= diff;
				else
					color.green += diff;
			}

			/* set new start color */
			if (x == 1) {
				lastCommand[7] = color.red;
				lastCommand[8] = color.green;
				lastCommand[9] = color.blue;
			}
		}
		panel->draw();
	}

	return "OK";
}

string ServerDisplayHandler::doAnimation(string command) {

	switch (command[1]) {
	case 0x01:
		return this->rotateLine(command);

	case 0x02:
		return this->waterdrop(command);

	case 0x03:
		return this->verticalFade(command);

	default:
		return "unknown animation";
	}

	return "OK"; /* compiler placebo */
}

/**
 * interprets the protocol and executes it
 */
string ServerDisplayHandler::executeCommand(string command) {
	switch ((uint8_t) command[0]) {
	case 0xC9:
		return executeTPM2Protocol(
				string(command.c_str() + 1, command.length() - 1));
	case 0x42:
		return executeMyProtocol(
				string(command.c_str() + 1, command.length() - 1));
	default:
		return "unknown protocol";
	}
}

/**
 *  Pixel matrix protocol
 *  see: http://www.ledstyles.de/ftopic18969.html for more information (german)
 */
string ServerDisplayHandler::executeTPM2Protocol(string command) {

	unsigned int size;

	cout << "TPM2 frame received" << endl;

	if (command[0] != 0xDA) {
		cerr << "unknown TPM2 frame type: " << hex << command[0] << dec << endl;
		return "";
	}

	if (command[command.length() - 1] != 0x36) {
		cerr << "ServerDisplayHandler: wrong TPM2 end byte" << endl;
		lastCommand[0] = 0x0; // stop doing anything
		return "";
	}

	size = (((unsigned int) command[1]) << 8) | command[2];
	if (size < command.length() + 4) {
		cerr << "ServerDisplayHandler: tmp2 frame is too short" << endl;
		lastCommand[0] = 0x0; // stop doing anything
		return "";
	}

	panel->draw(command.length() - 4, (uint8_t*) &command.c_str()[3]);

	return string(1, (char) 0xAC);
}

/**
 * Reads the header of MyProtocol, checks the size and returns the pure command.
 *
 *  message format:
 *   ___________________________________________
 *  | num_bytes | message_message_message_message |
 *  |0_|1_|2_|3_|______x bytes____________________|
 *
 *  where num_bytes is 4-byte binary and represents the length of the message (excluding num_byte)!
 *
 */
string ServerDisplayHandler::checkMyPMessageSize(string message) {
	unsigned long num_bytes = 0L;

	if (4 > message.size()) {
		cerr << "ServerDisplayHandler: invalid myp message received" << endl;
		lastCommand[0] = 0x0; // stop doing anything
		return "";
	}

	num_bytes = (message[0] << 24) | (message[1] << 16) | (message[2] << 8)
			| (message[3]);

//	cout << "num_bytes: " << num_bytes << " - message[0|1|2|3]: "
//			<< (int) message[0] << "|" << (int) message[1] << "|"
//			<< (int) message[2] << "|" << (int) message[3] << "|" << endl;

	if (message.size() == num_bytes + 4) {
		string rt = string(message.c_str() + 4, message.size() - 4);
		return rt;
	}

	if (message.size() > num_bytes + 4) {
		cerr << "ServerDisplayHandler: myp message is too long!!"
				<< message.size() << " vs " << num_bytes + 4 << endl;
		lastCommand[0] = 0x0; // stop doing anything
		return string(message.c_str() + 4);
	}

	cerr << "ServerDisplayHandler: myp message incomplete: received "
			<< message.size() - 4 << " - expected " << num_bytes << endl;
	lastCommand[0] = 0x0; // stop doing anything
	string rt = string(message.c_str() + 4, message.size() - 4);
	return rt;
}

/**
 * interprets a command of the MyProtocol
 */
string ServerDisplayHandler::executeMyProtocol(string command) {
	uint8_t type;
	string response = "-OK";

	command = checkMyPMessageSize(command);
	type = command[0];
	response[0] = type;

	switch (type) {
	case COMMAND_DO_NOTHING:
		/* do nothing (in case of an error occurred ) */
		break;

	case COMMAND_DRAW_COLOR:
		response = this->drawColor(command);
		break;

	case COMMAND_FADE_COLOR:
		response = this->fadeColor(command);
		break;

	case COMMAND_PULSE_COLOR:
		response = this->pulseColor(command);
		break;

	case COMMAND_DRAW_PICTURE:
		response = this->drawPicture(command);
		break;

	case COMMAND_DRAW_PIXEL:
		cerr << "COMMAND_DRAW_PIXEL not implemented" << endl;
		break;

	case COMMAND_DRAW_LINE:
		response = this->drawLine(command);
		break;

	case COMMAND_DRAW_CIRCLE:
		response = this->drawCircle(command);
		break;

	case COMMAND_DRAW_RECT:
		cerr << "COMMAND_DRAW_RECT not implemented" << endl;
		break;

	case COMMAND_DO_ANIMATION:
		response = this->doAnimation(command);
		break;

	default:
		cerr << "[ERROR] unknown command: 0x'" << hex
				<< (unsigned int) command[0] << dec << "'" << endl;
		response = "-unknown command";
		response[0] = 0xff;

		command[0] = 0;
	}

	return makeHeader(response);
}

void ServerDisplayHandler::run() {
	unsigned long long refTime;
	string rt;

	try {
		panel->showBootLogo();

		while (1) {
			while (1) {
				refTime = Utile::getTime();

				if (server->hasMessage())
					break;

				if (!server->isAlive())
					throw MyException("Server is dead");

				executeCommand(lastCommand);

				/* keep 60fps */
				usleep(16667 - (Utile::getTime() - refTime));
			}

			lastCommand = getCommand();
			rt = executeCommand(lastCommand);
			server->putMessage(makeHeader(rt));
		}
	} catch (MyException &e) {
		cerr << e.what() << endl;
	}

	cout << "shutdown" << endl;
	server->shutdown();
}
