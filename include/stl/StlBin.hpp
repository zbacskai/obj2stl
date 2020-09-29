#pragma once
#ifndef StlBin_hpp
#define StlBin_hpp

#include <FileWriterInterface.hpp>
#include <string>

namespace stl {
namespace bin {
    
class StlBin : public meshconvert::FileWriterInterface
{
    private:
        std::string fileName_;
    public:
        StlBin(const char* fileName);
        virtual void write(const trim::TriangleModel &tm);
        virtual ~StlBin() {};
};

} // end of namespace bin
} // end of namespace stl


#endif