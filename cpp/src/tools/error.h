#ifndef BCH_ERROR_H_INCLUDED
#define BCH_ERROR_H_INCLUDED

#include <exception>
#include <string>

namespace bch {

class Error : public std::exception {
  public:
    Error(char const *str) : msg(str)
    {
    }

    char const *what() const noexcept
    {
        return msg;
    }

  private:
    char const *msg;
};

class IOError : public Error {
    using Error::Error;
};

} // namespace bch

#endif
