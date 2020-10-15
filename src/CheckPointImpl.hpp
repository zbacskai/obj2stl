/**
  *
  * Copyright 2020 Zoltan Bacskai
  * 
  **/
#ifndef CheckPointImpl_hpp
#define CheckPointImpl_hpp
#include <CheckPoint.hpp>
#include <TriangleModel.hpp>

#include <eigen3/Eigen/Dense>

#include <list>
#include <map>
#include <set>
#include <vector>


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

struct PointCmp {
    bool operator ()(const point2d &a, const point2d &b) const;
};


struct edge2d {
    std::set<point2d, PointCmp> p;
    point2d nc;
};

struct edge2dNice {
    point2d p[2];
    float directionOfFace;

    void print() const;
};

struct Polygon {
    int _poligonIndex;
    std::vector<edge2dNice> _edges;

    bool pointInPolygon(float x, float y) const;
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

    void fillMatrixes(const std::list<edge3d> &edges, const trim::TriangleModel& tm);
    void add2Edge(const Eigen::Vector3f &point,
                  const Eigen::Vector3f &normal, int edgeId);

    void calculate2dEdges();
    void convertEdges2NicerForm();

    void projectEdges(const std::list<edge3d> &edges, const trim::TriangleModel& tm);
    std::map<point2d, int, PointCmp> initialisePointMap();
    void precalcPointsToPoligins(std::map<point2d, int, PointCmp> &point2poligon);
    std::map<int, Polygon> clusterPointsToFinalPoligons(std::map<point2d, int, PointCmp> &point2poligon);
    void debugPolygons(const std::map<int, Polygon> &poligons) const;
    std::map<int, Polygon> calculatePolygons();
public:
    PolygonMaster(unsigned int amountOfEdges,
                  int projectedPlaneIndex,
                  float planeCoordinate);

    std::map<int, Polygon> getPolygons(const std::list<edge3d> &edges, const trim::TriangleModel& tm);
    bool checkPointInPoligon(const Eigen::RowVector3f& point, const Polygon &poly) const;
}; // end of PolygonMaster

class CheckPointImpl {
private:
    typedef std::list<edge3d> EdgeList;
    std::vector<EdgeList> _edges;

    void checkIfEdgeCrossesPlane(const float pointXYZCoord,
                                 const int dimIndex,
                                 const trim::TriangleData& triangle,
                                 const trim::TriangleData& normals,
                                 const int triangleIndex,
                                 int triangleVertex1Index, int triangleVertex2Index);

    void filterTriangleFacesByProjectionPlane(const Eigen::RowVector3f& point,
            const int dimIndex,
            const trim::TriangleModel& tm);

    bool isPointIn2dProjection(const Eigen::RowVector3f& point,
                               int dimensionIndex,
                               const trim::TriangleModel& tm) const;

public:

    CheckPointImpl();

    bool isInModel(const trim::TriangleModel& tm,
                   const Eigen::RowVector3f& point);
};

} // end of namespace chp
#endif
