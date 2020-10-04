#ifndef Utils_hpp
#define Utils_hpp
/**
  *
  * Copyright 2020 Zoltan Bacskai
  *
  **/

#include <eigen3/Eigen/Dense>
#include <string>

namespace utils {

Eigen::RowVector3f strCoord2RowVector(const std::string& iStr);

} //end of namespace uitls

#endif