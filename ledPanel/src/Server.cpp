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
		stop(false),
		alive(true),
		mySocket(0),
		shut_down(false),
		listenSocket(0),
		type(type),
		port(port),
		clientAddrLen(0),
		close_connection(false) {
	int rt;
	struct sched_param param;
	pthread_attr_t threadAttr;

	rt = pthread_attr_init(&threadAttr);
	if (0 != rt) {
		throw MyException(
				"Server: thread attribute initialization failed: "
						+ string(strerror(rt)));
	}

	rt = pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);
	if (0 != rt) {
		throw MyException(
				"Server: setting detach state failed: "
						+ string(strerror(rt)));
	}

	rt = pthread_create(&serverThread, NULL, &serverTaskStatic, this);
	if (rt != 0) {
		cout << "unable to create thread: " << strerror(errno) << endl;
		exit(EXIT_FAILURE);
	}

    param.sched_priority = 50; 	//sched_get_priority_max(SCHED_FIFO);
    rt = pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
    if (0 != rt)
    {
        throw MyException("Server: unable to change schedule parameters: " + string(strerror(rt)));
    }
    Utile::printStars();
	cout << "* Server thread:" << endl;
	Utile::display_thread_sched_attr();
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
	//bzero((char *) &serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;//
	serverAddr.sin_port = htons(this->port);//set port number
	memset(&(serverAddr.sin_zero), 0, 8);
	serverAddr.sin_addr.s_addr = INADDR_ANY;//my ip adress

	/* create socket*/
	if (type == TCP) {
		listenSocket = socket(AF_INET, SOCK_STREAM, 0); // TCP
		cout << "* create TCP socket" << endl;
	} else {
		listenSocket = socket(AF_INET, SOCK_DGRAM, 0); //UDP
		cout << "* create UDP socket" << endl;

	}

	if (-1 == listenSocket) {
		shutdown();
		throw MyException("unable to create socket: " + string(strerror(errno)));
	}

	/*bind socket*/
	if (-1 == bind(listenSocket, (sockaddr*) &serverAddr, sizeof(serverAddr))) {
		shutdown();
		throw MyException("unable to bind socket: " + string(strerror(errno)));
	}

	cout << "* Server listens on port 0x" << hex << this->port << endl;

	/*listen for connection (not necessary for UDP)*/
	if (type == TCP) {
		if (-1 == listen(listenSocket,5)) {
			shutdown();
			throw MyException(
					"unable to listen socket: " + string(strerror(errno)));
		}
	}
}

/**
 * receives a message
 */
string Server::receiveCommand() {
	const int bufferLength = 1024;
	char buffer[bufferLength];
	int rc;

	memset(buffer, 0, bufferLength);

	/*receive*/
	if (type == TCP)
		rc = recv(mySocket, buffer, bufferLength, 0);
	else{
		clientAddrLen = (socklen_t) sizeof(clientAddr);
		rc = recvfrom(mySocket, buffer, bufferLength, 0, (sockaddr *) &clientAddr, &clientAddrLen);
	}
	if (-1 == rc) {
		throw MyException("receive failed: " + string(strerror(errno)));
	} else if (0 == rc) {
		cerr << "[ERROR] receive failed: " << strerror(errno) << "(" << errno
				<< ")" << endl;
		throw MyException("client disconnected");
	}
//	cout << "new BUFFER:" << endl;
//	for (int i = 0 ; i < rc; i++){
//		cout << hex << (int) buffer[i];
//	}
//	cout << endl;

	return string(buffer, rc);
}

/**
 * sends a message
 */
void Server::sendResponse(string response) {
	int bufferLength = response.size();

//    cout << "socket send response: " << response << endl;
	if (type == TCP){
		if (-1 == send(mySocket, response.c_str(), bufferLength, 0)) {
			throw MyException("send failed: " + string(strerror(errno)));
		}
	}
	else{
		int len;
		if (-1 == (len = sendto(mySocket, response.c_str(), bufferLength, 0, (sockaddr *) &clientAddr, sizeof(clientAddr)))) {
			throw MyException("sendto failed: " + string(strerror(errno)));
		}
		else if(bufferLength != len){
			throw MyException("sendto() sent a different number of bytes than expected");
		}
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
			Utile::printStars();
			cout << "[INFO] wait for connection ..." << endl;
			string actClient("");
			int actPort = 0;
			if (type == TCP) {
				/*establish connection with client (blocks until a client is connected)*/
				mySocket = accept(listenSocket, (sockaddr*) &clientAddr, &clientAddrLen);
				if (-1 == mySocket) {
					cerr << "[ERROR] socket accept failed: " << strerror(errno)
							<< "(" << errno << ")" << endl;
					break;
				}
			}
			else {
				mySocket = listenSocket;
			}

			try {
				/*connection established. ready for receiving data*/
				while ((!close_connection) && (!shut_down)) {
					message = this->receiveCommand();

					if (message[5] == 0xff) // close connection
						break;

					//put received message in que
					rx.put(message);
					//get a response from que
					response = tx.take();
					//send response
					if (!response.empty())
						this->sendResponse(response);
					//abort, if it is a UDP connection
					//if (type == UDP)
					//	break;
					if(type == TCP){
						cout 	<< "[INFO] connected with " << inet_ntoa(clientAddr.sin_addr)
								<< " on port 0x" << clientAddr.sin_port << endl;
					}
					else{
						if((0 != actClient.compare(inet_ntoa(clientAddr.sin_addr))) || (actPort != clientAddr.sin_port)){
							actClient = inet_ntoa(clientAddr.sin_addr);
							actPort = clientAddr.sin_port;
							cout 	<< "[INFO] connected with " << actClient
								    << " on port 0x" << actPort << endl;
						}
					}
				}
			} catch (MyException &e) {
				cerr << "[ERROR] " << e.what() << endl;
			}
			if (type == TCP){
				cout << "[INFO] close connection" << endl;
				closeSocket(this->mySocket);
			}
		}
	} catch (MyException &e) {
		cerr << "[ERROR] " << e.what() << endl;
	}

	this->alive = false;
	cout << "[INFO] Server is shutting down" << endl;
	closeSocket(listenSocket);

	return NULL;
}

