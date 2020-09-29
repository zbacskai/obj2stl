#pragma once
#ifndef TriangleModel_hpp
#define TriangleModel_hpp

#include <vector>
#include <memory>
#include <algorithm>
#include <eigen3/Eigen/Dense>

namespace trim {

struct Vertex {
    float p[3];
    operator Eigen::Vector3f() const {
        Eigen::Vector3f v;
        v << p[0], p[1] , p[2];
        return v;
    }
};

class Triangle {
    private:
        Vertex vertex_[3];
        Vertex normalVector_;
        bool normalVectorSet_;
        bool minCalculated_;
        bool maxCalculated_;
        Vertex min_;
        Vertex max_;
    public: 
        const Vertex& operator[](int i) const { return vertex_[i]; };
        const Vertex& getNormalVector() const { return normalVector_; };

        Triangle(const Vertex &a, const Vertex &b, const Vertex &c) :
            vertex_({a, b, c}), normalVector_({0.0, 0.0, 0.0}), normalVectorSet_(false) ,
            minCalculated_(false), maxCalculated_(false), min_({0.0, 0.0, 0.0}), max_({0.0, 0.0, 0.0 }){};

        void setNormalVector(const Vertex& n) {
            normalVector_ = n;
            normalVectorSet_ = true;
            minCalculated_ = false;
            maxCalculated_ = false;
        }

        void setVertex(int index, const Vertex& v)
        {
            vertex_[index] = v;
        }

        void calculateNormalVector() {

        }

        const Vertex& getMin() {
            if (not minCalculated_)
            {
                for (int i = 0; i < 3; ++i)
                    min_.p[i] = std::min(vertex_[0].p[i], std::min(vertex_[1].p[i], vertex_[2].p[i]));

                minCalculated_ = true;
            }
            return min_;
        }

        const Vertex& getMax() {
            if (not maxCalculated_)
            {
                for (int i = 0; i < 3; ++i)
                    max_.p[i] = std::max(vertex_[0].p[i], std::max(vertex_[1].p[i], vertex_[2].p[i]));
                maxCalculated_ = true;
            }       
            return max_;
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
    const std::vector<std::shared_ptr<Triangle>>& getTriangles() const {
        return triangles_;
    }

    ~TriangleModel() {};
};

} //end od namespace trim
#endif