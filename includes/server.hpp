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

#include <exception>

#include "user.hpp"
#include "channel.hpp"

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
            std::vector<Channel>        _channels;

            fd_set                      _read;
            fd_set                      _write;
            fd_set                      _except;
//......................................................................
    public:
            //Constructor & Destructor
            Server(int port, std::string password);
            ~Server(void);

            //Getters
            int                        get_socket(void) const;
            int                        get_port(void) const;
            int                        get_nb_user(void) const;
            std::string                get_password(void) const;
            
            std::vector<User>          get_users(void) const;
            User*                      get_user(std::string name);

            std::vector<Channel>       get_channels(void) const;
            Channel*                   get_channel(std::string name);



            //Socket
            int                         bind_sock(void);          
            int                         listen_sock(void); 
            int                         select_sock(void);            
            int                         accept_sock(void);
            int                         recv_sock(int fd, char *buffer);
            int                         send_sock(int fd, std::string message);
            //FDs
            int                         set_fds(void);
            int                         new_connection(void);
            int                         manage_connections(void);
            //Buffer
            void                        clean_buffer(void);
            void                        clean_users_buffer(void);  
            //Sending
            int                         send_to_all_users(std::string message);
            int                         send_to_users(int fd, std::string message);
            //Channels
            int                         add_user_to_channel(User user, std::string channel);
            int                         send_to_channel(int sender, std::string channel, std::string message);
            //Print
            void                        print_buff(void) const;
            void                        print_users(void);
            void                        print_users_from_channel(std::string channel);
    };

#endif