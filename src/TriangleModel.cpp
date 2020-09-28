#include <TriangleModel.hpp>

namespace trim {

void TriangleModel::addTriangle(std::shared_ptr<Triangle> triangle)
{
    triangles_.push_back(triangle);
}

}// end of namespace trim