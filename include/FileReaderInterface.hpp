#pragma once
#ifndef FileReaderInterface_hpp
#define FileReaderInterface_hpp

#include<TriangleModel.hpp>

namespace meshconvert {


class FileReaderInterface {
    public:
        FileReaderInterface(const char* fileName) {}
        virtual void parse() = 0;
        virtual trim::TriangleModel& convertToTriangleModel(trim::TriangleModel &tm) = 0;
        virtual ~FileReaderInterface() {};
};

} // end of namespace meshconvert

#endif