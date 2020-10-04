#include<CheckPointImpl.hpp>
#include <Logging.hpp>

namespace chp {

bool PointCmp::operator ()(const point2d &a, const point2d &b)
{
    if (a.x < b.x)
        return true;

    return (a.x == b.x and a.y < b.y);
}

void edge2dNice::print() const
{
    debug(std::setprecision(12) << directionOfFace << " " << p[0].x << "," << p[0].y << " " << p[1].x << "," << p[1].y);
}

bool Polygon::pointInPolygon(float x, float y) const
{
    bool retVal = false;
    for (auto& e : _edges) {
        float carthesian = (e.p[1].x - e.p[0].x)*(y - e.p[0].y) - (e.p[1].y - e.p[0].y) * (x -e.p[0].x);
        bool inside = (carthesian != e.directionOfFace);
        debug(( inside ? "I " : "] O ") << x << "," << y);
        retVal |= inside;
    }
    return retVal;
}

PolygonMaster::PolygonMaster(
    unsigned int amountOfEdges, int projectedPlaneIndex, float planeCoordinate) :
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
    switch(projectedPlaneIndex) {
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

void PolygonMaster::fillMatrixes(const std::list<edge3d> &edges, const trim::TriangleModel& tm)
{
    unsigned int columnNumber = 0;
    const trim::TriangleModel::ModelMatrix &mm = tm.getVerticleMatrix();
    const trim::TriangleModel::ModelMatrix &nm = tm.getNormalMatrix();
    for (auto& edge: edges) {
        _indexes[columnNumber] = edge.index_;
        for (int i = 0; i < 3; ++i) {
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

void PolygonMaster::add2Edge(const Eigen::Vector3f &point,
                             const Eigen::Vector3f &normal, int edgeId)
{
    if (_2dEdges.find(edgeId) == _2dEdges.end())
        _2dEdges.insert(std::pair<int, edge2d>(edgeId, edge2d()));

    edge2d& edge = _2dEdges[edgeId];
    if (edge.p.size() == 0) {
        edge.nc.x = normal(dimMapX[_projectedPlaneIndex]);
        edge.nc.y = normal(dimMapY[_projectedPlaneIndex]);
    }
    point2d p;
    p.x = point(dimMapX[_projectedPlaneIndex]);
    p.y = point(dimMapY[_projectedPlaneIndex]);
    edge.p.insert(p);
}

void PolygonMaster::calculate2dEdges()
{
    for (unsigned int colIndex = 0; colIndex < _amountOfEdges; ++colIndex) {
        float det = _determinants(0, colIndex);
        if (det != 0.0) { // shall never be in our case as we prefilter...
            float t = _p01xp02 * (_ia.col(colIndex) - _plane.col(0));
            t/=det;
            Eigen::Vector3f point = _ia.col(colIndex) + (_ia_b.col(colIndex)*t);
            Eigen::Vector3f normal = _na.col(colIndex);
            int edgeId = _indexes[colIndex];
            add2Edge(point, normal, edgeId);
        }
    }
}

void PolygonMaster::convertEdges2NicerForm()
{
    for (auto& edge : _2dEdges) {
        edge2dNice en;
        unsigned int i = 0;
        // add the points in order x < y.
        for (auto& pnts : edge.second.p) {
            en.p[i] = pnts;
            ++i;
        }
        //Calculate a direction
        float carthesian = (en.p[1].x - en.p[0].x)*edge.second.nc.y - (en.p[1].y - en.p[0].y) * edge.second.nc.x;
        en.directionOfFace =  carthesian > 0 ? 1.0 :
                              (carthesian == 0 ? 0.0 : -1.0);
        _2dedgesNice.push_back(en);
    }
}

void PolygonMaster::projectEdges(const std::list<edge3d> &edges, const trim::TriangleModel& tm)
{
    fillMatrixes(edges, tm);
    calculate2dEdges();
    convertEdges2NicerForm();
}


std::map<point2d, int, PointCmp> PolygonMaster::initialisePointMap()
{
    std::map<point2d, int, PointCmp> point2poligon;
    for (auto& edge : _2dedgesNice) {
        point2poligon.insert(std::pair<point2d, int>(edge.p[0], -1));
        point2poligon.insert(std::pair<point2d, int>(edge.p[1], -1));
    }
    return point2poligon;
}

void PolygonMaster::precalcPointsToPoligins(std::map<point2d, int, PointCmp> &point2poligon)
{
    unsigned int pIndex = 0;
    for (auto& edge : _2dedgesNice) {
        int p1 = point2poligon[edge.p[0]];
        int p2 = point2poligon[edge.p[1]];
        int pMax = std::max(p1, p2);
        if (pMax >=0) {
            point2poligon[edge.p[0]] = pMax;
            point2poligon[edge.p[1]] = pMax;
        }
        else {
            point2poligon[edge.p[0]] = pIndex;
            point2poligon[edge.p[1]] = pIndex;
            pIndex++;
        }
    }
}

std::map<int, Polygon> PolygonMaster::clusterPointsToFinalPoligons(std::map<point2d, int, PointCmp> &point2poligon)
{
    std::map<int, Polygon> poligons;
    bool mergeDone = true;
    while(mergeDone) {
        mergeDone = false;
        for (auto& edge : _2dedgesNice) {
            int pi1 = point2poligon[edge.p[0]];
            int pi2 = point2poligon[edge.p[1]];
            if (pi1 != pi2) {
                mergeDone = true;
                int pMax = std::max(pi1, pi2);
                point2poligon[edge.p[0]] = pMax;
                point2poligon[edge.p[1]] = pMax;
            }
        }
    }

    for (auto& edge : _2dedgesNice) {
        int key = point2poligon[edge.p[0]];
        poligons[key]._edges.push_back(edge);
    }

    return poligons;
}

void PolygonMaster::debugPolygons(const std::map<int, Polygon> &poligons) const
{
    for (auto &poly : poligons) {
        debug("Polygon: " << poly.first << " <> " << _projectedPlaneIndex);
        debug("------------------------------------------------");
        for (auto& pnts : poly.second._edges)
            pnts.print();
    }
}

std::map<int, Polygon> PolygonMaster::calculatePolygons()
{
    std::map<point2d, int, PointCmp> &&point2poligon = initialisePointMap();
    precalcPointsToPoligins(point2poligon);
    std::map<int, Polygon> &&poligons = clusterPointsToFinalPoligons(point2poligon);
    debugPolygons(poligons);
    return std::move(poligons);
}

std::map<int, Polygon> PolygonMaster::getPolygons(const std::list<edge3d> &edges, const trim::TriangleModel& tm)
{
    projectEdges(edges, tm);
    return std::move(calculatePolygons());
}

bool PolygonMaster::checkPointInPoligon(const Eigen::RowVector3f& point, const Polygon &poly) const
{
    float x = point(dimMapX[_projectedPlaneIndex]);
    float y = point(dimMapY[_projectedPlaneIndex]);
    return poly.pointInPolygon(x, y);
}


const int PolygonMaster::dimMapX[3] = {1, 0, 0};
const int PolygonMaster::dimMapY[3] = {2, 2, 1};

void CheckPointImpl::checkIfEdgeCrossesPlane(const float pointXYZCoord,
        const int dimIndex,
        const trim::TriangleData& triangle,
        const trim::TriangleData& normals,
        const int triangleIndex,
        int triangleVertex1Index, int triangleVertex2Index)
{
    float pointXYZMin = std::min(triangle(triangleVertex1Index, dimIndex),
                                 triangle(triangleVertex2Index, dimIndex));
    float pointXYZMax= std::max(triangle(triangleVertex1Index, dimIndex),
                                triangle(triangleVertex2Index, dimIndex));

    if ( pointXYZMin <= pointXYZCoord and pointXYZCoord <= pointXYZMax) {
        _edges[dimIndex].push_back({triangle(triangleVertex1Index),
                                    triangle(triangleVertex2Index),
                                    normals(triangleVertex1Index),
                                    normals(triangleVertex2Index),
                                    triangleIndex});
    }
}

void CheckPointImpl::filterTriangleFacesByProjectionPlane(const Eigen::RowVector3f& point,
        const int dimIndex,
        const trim::TriangleModel& tm)
{
    const std::vector<trim::TriangleData> &verticles = tm.getTriangles();
    const std::vector<trim::TriangleData> &normals = tm.getTriangleNormals();

    for (unsigned int triangleIndex = 0; triangleIndex < verticles.size(); ++triangleIndex) {
        const trim::TriangleData& triangle = verticles[triangleIndex];
        const trim::TriangleData& normal = normals[triangleIndex];

        checkIfEdgeCrossesPlane(point(dimIndex), dimIndex, triangle, normal, triangleIndex, 0, 1);
        checkIfEdgeCrossesPlane(point(dimIndex), dimIndex, triangle, normal, triangleIndex, 0, 2);
        checkIfEdgeCrossesPlane(point(dimIndex), dimIndex, triangle, normal, triangleIndex, 1, 2);
    }
}

bool CheckPointImpl::isPointIn2dProjection(const Eigen::RowVector3f& point,
        int dimensionIndex,
        const trim::TriangleModel& tm) const
{
    unsigned int numberOfEdges = _edges[dimensionIndex].size();
    bool retVal = false;
    if (numberOfEdges > 0) {
        PolygonMaster p(_edges[dimensionIndex].size(), dimensionIndex, point(dimensionIndex));
        std::map<int, Polygon>  &&poligons = p.getPolygons(_edges[dimensionIndex], tm);
        for (auto& poly : poligons) {
            retVal |= p.checkPointInPoligon(point, poly.second);
        }
    }
    return retVal;
}

CheckPointImpl::CheckPointImpl() : _edges(3)
{
}

bool CheckPointImpl::isInModel(const trim::TriangleModel& tm,
                               const Eigen::RowVector3f& point)
{
    for (int i = 0; i < 3; ++i)
        filterTriangleFacesByProjectionPlane(point, i, tm);

    bool ret = false;
    for (int i = 0; i < 3; ++i)
        ret ^= isPointIn2dProjection(point, i, tm);

    log("Point: [" << point << "] is " <<  (ret ? "inside" : "outside"));
    return ret;
}

} // end of namespace chp
