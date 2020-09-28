#include <iostream>
#include <obj/VertexData.hpp>
#include <obj/ObjFile.hpp>

int main() {
    obj::ObjFile o("./obj-files/test_01_hand.obj");
    o.parse();
    return 0;
}
