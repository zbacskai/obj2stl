#include <CheckPoint.hpp>
#include <eigen3/Eigen/Dense>

#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <vector>

#include <list>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

#include<Exception.hpp>

#include <cmath>

namespace chp {

    struct edge3d {
        static const uint32_t edge01;
        static const uint32_t edge02;
        static const uint32_t edge12;
        int32_t _vertexIndex;
        uint32_t _qualifiedEdges;

        edge3d() : _vertexIndex(0), _qualifiedEdges(0) {};

        static uint32_t getIndexCode(int i1, int i2)
        {
            switch(i1) {
                case 0:
                    switch (i2)
                    {
                        case 1:
                            return edge3d::edge01;
                            break;
                        case 2:
                            return edge3d::edge02;
                            break;
                        default:
                            throw exception::GenericException("Invalid second edge index, with first edge index 0");
                    }
                    break;
                case 1:
                    if (i2 != 2)
                        throw exception::GenericException("Invalid edge. It must be edge 1-2");
                    return edge3d::edge12;
                    break;
                default:
                    throw exception::GenericException("invalid edge index.");
            }
        }
    };

    const uint32_t edge3d::edge01 = 0x01;
    const uint32_t edge3d::edge02 = 0x02;
    const uint32_t edge3d::edge12 = 0x04;

    struct edge3dTransformed {
        float dim[2];
        float norm[2];
        float orientation;
        float reserved;
    };

    class CheckPointImpl {
        private:
            typedef std::list<edge3d> EdgeList;
            std::vector<EdgeList> _edges;

            bool checkIfEdgeQualifes(const float dim, const int index,
                                     const trim::TriangleData& triangle,
                                     edge3d& edge,
                                     int tindex1, int tindex2)
            {
                float dimMin = std::min(triangle(tindex1, index), triangle(tindex2, index));
                float dimMax = std::max(triangle(tindex1, index), triangle(tindex2, index));
                if ( dimMin <= dim and dim <= dimMax)
                {
                    edge._qualifiedEdges |= edge.getIndexCode(tindex1, tindex2);
                    return true;
                }
                return false;
            }
                                     
            void filterOneDimension(const float dim, const int index,
                                    const trim::TriangleModel& tm)
            {
                const trim::TriangleModel::ModelMatrix &mv = tm.getVerticleMatrix();
                const trim::TriangleModel::ModelMatrix &mn = tm.getNormalMatrix();
                const std::vector<trim::TriangleData> &verticles = tm.getTriangles();
                const std::vector<trim::TriangleData> &normals = tm.getTriangleNormals();
                for (int i = 0; i < verticles.size(); ++i) { // check if triangle needs here
                    const trim::TriangleData& triangle = verticles[i];
                    edge3d e;
                    e._vertexIndex = i;
                    bool storeIt = (
                        checkIfEdgeQualifes(dim, i, triangle, e, 0, 1) or
                        checkIfEdgeQualifes(dim, i, triangle, e, 0, 2) or
                        checkIfEdgeQualifes(dim, i, triangle, e, 1, 2)
                    );
                    if (storeIt)
                        _edges[index].push_back(e);
                }
            }

            void filterEdgesByPlanes(const trim::TriangleModel& tm,
                                     const Eigen::RowVector3f& point)
            {
                for (int i = 0; i <3; ++i)
                    filterOneDimension(point(i), i, tm);
            }
        public:

            CheckPointImpl() : _edges(3) {};

            bool isInModel(const trim::TriangleModel& tm,
                           const Eigen::RowVector3f& point)
            {
                filterEdgesByPlanes(tm,point);
                return true;
            }
    };

CheckPoint::CheckPoint() : _impl(std::make_shared<CheckPointImpl>()) {};

bool CheckPoint::isInModel(const trim::TriangleModel& tm,
                           const Eigen::RowVector3f& point)
{
    return _impl->isInModel(tm, point);
}

} // end of namespace chp
