#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cout << "Logs from your program will appear here!\n";

    // Uncomment this block to pass the first stage

    // AF_INET is the address family used for IPv4 addresses;
    // SOCK_STREAM is the socket type used for TCP connections
    // 0 is the protocol argument, it will choose TCP for stream sockets
    // Returns a file descriptor for the socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Failed to create server socket\n";
        return 1;
    }

    // Since the tester restarts your program quite often, setting SO_REUSEADDR
    // ensures that we don't run into 'Address already in use' errors.
    // SOL_SOCKET is the socket layer itself
    // &reuse is the value that we're setting the option to (1 for enabling it)
    int reuse = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        std::cerr << "setsockopt failed\n";
        return 1;
    }

    // sockaddr_in is a structure in library <netinet/in.h>
    // sin_family is the address family (AF_INET for IPv4 addresses)
    // sin_addr is the address in network byte order, INADDR_ANY means any address for binding
    // sin_port is the port in network byte order
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(4221);

    // Binds the socket to the address and port number specified in server_addr
    // sockaddr is structure in library <sys/socket.h> that has the same size as sockaddr_in
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
        std::cerr << "Failed to bind to port 4221\n";
        return 1;
    }

    // The number of connections that can be waiting while the process is handling a particular connection
    int connection_backlog = 5;
    if (listen(server_fd, connection_backlog) != 0) {
        std::cerr << "listen failed\n";
        return 1;
    }

    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);

    std::cout << "Waiting for a client to connect...\n";

    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, (socklen_t*)&client_addr_len);
    std::cout << "Client connected\n";

    // respond to an HTTP request with a 200 response
    std::string response = "HTTP/1.1 200 OK\r\n\r\n";
    send(client_fd, response.c_str(), response.size(), 0);

    close(server_fd);

    return 0;
}
