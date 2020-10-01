#include <CheckPoint.hpp>
#include <eigen3/Eigen/Dense>

#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <set>

#include <list>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include <cmath>

namespace chp {

bool CheckPoint::isInModel(const trim::TriangleModel& tm,
                           const Eigen::RowVector3f& point)
{
    return false;
}

} // end of namespace chp
