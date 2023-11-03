#include "../includes/server.hpp"

int main(int ac, char **av)
{
    int on = 0;
    int user = 0;

    if (ac != 2)
    {
        perror("Wrong number of argument\n");
        exit(EXIT_FAILURE);
    }
    //Welcome to our IRC server
    std::cout << "Welcome to our IRC server" << std::endl;

    //Initialiasing Server
    Server  IRC = Server(5555, "password");

    //Binding socket & server address
    IRC.bind_sock();
    //Socket listening for new connections
    IRC.listen_sock();

    on = 1;
    while (on)
    {
        IRC.set_fds();
        IRC.select_sock();
        user = IRC.new_connection();

        
        
        IRC.recv_sock(user);
        IRC.print_buff();
        usleep(5000);
    }

    //    std::cout << IRC.get_socket() << std::endl;
    //    std::cout << IRC.get_port() << std::endl;
    //    std::cout << IRC.get_nb_user() << std::endl;
    //    std::cout << IRC.get_password() << std::endl;
    return 0;
}
