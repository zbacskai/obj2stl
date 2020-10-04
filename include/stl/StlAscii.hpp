#ifndef StlAscii_hpp
#define StlAscii_hpp
/**
  *
  * Copyright 2020 Zoltan Bacskai
  * 
  **/

#include <FileWriterInterface.hpp>
#include <string>

namespace stl {
namespace ascii {
    
class StlAscii : public meshconvert::FileWriterInterface
{
    private:
        std::string fileName_;
    public:
        StlAscii(const char* fileName);
        virtual void write(const trim::TriangleModel &tm);
        virtual ~StlAscii() {};
};

} // end of namespace ascii
} // end of namespace stl


#endif