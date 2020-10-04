#include <CheckPointImpl.hpp>

namespace chp {

CheckPoint::CheckPoint() : _impl(std::make_shared<CheckPointImpl>())
{

}

bool CheckPoint::isInModel(const trim::TriangleModel& tm,
                           const Eigen::RowVector3f& point)
{
    return _impl->isInModel(tm, point);
}

} // end of namespace chp
