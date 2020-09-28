#pragma once
#ifndef ObjFile_hpp
#define ObjFile_hpp

#include <string>
#include <vector>
#include <memory>

namespace obj {

class GeometricVertex;
class TextureVertex;
class VertexNormal;
class Surface;

class ObjFile {
    private:
        std::string fileName_;
        int vCount_;
        int vtCount_;
        int vnCount_;
        std::vector<std::shared_ptr<GeometricVertex>> v_;
        std::vector<std::shared_ptr<TextureVertex>> vt_;
        std::vector<std::shared_ptr<VertexNormal>> vn_;
        std::vector<std::shared_ptr<Surface>> s_;
    public:
        ObjFile(const char* fileName);
        void parse();
};

}

#endif