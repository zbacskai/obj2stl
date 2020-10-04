#
#ifndef FileReaderInterface_hpp
#define FileReaderInterface_hpp

#include<TriangleModel.hpp>

namespace meshconvert {


class FileReaderInterface {
    public:
        FileReaderInterface(const char*) {}
        virtual trim::TriangleModel& parse(trim::TriangleModel &tm) = 0;
        virtual ~FileReaderInterface() {};
};

} // end of namespace meshconvert

#endif