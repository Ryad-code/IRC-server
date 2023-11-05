#ifndef CHANNEL_HPP
#define CHNANEL_HPP

#include <iostream>
#include <iomanip>


#include "user.hpp"

class Channel
{
    private:
            std::string                 _name;
            std::vector<User>           _users;
            int                         _nb_user;

    public:
            Channel(std::string name);
            ~Channel(void);

            //Getters
            std::string                 get_name(void) const;
            std::vector<User>           get_users(void) const;
            User*                       get_user(std::string name);

            //Checkers
            int                         is_user_in_channel(int fd);    
            
            //Methods
            int                         add_user(User user);
            int                         remove_user(User user);

            //Display
            void                        display_users(void);


};

#endif