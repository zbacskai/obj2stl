#include <stl/StlAscii.hpp>

#include<iostream>
#include <fstream>
#include <iomanip>

namespace stl {
namespace ascii {

StlAscii::StlAscii(const char* fileName) : meshconvert::FileWriterInterface(fileName), fileName_(fileName) {

}

void StlAscii::write(const trim::TriangleModel &tm) {
    std::ofstream ofile;
    ofile.open (fileName_);
  
    ofile << "solid test " << std::endl;
    ofile << std::setprecision(6);
    ofile << std::scientific;

    for (auto triangle : tm.getTriangles())
    {
        ofile << "facet normal ";
        ofile << triangle->getNormalVector().p[0] << " ";
        ofile << triangle->getNormalVector().p[1] << " ";
        ofile << triangle->getNormalVector().p[2] << std::endl;

        ofile << "    outer loop" << std::endl;
        for (int i = 0; i < 3; ++i)
        {
            ofile << "        vertex ";
            ofile << triangle->operator[](i).p[0] << " ";
            ofile << triangle->operator[](i).p[1] << " ";
            ofile << triangle->operator[](i).p[2] << std::endl;

        }
        ofile << "    endloop" << std::endl;
        ofile << "endfacet" << std::endl;
    }
    ofile.close();
}


} // end of namespace ascii
} // end of namespace stl
