#pragma once
#ifndef ObjFile_hpp
#define ObjFile_hpp

#include <string>
#include <vector>
#include <memory>

#include<FileReaderInterface.hpp>

namespace obj {

class GeometricVertex;
class TextureVertex;
class VertexNormal;
class Surface;

class ObjFile : public meshconvert::FileReaderInterface {
    private:
        std::string _fileName;
        int _vCount;
        int _vtCount;
        int _vnCount;
        std::vector<std::shared_ptr<Surface>> _surfaces;
        
        Eigen::RowVector4f  calculateMedian(
            const trim::TriangleModel::ModelMatrix& m,
            const std::vector<int>& refs) const;
        void addMultiTriangle(const Surface& surface,
                              trim::TriangleModel& tm);
        void add2Triangles(const Surface& surface,
                           trim::TriangleModel& tm);
        void addSimpleTriangle(const Surface& surface,
                               trim::TriangleModel& tm,
                               Eigen::Vector3i *cpoint = 0,
                               int a=0, int b=1, int c=2);
        void convertToTriangles(const Surface& surface,
                                trim::TriangleModel& tm);
        template <typename T>
        bool processFileEntry(const std::string& prefix,
                              const std::string& infoType,
                              std::istringstream& iss,
                              int &counter,
                               trim::TriangleModel& tm)
        {
            if (infoType != prefix)
                return false;
            
            T v;
            iss >> v;
            v.storeInModel(tm);
            counter++;
            return true;
        }

        bool processSurface(const std::string& prefix,
                            const std::string& infoType,
                            std::istringstream& iss);
    public:
        ObjFile(const char* fileName);
        virtual trim::TriangleModel& parse( trim::TriangleModel& tm);
        virtual ~ObjFile() {};
};

}

#endif