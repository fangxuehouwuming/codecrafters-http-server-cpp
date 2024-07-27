#ifndef SERVER_HPP
#define SERVER_HPP
#include "config.hpp"
class Server {
   public:
    Server(const Config& config);
    ~Server();
    void Run();

   private:
    int CreateServerSocket();
    void HandleClient(int client_fd);

    int server_fd_;
    std::string directory_;
};

#endif  // SERVER_HPP