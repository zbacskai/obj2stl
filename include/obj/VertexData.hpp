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
        double x_;
        double y_;
        double z_;
        double w_;
    public:
        GeometricVertex(double x, double y, double z, double w) : x_(x), y_(y), z_(z), w_(w) {}
        GeometricVertex() : GeometricVertex(0.0, 0.0, 0.0, 1.0) {}

        const double getX() const;
        const double getY() const;
        const double getZ() const;
        const double getW() const;

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
        double i_;
        double j_;
        double k_;
    public:
        TextureVertex(double i, double j, double k) :  i_(i), j_(j), k_(k) {}
        TextureVertex() : TextureVertex(0.0, 0.0, 0.0) {}

        const double getI() const;
        const double getJ() const;
        const double getK() const;

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
        double u_;
        double v_;
        double w_;
    public:
        VertexNormal(double u, double v, double w) :  u_(u), v_(v), w_(w) {}
        VertexNormal() : VertexNormal(0.0, 0.0, 0.0) {}

        const double getU() const;
        const double getV() const;
        const double getW() const;

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