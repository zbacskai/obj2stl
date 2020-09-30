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
        trim::TriangleModel& _tm;
        int _vCount;
        int _vtCount;
        int _vnCount;
        
        Eigen::RowVector4f  calculateMedian(
            const trim::TriangleModel::ModelMatrix& m,
            const std::vector<int>& refs) const;
        void addMultiTriangle(const Surface& surface);
        void add2Triangles(const Surface& surface);
        void addSimpleTriangle(const Surface& surface,
                               Eigen::Vector3i *cpoint = 0,
                               int a=0, int b=1, int c=2);
        void convertToTriangles(const Surface& surface);
        template <typename T>
        bool processFileEntry(const std::string& prefix,
                              const std::string& infoType,
                              std::istringstream& iss,
                              int &counter)
        {
            if (infoType != prefix)
                return false;
            
            T v;
            iss >> v;
            v.storeInModel(_tm);
            counter++;
            return true;
        }

        bool processSurface(const std::string& prefix,
                            const std::string& infoType,
                            std::istringstream& iss);
    public:
        ObjFile(const char* fileName, trim::TriangleModel& tm);
        virtual trim::TriangleModel& parse();
        virtual ~ObjFile() {};
};

}

#endif