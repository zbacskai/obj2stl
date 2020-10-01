#pragma once
#ifndef CheckPoint_hpp
#define CheckPoint_hpp

#include <TriangleModel.hpp>
#include <Utils.hpp>

namespace chp {

class CheckPoint {
    public:
        bool isInModel(const trim::TriangleModel& tm,
                       const Eigen::RowVector3f& point);
};

}

#endif