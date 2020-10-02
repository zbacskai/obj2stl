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

#include <TriangleModel.hpp>

namespace chp {

    struct edge3d {
        trim::VertexRef _e[2];
        trim::VertexRef _n[2];
        int index_;
    };

    struct Polygon {

    };

    class PolygonMaster {
        private:
            unsigned int _amountOfEdges;
            int _projectedPlaneIndex;
            float _planeCoordinate;
            Eigen::Matrix<float, 3, Eigen::Dynamic> _ia;
            Eigen::Matrix<float, 3, Eigen::Dynamic> _ib;
            Eigen::Matrix<float, 3, Eigen::Dynamic> _ib_a;
            Eigen::Matrix3f _plane;
        public:
            PolygonMaster(unsigned int amountOfEdges,
                         int projectedPlaneIndex,
                         float planeCoordinate) : 
                _amountOfEdges(amountOfEdges), 
                _projectedPlaneIndex(projectedPlaneIndex),
                _planeCoordinate(planeCoordinate),
                _ia(3, amountOfEdges),
                _ib(3, amountOfEdges), _ib_a(3, amountOfEdges)
            {
                float p = planeCoordinate;
                switch(projectedPlaneIndex)
                {
                    case 0: // X
                        _plane <<   p,   p,   p,
                                  0.0, 1.0, 0.0,
                                  0.0, 0.0, 1.0;
                        break;
                    case 1:
                        _plane << 0.0, 1.0, 0.0,
                                    p,   p,   p,
                                  0.0, 0.0, 1.0; 
                        break;
                    case 2:
                        _plane << 0.0, 1.0, 0.0,
                                  0.0, 0.0, 1.0,
                                    p,   p,   p;
                        break;
                    default:
                        break;
                }
            }
    };

    class CheckPointImpl {
        private:
            typedef std::list<edge3d> EdgeList;
            std::vector<EdgeList> _edges;
            typedef std::list<Polygon> PolygonList;
            std::vector<PolygonList> _polygons;

            void checkIfEdgeQualifes(const float pointXYZCoord, 
                                     const int dimIndex,
                                     const trim::TriangleData& triangle,
                                     const trim::TriangleData& normals,
                                     const int triangleIndex,
                                     int tindex1, int tindex2)
            {
                float pointXYZMin = std::min(triangle(tindex1, dimIndex), triangle(tindex2, dimIndex));
                float pointXYZMax= std::max(triangle(tindex1, dimIndex), triangle(tindex2, dimIndex));
                if ( pointXYZMin <= pointXYZCoord and pointXYZCoord <= pointXYZMax)
                    _edges[dimIndex].push_back({triangle(tindex1), triangle(tindex2), 
                                                normals(tindex1), normals(tindex2), triangleIndex});
            }
                                     
            void filterOneDimension(const Eigen::RowVector3f& point,
                                    const int dimIndex,
                                    const trim::TriangleModel& tm)
            {
                const trim::TriangleModel::ModelMatrix &mv = tm.getVerticleMatrix();
                const trim::TriangleModel::ModelMatrix &mn = tm.getNormalMatrix();
                const std::vector<trim::TriangleData> &verticles = tm.getTriangles();
                const std::vector<trim::TriangleData> &normals = tm.getTriangleNormals();
                for (int triangleIndex = 0; triangleIndex < verticles.size(); ++triangleIndex) { // check if triangle needs here
                    const trim::TriangleData& triangle = verticles[triangleIndex];
                    const trim::TriangleData& normal = normals[triangleIndex];
                    
                    checkIfEdgeQualifes(point(dimIndex), dimIndex, triangle, normal, triangleIndex, 0, 1);
                    checkIfEdgeQualifes(point(dimIndex), dimIndex, triangle, normal, triangleIndex, 0, 2);
                    checkIfEdgeQualifes(point(dimIndex), dimIndex, triangle, normal, triangleIndex, 1, 2);
                }
            }

            void calculate2DPolygons(const Eigen::RowVector3f& point, int dimensionIndex)
            {
                unsigned int numberOfEdges = _edges[dimensionIndex].size();
                PolygonMaster p(_edges[dimensionIndex].size(), dimensionIndex, point(dimensionIndex));
            }

            void filterEdgesByPlanes(const trim::TriangleModel& tm,
                                     const Eigen::RowVector3f& point)
            {
                for (int i = 0; i < 3; ++i)
                    filterOneDimension(point, i, tm);

                for (int i = 0; i < 3; ++i)
                    calculate2DPolygons(point, i);
            }
        public:

            CheckPointImpl() : _edges(3) {
            };

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
