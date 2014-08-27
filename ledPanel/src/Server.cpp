/*
 * Server.cpp
 *
 *  Created on: 02.02.2013
 *      Author: martin
 */

#include "Server.h"

using namespace std;

/**
 * static method for invoking a class-method out of a thread
 */
void* Server::serverTaskStatic(void* o) {
	return static_cast<Server*>(o)->serverTask();
}

/**
 * CTOR
 */
Server::Server(serverType_t type, unsigned int port) :
		stop(false), alive(true), mySocket(0), shut_down(false), listenSocket(
				0), type(type), port(port), addr_size(0), close_connection(
				false) {
	int rt;
	pthread_attr_t threadAttr;

	rt = pthread_attr_init(&threadAttr);
	if (0 != rt) {
		throw MyException(
				"BusAccessScheduler: thread attribute initialization failed: "
						+ string(strerror(rt)));
	}

	rt = pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);
	if (0 != rt) {
		throw MyException(
				"BusAccessScheduler: setting detach state failed: "
						+ string(strerror(rt)));
	}

	rt = pthread_create(&serverThread, NULL, &serverTaskStatic, this);
	if (rt != 0) {
		cout << "unable to create thread: " << strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}
}

/**
 * DTOR
 */
Server::~Server() {
	closeSocket(this->mySocket);
	closeSocket(this->listenSocket);
}

/**
 * initializes the socket
 */
void Server::init() {
	/*prepare settings*/
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(this->port);
	memset(&(my_addr.sin_zero), 0, 8);
	my_addr.sin_addr.s_addr = INADDR_ANY;

	/* create socket*/
	if (type == TCP) {
		listenSocket = socket(AF_INET, SOCK_STREAM, 0); // TCP
		cout << "create TCP socket" << endl;
	} else {
		listenSocket = socket(AF_INET, SOCK_DGRAM, 0);	// UDP
		cout << "create UDP socket" << endl;
	}

	if (-1 == listenSocket) {
		throw MyException(
				"unable to create socket: " + string(strerror(errno)));
	}

	/*bind socket*/
	if (-1 == bind(listenSocket, (sockaddr*) &my_addr, sizeof(my_addr))) {
		throw MyException("unable to bind socket: " + string(strerror(errno)));
	}

	cout << "Server listens on port " << this->port << endl;

	/*listen for connection (not necessary for UDP)*/
	if (type == TCP) {
		if (-1 == listen(listenSocket, 1)) {
			throw MyException(
					"unable to listen socket: " + string(strerror(errno)));
		}
	}
}

/**
 * receives a message
 */
string Server::receiveCommand() {
	int bufferLength = 1024;
	char buffer[1024];
	int rc;

	memset(buffer, 0, bufferLength);

	/*receive*/
	if (type == TCP)
		rc = recv(mySocket, buffer, bufferLength, 0);
	else
		rc = recv(mySocket, buffer, bufferLength, MSG_WAITALL);

	if (-1 == rc) {
		cerr << "[ERROR] receive failed: " << strerror(errno) << "(" << errno
				<< ")" << endl;
		throw MyException("receive failed: " + string(strerror(errno)));
	} else if (0 == rc) {
		cerr << "[ERROR] receive failed: " << strerror(errno) << "(" << errno
				<< ")" << endl;
		throw MyException("client disconnected");
	}

	return string(buffer, rc);
}

/**
 * sends a message
 */
void Server::sendResponse(string response) {
	int bufferLength = response.size();

//    cout << "socket send response: " << response << endl;

	if (-1 == send(mySocket, response.c_str(), bufferLength, 0)) {
		cerr << "[ERROR] send failed: " << strerror(errno) << "(" << errno
				<< ")" << endl;
		throw MyException("send failed: " + string(strerror(errno)));
	}
}

/**
 * closes the socket
 */
void Server::closeSocket(int socket) {
	if (-1 != socket)
		close(socket);

	socket = -1;
}

/**
 * closes the connection with the client
 */
void Server::closeConnection() {
	this->close_connection = true;
}

/**
 * shuts the server down
 */
void Server::shutdown() {
	this->shut_down = true;
	closeSocket(listenSocket);
}

/**
 * returns true if the server has received a message
 */
bool Server::hasMessage() {
	if (0 != this->rx.getSize())
		return true;

	return false;
}

/**
 *  returns true if the server is not shut down
 */
bool Server::isAlive() {
	return alive;
}

/**
 * returns a received message
 */
std::string Server::takeMessage() {
	return rx.take();
}

/**
 * sends the given message
 */
void Server::putMessage(std::string message) {
	tx.put(message);
}

/**
 * main task of the server thread
 */
void* Server::serverTask() {
	string message;
	string response;

	try {
		init();

		while (!shut_down) {
			close_connection = false;

			cout << "\n\n\n[INFO] wait for connection ..." << endl;

			if (type == TCP) {
				/*establish connection with client (blocks until a client is connected)*/
				mySocket = accept(listenSocket, (sockaddr*) &sadr, &addr_size);
				if (-1 == mySocket) {
					cerr << "[ERROR] socket accept failed: " << strerror(errno)
							<< "(" << errno << ")" << endl;
					break;
				}

				// TODO ip bei erster Verbindung falsch!!
				cout << "[INFO] connected with " << inet_ntoa(sadr.sin_addr)
						<< " on port " << sadr.sin_port << endl;
			} else {
				mySocket = listenSocket;
			}

			try {
				/*connection established. ready for receiving data*/
				while ((!close_connection) && (!shut_down)) {
					message = this->receiveCommand();

					if (message[4] == 0xff)
						break;

					rx.put(message);

					response = tx.take();

					if (!response.empty())
						this->sendResponse(response);
				}
			} catch (MyException &e) {
				cerr << "[ERROR] " << e.what() << endl;
			}

			cout << "[INFO] close connection" << endl;
			closeSocket(this->mySocket);
		}
	} catch (MyException &e) {
		cerr << "[ERROR] " << e.what() << endl;
	}

	this->alive = false;
	cout << "[INFO] Server is shutting down" << endl;
	closeSocket(listenSocket);

	return NULL;
}

