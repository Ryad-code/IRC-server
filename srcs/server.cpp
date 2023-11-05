#include "../includes/server.hpp"

//Constructor
Server::Server(int port, std::string password)
{
    int opt = 1;

    _port = port;
    _password = password;
    _nb_user = 0;
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(_port);
    _addr_len = sizeof(_address);
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    bzero(_buffer, 1024);

    _channels.push_back(Channel("Main"));
}
//Destructor
Server::~Server(void)
{}

//Getters
int                        Server::get_socket(void) const { return _socket; }
int                        Server::get_port(void) const { return _port; }
int                        Server::get_nb_user(void) const { return _nb_user; }
std::string                Server::get_password(void) const { return _password; }

std::vector<User*>           Server::get_users(void) const { return _users; }
User*                       Server::get_user(std::string name)
{

    for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        if ((*it)->get_name() == name)
            return (*it);
    }
    return NULL;
}
std::vector<Channel>        Server::get_channels(void) const { return _channels; }
Channel*                    Server::get_channel(std::string name)
{

    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
    {
        if ((*it).get_name() == name)
            return &(*it);
    }
    return NULL;
}            

///////////////////////////////////////////////////////////////////METHODS
int                         Server::bind_sock(void)
{
  //  std::cout << "Binding..." << std::endl;
    return (bind(_socket, (struct sockaddr *)&_address, _addr_len));
}

int                         Server::listen_sock(void)
{
  //  std::cout << "Listening..." << std::endl;
    return (listen(_socket, BACK_LOG));
}

int                         Server::accept_sock(void)
{
    //std::cout << "Accepting..." << std::endl;
    return (accept(_socket, (struct sockaddr *)&_address, (socklen_t *)&_addr_len));
}

int                         Server::recv_sock(int fd, char *buffer)
{
//    std::cout << "Receiving..." << std::endl;
    return (recv(fd, (void*)buffer, BUFF_SIZE, 0));
}

int                          Server::send_sock(int fd, std::string message)
{
//    std::cout << "Sending..." << std::endl;
    return (send(fd, message.c_str(), strlen(message.c_str()), 0));
}
//////////////////FD sets
int                         Server::set_fds(void)
{
//    std::cout << "Setting FDs" << std::endl;
    FD_ZERO(&_read);
    FD_ZERO(&_write);
    FD_ZERO(&_except);

    FD_SET(_socket, &_read);
    FD_SET(_socket, &_write);
    FD_SET(_socket, &_except);
    for (int i = 0; i < _nb_user; i++)
    {
        FD_SET(_users[i]->get_socket(), &_read);
        FD_SET(_users[i]->get_socket(), &_write);
        FD_SET(_users[i]->get_socket(), &_except);
    }
    return (0);
}

int                         Server::select_sock(void)
{
   // std::cout << "Selecting FDs" << std::endl;
    return (select(FD_SETSIZE, &_read, &_write, &_except, NULL));
}

int                         Server::new_connection(void)
{
    int     new_fd = 0;
    User*   new_user;
    if (FD_ISSET(_socket, &_read)) //--->If listening socket is readable--->New connection
    {
        std::cout << "New connection to server" << std::endl;
        new_fd = accept_sock();
        new_user = new User(new_fd, "alfred");
        _users.push_back(new_user);//---------------->Creation of new user
        _nb_user++;
        add_user_to_channel(new_user, "Main");//----->Adding new user to Main channel
        recv_sock(new_fd, _users[_nb_user -1]->get_buffer());
    }
    return new_fd;
}

int                         Server::manage_connections(void)
{
    for (int i = 0; i < _nb_user; i++)
    {
        if (FD_ISSET(_users[i]->get_socket(), &_read))//---->If an existing user is readable
        {
            if (recv(_users[i]->get_socket(), _users[i]->get_buffer(), BUFF_SIZE, 0) < 0)
            {
                std::cout << "Failed to receive message from existing client\n";
                return -1;
            }
            std::cout << "FD[" << i << "]:" << std::endl;//----->Display new user message
            std::cout << _users[i]->get_buffer() << std::endl;
            if ((_users[i]->get_buffer())[0] == 'E' && (_users[i]->get_buffer())[1] == 'X' && (_users[i]->get_buffer())[2])
                return -1;
        }        
    }
    return 0;
}

//..........................................................Buffer
void                         Server::clean_buffer(void)
{
    bzero(_buffer, 1024);
}

void                         Server::clean_users_buffer(void)
{
    for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        (*it)->clean_buffer();
    }
}
//...........................................................Sending
int                          Server::send_to_all_users(std::string message)
{
    for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        std::cout << "Sending to all users" << std::endl;
        send_sock((*it)->get_socket(), message);
    }
    return 0;  
}

int                     Server::send_to_channel(int sender, std::string channel, std::string message)
{
    Channel* tmp_channel = get_channel(channel);
    std::vector<User*> tmp_users =  tmp_channel->get_users();
    for (std::vector<User*>::iterator it = tmp_channel->get_users().begin(); it != tmp_channel->get_users().end(); it++)
    {
        if ((*it)->get_socket() != sender)
        {
            write((*it)->get_socket(), message.c_str(), strlen(message.c_str()));
        }
    }
    return 0;
}

//.........................................................Channels
int                          Server::add_user_to_channel(User* user, std::string channel)
{
    for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
    {
        if ((*it).get_name() == channel)
        {
            (*it).add_user(user);
        }
    }
    return 0;
}

//--------------------------------------------------------->DISPLAY
void                         Server::print_buff(void) const
{
    std::cout << _buffer << std::endl;
}

void                          Server::print_users(void)
{
    for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        std::cout << "Name: " << (*it)->get_name() << " | " << "Fd: " << (*it)->get_socket() << " |" << std::endl;
    }
}

void                          Server::print_users_from_channel(std::string channel)
{
        Channel* tmp_channel = get_channel(channel);
        if (tmp_channel)
            tmp_channel->display_users();
        else
            std::cout << "No such channel" << std::endl;
}
