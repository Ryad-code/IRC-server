#ifndef SERVER_HPP
#define SERVER_HPP

#include <iomanip>
#include <iostream>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "user.hpp"

#define MAX_CLIENTS 10
#define BACK_LOG 10
#define BUFF_SIZE 1024

class Server
{
    private:
            int                         _socket;

            struct sockaddr_in          _address;
            int                         _addr_len;

            int                         _port;
            int                         _nb_user;
            char                        _buffer[1024];
            std::string                 _password;
            std::vector<User>           _users;

            fd_set                      _read;
            fd_set                      _write;
            fd_set                      _except;

    public:
            //Constructor & Destructor
            Server(int port, std::string password);
            ~Server(void);

            //Getters
            int                         get_socket(void) const;
            int                         get_port(void) const;
            int                         get_nb_user(void) const;
            std::string                 get_password(void) const;
            std::vector<User>           get_users(void) const;

            //Socket
            int                         bind_sock(void);          
            int                         listen_sock(void); 
            int                         select_sock(void);            
            int                         accept_sock(void);
            int                         recv_sock(int fd);
            int                         send_sock(int fd, char* str);
            //FDs
            int                         set_fds(void);
            int                         new_connection(void);
            int                         manage_connections(void);
            //Print
            void                        print_buff(void) const;
            void                        print_users(void) const;
            
    };

#endif