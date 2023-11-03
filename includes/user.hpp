#ifndef USER_HPP
#define USER_HPP
#include <iomanip>
#include <iostream>

#include <strings.h>

#include <vector>
#include <iterator>


class User
{
    private:
            int                 _socket;
            char                _buffer[1024];
            std::string         _name;
            int                 _status;

    public:
            User(int socket, std::string name);
            ~User(void);

            //Getters
            int                         get_socket(void) const;
            std::string                 get_name(void) const;
            int                         get_status(void) const;

            //Printing
            void                        display_user(void) const;
};

#endif