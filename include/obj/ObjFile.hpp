#pragma once
#ifndef ObjFile_hpp
#define ObjFile_hpp

#include <string>
#include <vector>
#include <memory>

#include<FileReaderInterface.hpp>

namespace obj {

class GeometricVertex;
class TextureVertex;
class VertexNormal;
class Surface;

class ObjFile : public meshconvert::FileReaderInterface {
    private:
        std::string fileName_;
        int vCount_;
        int vtCount_;
        int vnCount_;
        std::vector<std::shared_ptr<GeometricVertex>> v_;
        std::vector<std::shared_ptr<TextureVertex>> vt_;
        std::vector<std::shared_ptr<VertexNormal>> vn_;
        std::vector<std::shared_ptr<Surface>> s_;

        void addSimpleTriangle(trim::TriangleModel &tm, const Surface& s);
        void add2Triangle(trim::TriangleModel &tm, const Surface& s);
        void addMultiTriangle(trim::TriangleModel &tm, const Surface& s);
    public:
        ObjFile(const char* fileName);
        virtual void parse();
        virtual trim::TriangleModel& convertToTriangleModel(trim::TriangleModel &tm);
        virtual ~ObjFile() {};
};

}

#endif