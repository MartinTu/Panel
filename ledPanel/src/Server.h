/*
 * Server.h
 *
 *  Created on: 02.02.2013
 *      Author: martin
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <vector>
#include <pthread.h>

//socket
//#include <winsock2.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "MyException.h"
#include "BlockingQueue.h"
#include "Utile.h"

enum serverType_t{
	UDP = 0,
	TCP
};


class Server
{
    public:
        Server(serverType_t type, unsigned int port);
        virtual ~Server();

        bool hasMessage();
        bool isAlive();

        std::string takeMessage();
        void putMessage(std::string message);

        void closeConnection();
        void shutdown();

        static void* serverTaskStatic(void* o);

    protected:
        void init();
        void sendResponse(std::string response);
        std::string receiveCommand();
        void closeSocket(int socket);

        void* serverTask();

        bool stop;
        bool alive;
        int mySocket;
        bool shut_down;
        int listenSocket;
        sockaddr_in sadr;
        serverType_t type;
        unsigned int port;
        socklen_t addr_size;
        bool close_connection;
        pthread_t serverThread;
        struct sockaddr_in my_addr;
        BlockingQueue<std::string> rx;
        BlockingQueue<std::string> tx;
};

#endif /* SERVER_H_ */
