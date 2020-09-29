#include <CheckPoint.hpp>
#include <eigen3/Eigen/Dense>

#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <set>

#include <list>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>


namespace {
    struct Coordinates {
        float p[2];
    };

    enum planeType{
        pX,
        pY,
        pZ
    };

    void addPointToPoly(const Eigen::Vector3f& point,
                        std::vector<Coordinates> &projectedPoly,
                        planeType plT,
                        const trim::Vertex& minP,
                        const trim::Vertex& maxP)
    {
        switch (plT)
        {
        case pX:
            projectedPoly.push_back({point(1), point(2)});
            break;
        case pY:
            projectedPoly.push_back({point(0), point(2)});
            break;
        case pZ:
            projectedPoly.push_back({point(0), point(1)});
            break;
        default:
            break;
        }
    }

    void calculateCrossSection(std::vector<std::shared_ptr<trim::Triangle>> projectTriangle,
                               std::vector<Coordinates> &projectedPoly,
                               const Eigen::Matrix3f& plane,
                               planeType plT)
    {
        for (auto triangle : projectTriangle)
        {
            Eigen::Vector3f ia = triangle->operator[](0);
            Eigen::Vector3f ib = triangle->operator[](1);
            Eigen::Vector3f ic = triangle->operator[](2);


            Eigen::Vector3f iab = ib - ia;
            Eigen::Vector3f iac = ic - ia;
            Eigen::Vector3f ibc = ic - ib;

            Eigen::Vector3f p01 = plane.col(1) - plane.col(0);
            Eigen::Vector3f p02 = plane.col(2) - plane.col(0);

            Eigen::RowVector3f p01xp02 = p01.cross(p02);

            float detAB = p01xp02 * (iab * -1.0);
            float detAC = p01xp02 * (iac * -1.0);
            float detBC = p01xp02 * (ibc * -1.0);

            trim::Vertex minP = triangle->getMin();
            trim::Vertex maxP = triangle->getMin();

            std::cout << "Det " << detAB << " " << detAC << " " << detBC << std::endl;
            if (detAB != 0.0)
            {
                float t = p01xp02 * (ia - plane.col(0));
                t/=detAB;
                Eigen::Vector3f point = ia + (iab*t);
                std::cout << "[ "  << point.transpose() << " ] " << plT << std::endl;
                addPointToPoly(point, projectedPoly, plT, minP, maxP);
            }

            if (detAC != 0.0)
            {
                float t = p01xp02 * (ia - plane.col(0));
                t/=detAC;
                Eigen::Vector3f point = ia + (iac*t);
                std::cout << "[ " << point.transpose() << " ] " << plT <<  std::endl;
                addPointToPoly(point, projectedPoly, plT, minP, maxP);
            }

            if (detBC != 0.0)
            {
                float t = p01xp02 * (ib - plane.col(0));
                t/=detBC;
                Eigen::Vector3f point = ib + (ibc*t);
                std::cout << "[ " << point.transpose() <<  " ] " << plT <<  std::endl;
                addPointToPoly(point, projectedPoly, plT, minP, maxP);
            }
        }

    }

    std::string constructPointStr(float x, float y) {
        std::stringstream ss;
        ss << std::setprecision(6);
        ss << std::fixed;
        ss << x << " " << y;
        return ss.str();
    }

    bool checkPointInPoly(float x, float y, std::vector<Coordinates> &projectedPoly)
    {
        std::set<std::string> points;
        for (auto c : projectedPoly) {
            std::string ps = constructPointStr(c.p[0], c.p[1]);
            points.insert(ps);
        }

        std::stringstream poly_str;

        poly_str << "POLYGON((";
        bool first = true;
        for (auto p : points)
        {
            if (not first)
                poly_str << ",";

            poly_str << p;
            first = false;
        }
        poly_str << "))";

        std::string test_str = poly_str.str();

        std::cout << test_str << std::endl;

        typedef boost::geometry::model::d2::point_xy<double> point_type;
        typedef boost::geometry::model::polygon<point_type> polygon_type;

        polygon_type poly;
        boost::geometry::read_wkt(test_str, poly);
        
        point_type p(x, y);

        std::cout << x << "---" << y << std::endl;

        std::cout << "within: " << (boost::geometry::within(p, poly) ? "yes" : "no") << std::endl;
        return boost::geometry::within(p, poly);
    }
}

namespace chp {

bool CheckPoint::isInModel(const trim::TriangleModel& tm)
{
    float x = point_.p[0];
    float y = point_.p[1];
    float z = point_.p[2];

    std::vector<std::shared_ptr<trim::Triangle>> crossSectX;
    std::vector<std::shared_ptr<trim::Triangle>> crossSectY;
    std::vector<std::shared_ptr<trim::Triangle>> crossSectZ;

    float absMinX = 0.0;
    float absMinY = 0.0;
    float absMinZ = 0.0;

    float absMaxX = 0.0;
    float absMaxY = 0.0;
    float absMaxZ = 0.0;

    for (auto triangle : tm.getTriangles())
    {
        const trim::Vertex& minP = triangle->getMin();
        const trim::Vertex& maxP = triangle->getMax();

        std::cout << "Min: " << minP.p[0] << ","
                             << minP.p[1] << ","
                             << minP.p[2] 
                  << " Max: " << maxP.p[0] << ","
                              << maxP.p[1] << ","
                              << maxP.p[2] <<std::endl;

        if (minP.p[0] < x and x < maxP.p[0])
            crossSectX.push_back(triangle);

        if (minP.p[1] < y and y < maxP.p[1])
            crossSectY.push_back(triangle);

        if (minP.p[2] < z and z < maxP.p[2])
            crossSectZ.push_back(triangle);

        absMaxX = std::max(maxP.p[0], absMaxX);
        absMaxY = std::max(maxP.p[1], absMaxY);
        absMaxZ = std::max(maxP.p[2], absMaxZ);

        absMinX = std::min(minP.p[0], absMinX);
        absMinY = std::min(minP.p[1], absMinY);
        absMinZ = std::min(minP.p[2], absMinZ);
    }

    if ( x < absMinX or x > absMaxX or y < absMinY or y > absMaxY or z < absMinZ or z > absMaxZ)
    {
        std::cout << "Point is outside max-min vals can't be inside" << std::endl;
        std::cout << absMinX << "," << absMinY << "," << absMinZ << std::endl;
        std::cout << absMaxX << "," << absMaxY << "," << absMaxZ << std::endl;
        return false; 
    }

    std::vector<Coordinates> projectedPolyX;
    Eigen::Matrix3f planeX;
    planeX << x, x, x,
              0.0, 1.0, 0.0,
              0.0, 0.0, 1.0;
    calculateCrossSection(crossSectX, projectedPolyX, planeX, pX);

    std::vector<Coordinates> projectedPolyY;
    Eigen::Matrix3f planeY;
    planeY << 0.0, 1.0, 0.0,
                y,   y,   y,
              0.0, 0.0, 1.0;
    calculateCrossSection(crossSectY, projectedPolyY, planeY, pY);

    std::vector<Coordinates> projectedPolyZ;
    Eigen::Matrix3f planeZ;
    planeZ << 0.0, 1.0, 0.0,
              0.0, 0.0, 1.0,
                z,   z,   z;
    calculateCrossSection(crossSectZ, projectedPolyZ, planeZ, pZ);

    return checkPointInPoly(x, y, projectedPolyZ) and
           checkPointInPoly(x, z, projectedPolyY) and
           checkPointInPoly(y, z, projectedPolyX);
}

}
