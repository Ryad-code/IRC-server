#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <strings.h>

#include <sys/socket.h>
#include <netinet/in.h>

class Socket
{
        private:
                int             _socket;
                sockaddr_in     _address;
                int             _back_log;

        public:
                //Constructor & Destructor
                Socket(int domain, int type, int protocol);
}
