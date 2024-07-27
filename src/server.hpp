#ifndef SERVER_HPP
#define SERVER_HPP

class Server {
   public:
    Server();
    ~Server();
    void Run();

   private:
    int CreateServerSocket();
    static void HandleClient(int client_fd);
    int server_fd_;
};

#endif  // SERVER_HPP