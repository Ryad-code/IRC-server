#include "../includes/server.hpp"

//Constructor
Server::Server(int port, std::string password)
{
    bzero(_buffer, 1024);

    _port = port;
    _password = password;
    _nb_user = 0;

    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(_port);

    _addr_len = sizeof(_address);

    _socket = socket(AF_INET, SOCK_STREAM, 0);
}
//Destructor
Server::~Server(void)
{}

//Getters
int                         Server::get_socket(void) const { return _socket; }
int                         Server::get_port(void) const { return _port; }
int                         Server::get_nb_user(void) const { return _nb_user; }
std::string                 Server::get_password(void) const { return _password; }
std::vector<User>           Server::get_users(void) const { return _users; }

//Methods
int                         Server::bind_sock(void)
{
    std::cout << "Binding..." << std::endl;
    return (bind(_socket, (struct sockaddr *)&_address, _addr_len));
}

int                         Server::listen_sock(void)
{
    std::cout << "Listening..." << std::endl;
    return (listen(_socket, BACK_LOG));
}

int                         Server::accept_sock(void)
{
    std::cout << "Accepting..." << std::endl;
    return (accept(_socket, (struct sockaddr *)&_address, (socklen_t *)&_addr_len));
}

int                         Server::recv_sock(int fd)
{
    std::cout << "Receiving..." << std::endl;
    return (recv(fd, _buffer, BUFF_SIZE, 0));
}

int                          Server::send_sock(int fd, char* str)
{
    std::cout << "Sending..." << std::endl;
    return (send(fd, str, strlen(str), 0));
}
//////////////////FD sets
int                         Server::set_fds(void)
{
    std::cout << "Setting FDs" << std::endl;
    FD_ZERO(&_read);
    FD_ZERO(&_write);
    FD_ZERO(&_except);

    FD_SET(_socket, &_read);
    FD_SET(_socket, &_write);
    FD_SET(_socket, &_except);
    for (int i = 0; i < _nb_user; i++)
    {
        FD_SET(_users[i].get_socket(), &_read);
        FD_SET(_users[i].get_socket(), &_write);
        FD_SET(_users[i].get_socket(), &_except);
    }
    return (0);
}

int                         Server::select_sock(void)
{
    std::cout << "Selecting FDs" << std::endl;
    return (select(FD_SETSIZE, &_read, &_write, &_except, NULL));
}

int                         Server::new_connection(void)
{
    std::cout << "New connection to server" << std::endl;
    int new_fd = 0;
    if (FD_ISSET(_socket, &_read)) //--->If listening socket is readable--->New connection
    {
        std::cout << "New connection confirmed\n";
        new_fd = accept_sock();
        _users.push_back(User(new_fd, ""));
        _nb_user++;

        recv_sock(new_fd);
    }
    return new_fd;
}

int                         Server::manage_connections(void)
{
    for (int i = 0; i < _nb_user; i++)
    {
        if (FD_ISSET(_users[i].get_socket(), &_read))
        {
            if (recv(_users[i].get_socket(), _buffer, BUFF_SIZE, 0) < 0)
            {
                std::cout << "Failed to receive message from existing client\n";
                return -1;
            }
            std::cout << "Message received by user fd[" << i << "]" << std::endl;
            std::cout << _buffer << std::endl;
            bzero(_buffer, 1024);
        }        
    }
    return 0;
}

//--------------------------------------------------------->Display
void                         Server::print_buff(void) const
{
    std::cout << _buffer << std::endl;
}

void                          Server::print_users(void) const
{
    for (auto it = _users.begin(); it != _users.end(); it++)
    {
        (*it).display_user();
    }
}

