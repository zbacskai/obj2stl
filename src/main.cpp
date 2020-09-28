#include <iostream>
#include <obj/VertexData.hpp>
#include <obj/ObjFile.hpp>
#include <TriangleModel.hpp>

int main() {
    obj::ObjFile o("./obj-files/test_01_hand.obj");
    o.parse();
    trim::TriangleModel tm;
    o.convertToTriangleModel(tm);
    return 0;
}
