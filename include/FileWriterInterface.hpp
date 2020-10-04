#ifndef FileWriterInterface_hpp
#define FileWriterInterface_hpp
/**
  *
  * Copyright 2020 Zoltan Bacskai
  *
  **/

#include<TriangleModel.hpp>

namespace meshconvert {


class FileWriterInterface {
public:
    FileWriterInterface(const char*) {}
    virtual void write(const trim::TriangleModel &tm) = 0;
    virtual ~FileWriterInterface() {};
};

} // end of namespace meshconvert


#endif