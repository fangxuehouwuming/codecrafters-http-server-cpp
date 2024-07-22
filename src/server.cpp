#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> split_message(const std::string& message, const std::string& delim) {
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;
    while ((end = message.find(delim, start)) != std::string::npos) {
        tokens.push_back(message.substr(start, end - start));
        start = end + delim.size();
    }
    tokens.push_back(message.substr(start));
    return tokens;
}

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

    // Read from the client
    char buffer[1024] = {0};
    int valread = read(client_fd, buffer, 1024);
    std::string request(buffer);
    std::cout << request << std::endl;

    if (valread < 0) {
        std::cerr << "read failed\n";
        return 1;
    }

    std::string response = "HTTP/1.1 404 Not Found\r\n\r\n";

    // request = "GET /echo/abc HTTP/1.1\r\nHost: localhost:4221\r\nUser-Agent: curl/7.64.1\r\nAccept: */*\r\n\r\n"
    std::string path = split_message(request, " ")[1];
    std::vector<std::string> split_paths = split_message(path, "/");
    if (path == "/") {
        response = "HTTP/1.1 200 OK\r\n\r\n";
    } else if (split_paths[1] == "echo") {
        response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: " + std::to_string(split_paths[2].length()) + "\r\n\r\n" + split_paths[2];
    }

    send(client_fd, response.c_str(), response.size(), 0);

    close(server_fd);

    return 0;
}
