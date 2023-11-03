#include "../includes/user.hpp"

User::User(int socket, std::string name)
{
    bzero(_buffer, 1024);

    _socket = socket;
    _name  = name;
    _status = 0;
}

User::~User(void) {}

//Getters
int                     User::get_socket(void) const { return _socket; }
std::string             User::get_name(void) const { return _name; }
int                     User::get_status(void) const { return _status; }

void                    User::display_user(void) const
{
    std::cout << get_name() << " | " << get_socket() << " | " << std::endl;
}