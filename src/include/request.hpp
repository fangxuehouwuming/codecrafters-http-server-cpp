#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <unordered_map>

class Request {
   public:
    bool Parse(int client_fd);
    const std::string& GetMethod() const;
    const std::string& GetPath() const;
    const std::string& GetVersion() const;
    const std::unordered_map<std::string, std::string>& GetHeaders() const;
    const std::string& GetBody() const;

   private:
    std::string method_;
    std::string path_;
    std::string version_;
    std::unordered_map<std::string, std::string> headers_;
    std::string body_;
};

#endif  // REQUEST_HPP