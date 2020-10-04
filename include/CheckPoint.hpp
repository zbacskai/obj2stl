#
#ifndef CheckPoint_hpp
#define CheckPoint_hpp

#include <TriangleModel.hpp>
#include <Utils.hpp>

namespace chp {

class CheckPointImpl;

class CheckPoint {
private:
    std::shared_ptr<CheckPointImpl> _impl;
public:
    CheckPoint();
    bool isInModel(const trim::TriangleModel& tm,
                   const Eigen::RowVector3f& point);
};

}

#endif