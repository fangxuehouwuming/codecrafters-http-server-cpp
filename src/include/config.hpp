#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

class Config {
   public:
    Config(int argc, char** argv);
    const std::string& GetDirectory() const;

   private:
    std::string directory_ = "";
};

#endif  // CONFIG_HPP