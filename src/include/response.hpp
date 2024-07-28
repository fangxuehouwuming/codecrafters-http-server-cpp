#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <unordered_map>
#include "config.hpp"
#include "request.hpp"

class Response {
   public:
    static Response GenerateResponse(const Request& request, const Config& server_config);
    void Send(int client_fd) const;

   private:
    std::string status_line_;
    std::unordered_map<std::string, std::string> headers_;
    std::string body_;

    std::string ComposeResponse() const;
};

#endif  // RESPONSE_HPP