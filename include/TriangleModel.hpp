#pragma once
#ifndef TriangleModel_hpp
#define TriangleModel_hpp

#include <vector>
#include <memory>

namespace trim {

struct Vertex {
    double p[3];
};

class Triangle {
    private:
        Vertex vertex_[3];
        Vertex normalVector_;
        bool normalVectorSet_;
    public: 
        const Vertex& operator[](int i) const { return vertex_[i]; };
        const Vertex& getNormalVector() const { return normalVector_; };

        Triangle(const Vertex &a, const Vertex &b, const Vertex &c) :
            vertex_({a, b, c}), normalVector_({0.0, 0.0, 0.0}), normalVectorSet_(false) {};

        void setNormalVector(const Vertex& n) {
            normalVector_ = n;
            normalVectorSet_ = true;
        }

        void calculateNormalVector() {

        }
};

class TriangleModel
{
private:
    std::vector<std::shared_ptr<Triangle>> triangles_;
public:
    TriangleModel(const std::vector<std::shared_ptr<Triangle>>& triangles) : triangles_(triangles) {};
    TriangleModel() {};
    void addTriangle(std::shared_ptr<Triangle> triangle);
    ~TriangleModel() {};
};

} //end od namespace trim
#endif