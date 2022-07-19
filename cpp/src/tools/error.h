#include <exception>
#include <string>

namespace bch {
class Error : public std::exception {
  public:
    Error(std::string const &str) : msg(str)
    {
    }

    char const *what() const noexcept
    {
        return msg.c_str();
    }

  private:
    std::string msg;
};

class IOError : public Error {
    using Error::Error;
};

} // namespace bch
