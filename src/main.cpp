#include <iostream>
#include <obj/VertexData.hpp>
#include <obj/ObjFile.hpp>
#include <TriangleModel.hpp>
#include <stl/StlAscii.hpp>

int main() {
    //obj::ObjFile o("./obj-files/test_01_hand.obj");
    //obj::ObjFile o("./obj-files/test_04_sofa.obj");
    //obj::ObjFile o("./obj-files/test_05_skull.obj");
    //obj::ObjFile o("./obj-files/test_06_ribcage.obj");
    obj::ObjFile o("./obj-files/test_07_desk.obj");
    o.parse();
    trim::TriangleModel tm;
    o.convertToTriangleModel(tm);
    stl::ascii::StlAscii::write_stdout(tm);
    return 0;
}
