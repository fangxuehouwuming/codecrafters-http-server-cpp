#include "include/server.hpp"
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include "include/request.hpp"
#include "include/response.hpp"
namespace {
const int kPort = 4221;
const int kConnectionBacklog = 5;
}  // namespace

Server::Server()
    : server_fd_(CreateServerSocket()) {}

Server::~Server() {
    close(server_fd_);
}

int Server::CreateServerSocket() {
    // AF_INET is the address family used for IPv4 addresses;
    // SOCK_STREAM is the socket type used for TCP connections
    // 0 is the protocol argument, it will choose TCP for stream sockets
    // Returns a file descriptor for the socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Failed to create server socket\n";
        exit(EXIT_FAILURE);
    }

    // Since the tester restarts your program quite often, setting SO_REUSEADDR
    // ensures that we don't run into 'Address already in use' errors.
    // SOL_SOCKET is the socket layer itself
    // &reuse is the value that we're setting the option to (1 for enabling it)
    int reuse = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        std::cerr << "setsockopt failed\n";
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // sockaddr_in is a structure in library <netinet/in.h>
    // sin_family is the address family (AF_INET for IPv4 addresses)
    // sin_addr is the address in network byte order, INADDR_ANY means any address for binding
    // sin_port is the port in network byte order
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(kPort);

    // Binds the socket to the address and port number specified in server_addr
    // sockaddr is structure in library <sys/socket.h> that has the same size as sockaddr_in
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
        std::cerr << "Failed to bind to port " << kPort << "\n";
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // The number of connections that can be waiting while the process is handling a particular connection
    if (listen(server_fd, kConnectionBacklog) != 0) {
        std::cerr << "Listen failed\n";
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    return server_fd;
}

void Server::Run() {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    std::cout << "Waiting for a client to connect...\n";

    while (true) {
        int client_fd = accept(server_fd_, (struct sockaddr*)&client_addr, (socklen_t*)&client_addr_len);
        if (client_fd < 0) {
            std::cerr << "Accept failed\n";
            continue;
        }

        std::thread client_thread(HandleClient, client_fd);
        client_thread.detach();
    }
}

void Server::HandleClient(int client_fd) {
    Request request;
    if (!request.Parse(client_fd)) {
        std::cerr << "Failed to parse request\n";
        close(client_fd);
        return;
    }

    Response response = Response::GenerateResponse(request);
    response.Send(client_fd);

    close(client_fd);
}
