/**
  *
  * Copyright 2020 Zoltan Bacskai
  *
  **/
#include <stl/StlBin.hpp>

#include<iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

#include <endian.h>

namespace {
#pragma pack(push, 1)

struct fheader {
    uint8_t data[80];
    uint32_t num_of_triangles;
};

union real32 {
    float f;
    uint32_t wo;
};

struct triangle_info {
    real32 normal[3];
    real32 v[3][3];
    uint16_t attr_byte_count;
};

#pragma pack(pop)

} // end of unnamed namespace

namespace stl {
namespace bin {

StlBin::StlBin(const char* fileName) : meshconvert::FileWriterInterface(fileName), fileName_(fileName)
{

}

void StlBin::write(const trim::TriangleModel &tm)
{
    std::ofstream ofile;
    ofile.open (fileName_);

    fheader f;
    std::strcpy(reinterpret_cast<char*>(&f.data[0]), "binary stl by bacsek");
    f.num_of_triangles = tm.getTriangles().size();
    f.num_of_triangles = htole32(f.num_of_triangles);
    ofile.write(reinterpret_cast<char*>(&f), sizeof(fheader));

    auto& triangles = tm.getTriangles();
    auto& triangleNormals = tm.getTriangleNormals();
    for (unsigned int k = 0; k < triangles.size(); ++k) {
        triangle_info ti;
        for (int i = 0; i < 3; ++i) {
            ti.normal[i].f = triangleNormals[k](0,i);
            ti.normal[i].wo = htole32(ti.normal[i].wo);
        }

        for (int j = 0; j < 3; ++j)
            for (int i = 0; i < 3; ++i) {
                ti.v[j][i].f = triangles[k](j,i);
                ti.v[j][i].wo = htole32(ti.v[j][i].wo);
            }

        ti.attr_byte_count = 0;
        ofile.write(reinterpret_cast<char*>(&ti), sizeof(triangle_info));
    }
    ofile.close();
}


} // end of namespace ascii
} // end of namespace stl
