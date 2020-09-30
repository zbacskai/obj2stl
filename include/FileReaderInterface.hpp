#pragma once
#ifndef FileReaderInterface_hpp
#define FileReaderInterface_hpp

#include<TriangleModel.hpp>

namespace meshconvert {


class FileReaderInterface {
    public:
        FileReaderInterface(const char* fileName, trim::TriangleModel &tm) {}
        virtual trim::TriangleModel& parse() = 0;
        virtual ~FileReaderInterface() {};
};

} // end of namespace meshconvert

#endif