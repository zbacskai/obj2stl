#include <obj/ObjFile.hpp>
#include <obj/VertexData.hpp>

#include <sstream>
#include <fstream>

namespace obj {

    ObjFile::ObjFile(const char* fileName) : fileName_(fileName), vCount_(0), vtCount_(0), vnCount_(0) {}

    void ObjFile::parse() {       
        std::ifstream infile(fileName_);
        std::string line;
        while (std::getline(infile, line))
        {
            std::istringstream iss(line);
            std::string info_type;
            iss >> info_type;

            if (info_type == "v")
            {
                std::shared_ptr<GeometricVertex> v = std::make_shared<GeometricVertex>();
                iss >> *v;
                vCount_++;
                v_.push_back(v);
            } else if (info_type == "vt")
            {
                std::shared_ptr<TextureVertex> vt = std::make_shared<TextureVertex>();
                iss >> *vt;
                vtCount_++;
                vt_.push_back(vt);
            } else if (info_type == "vn")
            {
                std::shared_ptr<VertexNormal> vn = std::make_shared<VertexNormal>();
                iss >> *vn;
                vnCount_++;
                vn_.push_back(vn);
            } else if (info_type == "f")
            {
                std::shared_ptr<Surface> s = std::make_shared<Surface>();
                iss >> *s;
                s->makeRefsAbsolute(vCount_, vtCount_, vnCount_);
                s_.push_back(s);
            }
        }
    }
}
