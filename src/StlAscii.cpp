#include <stl/StlAscii.hpp>

#include<iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

#include <arpa/inet.h>

namespace {

    void writeVertex(const trim::TriangleData& triangle, 
                           std::ofstream& ofile)
    {
        for (int i = 0; i < 3; ++i)
        {
            ofile << "        vertex ";
            ofile << triangle(i, 0) << " ";
            ofile << triangle(i, 1) << " ";
            ofile << triangle(i, 2) << std::endl;
        }
    }

    void writeNormalVector(const trim::TriangleData& triangleNormal, 
                           std::ofstream& ofile)
    {
        ofile << triangleNormal(0,0) << " ";
        ofile << triangleNormal(0,1) << " ";
        ofile << triangleNormal(0,1) << std::endl;
    }

    void writeTriangle(const trim::TriangleData& triangle, 
                       const trim::TriangleData& triangleNormal, 
                       std::ofstream& ofile)
    {
        ofile << "facet normal ";
        writeNormalVector(triangleNormal, ofile);
        ofile << "    outer loop" << std::endl;
        writeVertex(triangle, ofile);
        ofile << "    endloop" << std::endl;
        ofile << "endfacet" << std::endl;
    }
}

namespace stl {
namespace ascii {

StlAscii::StlAscii(const char* fileName) : meshconvert::FileWriterInterface(fileName), fileName_(fileName) {

}

void StlAscii::write(const trim::TriangleModel &tm) {
    std::ofstream ofile;
    ofile.open (fileName_);
    ofile << std::setprecision(6);
    ofile << std::scientific;

    ofile << "solid test " << std::endl;
    
    auto& triangleNormals = tm.getTriangleNormals();
    auto& triangles = tm.getTriangles();
    for (unsigned int i = 0; i < triangles.size(); ++i)
        writeTriangle(triangles[i], triangleNormals[i], ofile);
    ofile.close();
}


} // end of namespace ascii
} // end of namespace stl
