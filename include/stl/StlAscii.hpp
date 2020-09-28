#pragma once
#ifndef StlAscii_hpp
#define StlAscii_hpp

#include<TriangleModel.hpp>

namespace stl {
namespace ascii {
    
class StlAscii
{
    public:
        static void write_stdout(const trim::TriangleModel &tm);
};

} // end of namespace ascii
} // end of namespace stl


#endif