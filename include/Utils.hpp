#
#ifndef Utils_hpp
#define Utils_hpp

#include <eigen3/Eigen/Dense>
#include <string>

namespace utils
{

Eigen::RowVector3f strCoord2RowVector(const std::string& iStr);

} //end of namespace uitls

#endif