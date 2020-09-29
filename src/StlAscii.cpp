#include <stl/StlAscii.hpp>

#include<iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

#include <arpa/inet.h>

namespace {

    void writeVertex(const trim::Triangle& triangle, 
                           std::ofstream& ofile)
    {
        for (int i = 0; i < 3; ++i)
        {
            ofile << "        vertex ";
            ofile << triangle[i].p[0] << " ";
            ofile << triangle[i].p[1] << " ";
            ofile << triangle[i].p[2] << std::endl;
        }
    }

    void writeNormalVector(const trim::Triangle& triangle, 
                           std::ofstream& ofile)
    {
        ofile << triangle.getNormalVector()[0] << " ";
        ofile << triangle.getNormalVector()[1] << " ";
        ofile << triangle.getNormalVector()[2] << std::endl;
    }

    void writeTriangle(const trim::Triangle& triangle, 
                       std::ofstream& ofile)
    {
        ofile << "facet normal ";
        writeNormalVector(triangle, ofile);
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

    for (auto triangle : tm.getTriangles())
        writeTriangle(*triangle, ofile);
    ofile.close();
}


} // end of namespace ascii
} // end of namespace stl
