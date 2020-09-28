#include <stl/StlAscii.hpp>

#include<iostream>
namespace stl {
namespace ascii {
    
void StlAscii::write_stdout(const trim::TriangleModel &tm) {
    std::cout << "solid test " << std::endl;
    std::cout.precision(6);
    std::cout << std::scientific;

    for (auto triangle : tm.getTriangles())
    {
        std::cout << "facet normal ";
        std::cout << triangle->getNormalVector().p[0] << " ";
        std::cout << triangle->getNormalVector().p[1] << " ";
        std::cout << triangle->getNormalVector().p[2] << std::endl;

        std::cout << "    outer loop" << std::endl;
        for (int i = 0; i < 3; ++i)
        {
            std::cout << "        vertex ";
            std::cout << triangle->operator[](i).p[0] << " ";
            std::cout << triangle->operator[](i).p[1] << " ";
            std::cout << triangle->operator[](i).p[2] << std::endl;

        }
        std::cout << "    endloop" << std::endl;
        std::cout << "endfacet" << std::endl;
    }
}


} // end of namespace ascii
} // end of namespace stl
