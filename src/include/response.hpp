#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include "request.hpp"

class Response {
   public:
    static Response GenerateResponse(const Request& request, const std::string& server_files_dir);
    void Send(int client_fd) const;

   private:
    std::string status_line_;
    std::string content_type_;
    int content_length_;
    std::string body_;
    std::string response_;
};

#endif  // RESPONSE_HPP
