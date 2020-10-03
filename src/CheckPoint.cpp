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
#include <set>

#include <TriangleModel.hpp>

namespace chp {

    struct edge3d {
        trim::VertexRef _e[2];
        trim::VertexRef _n[2];
        int index_;
    };

    struct point2d {
        float x;
        float y;
    };

    struct PointCmp{
        bool operator ()(const point2d &a, const point2d &b)
        {
            if (a.x < b.x)
                return true;

            return (a.x == b.x and a.y < b.y);
        }
    };


    struct edge2d {
        std::set<point2d, PointCmp> p;
        point2d nc;
    };

    struct edge2dNice {
        point2d p[2];
        float directionOfFace;

        void print() const {
            std::cout << directionOfFace << " " << p[0].x << "," << p[0].y << " " << p[1].x << "," << p[1].y << std::endl;
        }
    };

    struct Polygon {
        int _poligonIndex;
        std::vector<edge2dNice> _edges;
    };

    class PolygonMaster {
        private:
            unsigned int _amountOfEdges;
            int _projectedPlaneIndex;
            float _planeCoordinate;
            std::vector<int> _indexes;            
            Eigen::Matrix<float, 1, Eigen::Dynamic>  _determinants;
            Eigen::Matrix<float, 3, Eigen::Dynamic> _ia;
            Eigen::Matrix<float, 3, Eigen::Dynamic> _ib;
            Eigen::Matrix<float, 3, Eigen::Dynamic> _ia_b;
            Eigen::Matrix<float, 3, Eigen::Dynamic> _na;
            Eigen::Matrix<float, 3, Eigen::Dynamic> _nb;
            Eigen::RowVector3f _p01xp02;
            Eigen::Matrix3f _plane;
            std::map<int, edge2d> _2dEdges;
            std::vector<edge2dNice> _2dedgesNice;

            static const int dimMapX[3];
            static const int dimMapY[3];
        public:
            PolygonMaster(unsigned int amountOfEdges,
                         int projectedPlaneIndex,
                         float planeCoordinate) : 
                _amountOfEdges(amountOfEdges), 
                _projectedPlaneIndex(projectedPlaneIndex),
                _planeCoordinate(planeCoordinate),
                _indexes(amountOfEdges),
                _determinants(amountOfEdges),
                _ia(3, amountOfEdges),
                _ib(3, amountOfEdges), _ia_b(3, amountOfEdges),
                _na(3, amountOfEdges), _nb(3, amountOfEdges),
                _p01xp02()
            {
                float p = planeCoordinate;
                switch(projectedPlaneIndex)
                {
                    case 0: // X
                        _plane <<   p,   p,   p,
                                  0.0, 1.0, 0.0,
                                  0.0, 0.0, 1.0;
                        break;
                    case 1: // y
                        _plane << 0.0, 1.0, 0.0,
                                    p,   p,   p,
                                  0.0, 0.0, 1.0; 
                        break;
                    case 2: //z
                        _plane << 0.0, 1.0, 0.0,
                                  0.0, 0.0, 1.0,
                                    p,   p,   p;
                        break;
                    default:
                        break;
                }
            }

            void fillMatrixes(const std::list<edge3d> &edges, const trim::TriangleModel& tm)
            {
                unsigned int columnNumber = 0;
                const trim::TriangleModel::ModelMatrix &mm = tm.getVerticleMatrix();
                const trim::TriangleModel::ModelMatrix &nm = tm.getNormalMatrix();
                for (auto& edge: edges)
                {
                    _indexes[columnNumber] = edge.index_;
                    for (int i = 0; i < 3; ++i)
                    {
                        _ia(i, columnNumber) = mm(edge._e[0], i);
                        _ib(i, columnNumber) = mm(edge._e[1], i);
                        _na(i, columnNumber) = nm(edge._n[0], i);
                        _nb(i, columnNumber) = nm(edge._n[1], i);
                    }
                    ++columnNumber;
                }
                _ia_b = _ib - _ia;
                Eigen::Vector3f p01 = _plane.col(1) - _plane.col(0);
                Eigen::Vector3f p02 = _plane.col(2) - _plane.col(0);

                _p01xp02 = p01.cross(p02);
                _determinants = _p01xp02 * (_ia_b * -1.0);
            }

            void add2Edge(const Eigen::Vector3f &point, 
                          const Eigen::Vector3f &normal, int edgeId)
            {
                if (_2dEdges.find(edgeId) == _2dEdges.end())
                    _2dEdges.insert(std::pair<int, edge2d>(edgeId, edge2d()));

                edge2d& edge = _2dEdges[edgeId];
                if (edge.p.size() == 0)
                {
                    edge.nc.x = normal(dimMapX[_projectedPlaneIndex]);
                    edge.nc.y = normal(dimMapY[_projectedPlaneIndex]);
                }
                point2d p;
                p.x = point(dimMapX[_projectedPlaneIndex]);
                p.y = point(dimMapY[_projectedPlaneIndex]);
                edge.p.insert(p);
            }

            void calculate2dEdges()
            {
                for (unsigned int colIndex = 0; colIndex < _amountOfEdges; ++colIndex)
                {
                    float det = _determinants(0, colIndex);
                    if (det != 0.0) // shall never be in our case as we prefilter...
                    {
                        float t = _p01xp02 * (_ia.col(colIndex) - _plane.col(0));
                        t/=det;
                        Eigen::Vector3f point = _ia.col(colIndex) + (_ia_b.col(colIndex)*t);
                        Eigen::Vector3f normal = _na.col(colIndex);
                        int edgeId = _indexes[colIndex];
                        add2Edge(point, normal, edgeId);
                    }
                }
            }

            void massageEdges() {
                for (auto& edge : _2dEdges)
                {
                    edge2dNice en;
                    unsigned int i = 0;
                    // add the points in order x < y.
                    for (auto& pnts : edge.second.p)
                    {
                        en.p[i] = pnts;
                        ++i;
                    }
                    //Calculate a direction :-)
                    float carthesian = (en.p[1].x - en.p[0].x)*(edge.second.nc.y - en.p[0].y) - (en.p[1].y - en.p[0].y) * (edge.second.nc.x -en.p[0].x);
                    en.directionOfFace =  carthesian > 0 ? 1.0 :
                                                           (carthesian == 0 ? 0.0 : -1.0);
                    _2dedgesNice.push_back(en);
                }
            }

            void projectEdges(const std::list<edge3d> &edges, const trim::TriangleModel& tm)
            {
                fillMatrixes(edges, tm);
                calculate2dEdges();
                massageEdges();
            }

            void calculatePolygons() {
                std::vector<Polygon> poligons;
                std::map<point2d, int, PointCmp> point2poligon; 
                bool mergeDone = true;
                for (auto& edge : _2dedgesNice)
                {
                    const auto itA = point2poligon.find(edge.p[0]);
                    const auto itB = point2poligon.find(edge.p[1]);

                    if (itA == point2poligon.end() and
                        itB == point2poligon.end())
                    {
                        point2poligon.insert(std::pair<point2d, int>(edge.p[0], poligons.size()));
                        point2poligon.insert(std::pair<point2d, int>(edge.p[1], poligons.size()));
                        Polygon poly;
                        poly._poligonIndex = poligons.size();
                        poly._edges.push_back(edge);
                        poligons.push_back(poly);
                    }
                    else if (itA == point2poligon.end())
                    {
                        int polyIndexB = itB->second;
                        point2poligon.insert(std::pair<point2d, int>(edge.p[0], polyIndexB));
                        poligons[polyIndexB]._edges.push_back(edge);
                    }
                    else if (itB == point2poligon.end())
                    {
                        int polyIndexA = itA->second;
                        point2poligon.insert(std::pair<point2d, int>(edge.p[1], polyIndexA));
                        poligons[polyIndexA]._edges.push_back(edge);
                    }
                    else
                    {   
                        int polyIndexA = itA->second;
                        int polyIndexB = itB->second;
                        if (polyIndexA == polyIndexB) // same poligon just push edge
                            poligons[polyIndexA]._edges.push_back(edge);
                        else
                        {
                            //merge poligons
                            Polygon &pA = poligons[polyIndexA];
                            Polygon &pB = poligons[polyIndexB];
                            for (auto& edge : pB._edges)
                                pA._edges.push_back(edge);
                            pB._edges.clear();
                            std::list<point2d> pu;
                            for (auto& p2p : point2poligon)
                            {
                                pu.push_back(p2p.first);
                            }
                            for (auto& p : pu)
                                point2poligon[p] = polyIndexA;
                        }
                    }
                }

                for (auto &poly : poligons)
                {
                    std::cout << "Polygon: " << poly._poligonIndex << " <> " << _projectedPlaneIndex << std::endl;
                    std::cout << "------------------------------------------------" << std::endl;
                    for (auto& pnts : poly._edges)
                        pnts.print();
                }
            }

            void getPolygons(const std::list<edge3d> &edges, const trim::TriangleModel& tm)
            {
                projectEdges(edges, tm);
                calculatePolygons();
            }
    };

    const int PolygonMaster::dimMapX[3] = {1, 0, 0};
    const int PolygonMaster::dimMapY[3] = {2, 2, 1};

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
                {
                    _edges[dimIndex].push_back({triangle(tindex1), triangle(tindex2), 
                                                normals(tindex1), normals(tindex2), triangleIndex});
                }
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

            void calculate2DPolygons(const Eigen::RowVector3f& point, int dimensionIndex, const trim::TriangleModel& tm)
            {
                unsigned int numberOfEdges = _edges[dimensionIndex].size();
                if (numberOfEdges > 0)
                {
                    PolygonMaster p(_edges[dimensionIndex].size(), dimensionIndex, point(dimensionIndex));
                    p.getPolygons(_edges[dimensionIndex], tm);
                }
            }

            void filterEdgesByPlanes(const trim::TriangleModel& tm,
                                     const Eigen::RowVector3f& point)
            {
                for (int i = 0; i < 3; ++i)
                    filterOneDimension(point, i, tm);

                for (int i = 0; i < 3; ++i)
                    calculate2DPolygons(point, i, tm);
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
