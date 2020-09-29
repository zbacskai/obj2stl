#pragma once
#ifndef VertexData_hpp
#define vertexData_hpp

#include <iostream>
#include <iomanip>
#include <vector>

#include<TriangleModel.hpp>

namespace obj {

class ObjVertexBase {
    protected:
        trim::Vertex v_;
    public:
        ObjVertexBase(float x, float y, float z) : v_({x, y, z}) {};
        operator const trim::Vertex&() const {
            return v_;
        }
};

class GeometricVertex : public ObjVertexBase{
    private:
        float w_;
    public:
        GeometricVertex(float x, float y, float z, float w) : ObjVertexBase(x,y,z), w_(w) {}
        GeometricVertex() : GeometricVertex(0.0, 0.0, 0.0, 1.0) {}
        friend std::istream &operator>>( std::istream  &input, GeometricVertex &v ); 
};

class TextureVertex : public ObjVertexBase {
    public:
        TextureVertex(float i, float j, float k) :  ObjVertexBase(i,j,k) {};
        TextureVertex() : TextureVertex(0.0, 0.0, 0.0) {}

        friend std::istream &operator>>( std::istream  &input, TextureVertex &v ); 
};

class VertexNormal : public ObjVertexBase {
    public:
        VertexNormal(float u, float v, float w) :  ObjVertexBase(u, v, w) {}
        VertexNormal() : VertexNormal(0.0, 0.0, 0.0) {}

        friend std::istream &operator>>( std::istream  &input, VertexNormal &v ); 
};

class Surface {
    private:
        std::vector<int> vRefs_;
        std::vector<int> tvRefs_;
        std::vector<int> vnRefs_;
    public:
        Surface() {};
        const std::vector<int>& getVRefs() const;
        const std::vector<int>& getTvRefs() const;
        const std::vector<int>& getVnRefs() const;

        friend std::istream &operator>>( std::istream  &input, Surface &s ); 

        void makeRefsAbsolute(int vCount, int tvCount, int vnCount);

        void debug() {
            std::cout << "surface " << std::endl;
            for (unsigned int i=0; i < vRefs_.size(); ++i)
            {
                std::cout << vRefs_[i] << '/'
                          << tvRefs_[i] << '/'
                          << vnRefs_[i] << std::endl;
            }
        }
};

}
#endif