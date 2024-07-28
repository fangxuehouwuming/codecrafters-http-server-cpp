#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>

class Request {
   public:
    bool Parse(int client_fd);
    const std::string& GetMethod() const;
    const std::string& GetPath() const;
    const std::string& GetUserAgent() const;

   private:
    std::string method_;
    std::string path_;
    std::string user_agent_;
};

#endif  // REQUEST_HPP
