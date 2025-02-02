#ifndef SERVER_HPP
#define SERVER_HPP
#include "config.hpp"
class Server {
   public:
    Server(int argc, char** argv);
    ~Server();
    void Run();

   private:
    int CreateServerSocket();
    void HandleClient(int client_fd);

    int server_fd_;
    Config server_config_;
};

#endif  // SERVER_HPP