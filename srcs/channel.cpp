#include "../includes/channel.hpp"

Channel::Channel(std::string name) 
{
    _name = name;
    _nb_user = 0;   
}

Channel::~Channel(void) {}

//Getters
std::string             Channel::get_name(void) const { return _name; }
std::vector<User*>      Channel::get_users(void) const { return _users; }

User*                   Channel::get_user(std::string name)
{
    for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        if ((*it)->get_name() == name)
            return (*it);
    }
    return NULL;
}

//Checkers
int                    Channel::is_user_in_channel(int fd)
{
    for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        if ((*it)->get_socket() == fd)
            return 1;
    }
    return 0;
}

//Methods
int                     Channel::add_user(User* user) 
{
    _users.push_back(user);
    return 0;
}

int                     Channel::remove_user(User* user)
{
    for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); it++)
    {
        if ((*it)->get_socket() == user->get_socket())
        {
            _users.erase(it);
        }
        return 0;
    }
    return 0;
}

//Display
void                    Channel::display_users(void)
{
    if (!_users.empty())
    {
        for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); it++)
        {
            std::cout << "Name: " << (*it)->get_name() << " | " << "Fd: " << (*it)->get_socket() << " |" << std::endl; 
        }
    }
}