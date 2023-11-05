#include "../includes/server.hpp"

int main(int ac, char **av)
{
    int user = 0;
    int res;

    if (ac != 2)
    {
        std::cout << "Wrong number of argument\n";
        return (-1);
    }
    //Welcome to our IRC server
    std::cout << "Welcome to our IRC server" << std::endl;
    //Initialiasing Server
    Server  IRC = Server(5555, "password");
    //Binding socket & server address
    IRC.bind_sock();
    //Socket listening for new connections
    IRC.listen_sock();

    //Main loop
    int on = 1;
    while (on == 1)
    {    
        IRC.set_fds();
        IRC.select_sock();

        user = IRC.new_connection();
        res = IRC.manage_connections();

        IRC.clean_buffer();
        IRC.clean_users_buffer();
        if (res < 0)
        {
            std::cout << "EXIT" << std::endl;
            on++;
        }
    }
    std::cout << "VOILA ta grand mere la pute" << std::endl;

    return 0;
}
