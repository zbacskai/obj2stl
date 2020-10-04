#include <Utils.hpp>

namespace utils {

Eigen::RowVector3f strCoord2RowVector(const std::string& iStr)
{
    std::stringstream param_desc(iStr);
    std::string param;
    Eigen::RowVector3f pl;

    for (unsigned i = 0; std::getline(param_desc, param, ',') and (i < 3); ++i)
        pl(i) = std::atof(param.c_str());

    return pl;
}

}