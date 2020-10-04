#
#ifndef Exception_hpp
#define Exception_hpp

#include <string>

namespace exception
{

class GenericException
{
private:
    std::string _s;
public:
    GenericException(const std::string& s) : _s(s) {};
    operator const std::string&()
    {
        return _s;
    }

};
}

#endif