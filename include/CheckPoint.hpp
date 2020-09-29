#pragma once
#ifndef CheckPoint_hpp
#define CheckPoint_hpp

#include <TriangleModel.hpp>

namespace chp {

class CheckPoint {
    private:
        trim::Vertex point_;
    public:
        CheckPoint(float x, float y, float z) : point_({x, y, z}) {};
        bool isInModel(const trim::TriangleModel& tm);
};

}

#endif