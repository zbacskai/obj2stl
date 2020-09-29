#pragma once
#ifndef VertexData_hpp
#define vertexData_hpp

#include <iostream>
#include <iomanip>
#include <vector>

#include<TriangleModel.hpp>

namespace obj {

class GeometricVertex {
    private:
        float x_;
        float y_;
        float z_;
        float w_;
    public:
        GeometricVertex(float x, float y, float z, float w) : x_(x), y_(y), z_(z), w_(w) {}
        GeometricVertex() : GeometricVertex(0.0, 0.0, 0.0, 1.0) {}

        const float getX() const;
        const float getY() const;
        const float getZ() const;
        const float getW() const;

        friend std::istream &operator>>( std::istream  &input, GeometricVertex &v ); 
        void debug() {
            std::cout << std::fixed;
            std::cout << std::setprecision(4);
            std::cout << "geometric-vertex "
                      << x_ << ","
                      << y_ << ","
                      << z_ << ","
                      << w_ << std::endl;
        }

        trim::Vertex getVertex() const {
            return {x_, y_, z_};
        }
};

class TextureVertex {
    private:
        float i_;
        float j_;
        float k_;
    public:
        TextureVertex(float i, float j, float k) :  i_(i), j_(j), k_(k) {}
        TextureVertex() : TextureVertex(0.0, 0.0, 0.0) {}

        const float getI() const;
        const float getJ() const;
        const float getK() const;

        friend std::istream &operator>>( std::istream  &input, TextureVertex &v ); 
        void debug() {
            std::cout << std::fixed;
            std::cout << std::setprecision(4);
            std::cout << "texture-vertex "
                      << i_ << ","
                      << j_ << ","
                      << k_ << std::endl;
        }

        trim::Vertex getVertex() const {
            return {i_, j_, k_};
        }
};

class VertexNormal {
    private:
        float u_;
        float v_;
        float w_;
    public:
        VertexNormal(float u, float v, float w) :  u_(u), v_(v), w_(w) {}
        VertexNormal() : VertexNormal(0.0, 0.0, 0.0) {}

        const float getU() const;
        const float getV() const;
        const float getW() const;

        friend std::istream &operator>>( std::istream  &input, VertexNormal &v ); 
        void debug() {
            std::cout << std::fixed;
            std::cout << std::setprecision(4);
            std::cout << "vertex-normal "
                      << u_ << ","
                      << v_ << ","
                      << w_ << std::endl;
        }

        trim::Vertex getVertex() const {
            return {u_, v_, w_};
        }
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