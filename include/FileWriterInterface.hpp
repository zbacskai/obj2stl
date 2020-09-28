#pragma once
#ifndef FileWriterInterface_hpp
#define FileWriterInterface_hpp

#include<TriangleModel.hpp>

namespace meshconvert {


class FileWriterInterface {
    public:
        FileWriterInterface(const char* fileName) {}
        virtual void write(const trim::TriangleModel &tm) = 0;
        virtual ~FileWriterInterface() {};
};

} // end of namespace meshconvert


#endif