#include <obj/ObjFile.hpp>
#include <obj/VertexData.hpp>

#include <sstream>
#include <fstream>

#include<TriangleModel.hpp>

namespace obj {

ObjFile::ObjFile(const char* fileName) :
    FileReaderInterface(fileName),
    _fileName(fileName),
    _vCount(0), _vtCount(0), _vnCount(0)

{

}

Eigen::RowVector4f ObjFile::calculateMedian(const trim::TriangleModel::ModelMatrix& m,
        const std::vector<int>& refs) const
{
    float total = 0;
    float sumX = 0;
    float sumY = 0;
    float sumZ = 0;
    for (auto r : refs) {
        total+=1.0;
        sumX += m(r,0);
        sumY += m(r,1);
        sumZ += m(r,2);
    }
    Eigen::RowVector4f row;
    row << sumX/total, sumY/total, sumZ/total, 1.0;

    return row;
}


void ObjFile::addMultiTriangle(const Surface& surface,
                               trim::TriangleModel& tm)
{
    int medianVertex = tm.addVertex(calculateMedian(tm.getVerticleMatrix(), surface.getVRefs()));
    int medianTexture = tm.addTexture(calculateMedian(tm.getTextureMatrix(), surface.getTvRefs()));
    int medianNormal = tm.addNormalVector(calculateMedian(tm.getNormalMatrix(), surface.getVnRefs()));

    Eigen::Vector3i median;
    median << medianVertex, medianTexture, medianNormal;
    int prev = 0;
    for (unsigned int curr = 1; curr < surface.getVRefs().size(); ++curr, ++prev)
        addSimpleTriangle(surface, tm, &median, prev, curr, 0);
}


void ObjFile::add2Triangles(const Surface& surface,  trim::TriangleModel& tm)
{
    addSimpleTriangle(surface, tm, 0, 0, 1, 2);
    addSimpleTriangle(surface, tm, 0, 2, 3, 0);
}

void ObjFile::addSimpleTriangle(const Surface& surface,
                                trim::TriangleModel& tm,
                                Eigen::Vector3i* cpoint,
                                int a, int b, int c)
{
    trim::TriangleData verticles(
        surface.getVRefs()[a],
        surface.getVRefs()[b],
        cpoint == 0 ? surface.getVRefs()[c] : (*cpoint)(0));

    trim::TriangleData textures(
        surface.getTvRefs()[a],
        surface.getTvRefs()[b],
        cpoint == 0 ? surface.getTvRefs()[c] : (*cpoint)(1));

    trim::TriangleData normals(
        surface.getVnRefs()[a],
        surface.getVnRefs()[b],
        cpoint == 0 ? surface.getVnRefs()[c] : (*cpoint)(2));

    tm.addTriangle(verticles, textures, normals);
}

void ObjFile::convertToTriangles(const Surface& surface,  trim::TriangleModel& tm)
{
    int numberOfVerticles = surface.getVRefs().size();
    if (numberOfVerticles == 3)
        addSimpleTriangle(surface, tm);
    else if (numberOfVerticles == 4)
        add2Triangles(surface, tm);
    else if (numberOfVerticles > 4)
        addMultiTriangle(surface, tm);
    else {
        std::stringstream ss;
        ss << "Something wrong. Can't consturct urface from. " <<
           numberOfVerticles;
        throw ss.str();
    }
}

bool ObjFile::processSurface(const std::string& prefix,
                             const std::string& infoType,
                             std::istringstream& iss)
{
    if (infoType != prefix)
        return false;

    std::shared_ptr<Surface> s = std::make_shared<Surface>();
    iss >> *s;
    s->makeRefsAbsolute(_vCount, _vtCount, _vnCount);
    _surfaces.push_back(s);

    return true;
}

trim::TriangleModel& ObjFile::parse(trim::TriangleModel& tm)
{
    std::ifstream infile(_fileName);
    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string infoType;
        iss >> infoType;

        if (processFileEntry<GeometricVertex>("v", infoType, iss, _vCount, tm) or
                processFileEntry<TextureVertex>("vt", infoType, iss, _vtCount, tm) or
                processFileEntry<VertexNormal>("vn", infoType, iss, _vnCount, tm) or
                processSurface("f", infoType, iss))
            continue;
    }
    for (auto s : _surfaces) {
        convertToTriangles(*s, tm);
    }
    return tm;
}
} //end of namespace obj
