#include <obj/ObjFile.hpp>
#include <obj/VertexData.hpp>

#include <sstream>
#include <fstream>

namespace obj {

    ObjFile::ObjFile(const char* fileName) : fileName_(fileName), vCount_(0), vtCount_(0), vnCount_(0), FileReaderInterface(fileName) {}

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

    void ObjFile::addSimpleTriangle(trim::TriangleModel &tm, const Surface& s) {
        int vnRef = s.getVnRefs()[0];
        std::shared_ptr<VertexNormal> vn = vn_[vnRef];
        std::shared_ptr<trim::Triangle> t = 
                std::make_shared<trim::Triangle>(
                    v_[s.getVRefs()[0]]->getVertex(),
                    v_[s.getVRefs()[1]]->getVertex(),
                    v_[s.getVRefs()[2]]->getVertex());

        if (vnRef > 0)
            t->setNormalVector(vn->getVertex());
        else
            t->calculateNormalVector();
        
        tm.addTriangle(t);
    }

    void ObjFile::add2Triangle(trim::TriangleModel &tm, const Surface& s) {
        int vnRef = s.getVnRefs()[0];
        
        std::shared_ptr<trim::Triangle> t1 = 
                std::make_shared<trim::Triangle>(
                    v_[s.getVRefs()[0]]->getVertex(),
                    v_[s.getVRefs()[1]]->getVertex(),
                    v_[s.getVRefs()[2]]->getVertex());

        std::shared_ptr<trim::Triangle> t2 = 
                std::make_shared<trim::Triangle>(
                    v_[s.getVRefs()[2]]->getVertex(),
                    v_[s.getVRefs()[3]]->getVertex(),
                    v_[s.getVRefs()[0]]->getVertex());

        if (vnRef > 0)
        {
            std::shared_ptr<VertexNormal> vn = vn_[vnRef];
            t1->setNormalVector(vn->getVertex());
            t2->setNormalVector(vn->getVertex());
        }
        else
        {
            t1->calculateNormalVector();
            t2->calculateNormalVector();
        }
        
        tm.addTriangle(t1);
        tm.addTriangle(t2);
    }
        
    void ObjFile::addMultiTriangle(trim::TriangleModel &tm, const Surface& s) {
        float total = 0;
        float sumX = 0;  
        float sumY = 0;  
        float sumZ = 0;   
        for (auto vert : s.getVRefs())
        {
            auto gvert = v_[vert];
            total+=1.0;
            sumX += gvert->getX();
            sumY += gvert->getY();
            sumZ += gvert->getZ();
        }
        trim::Vertex median = {sumX/total, sumY/total, sumZ/total};
        int vnRef = s.getVnRefs()[0];
        bool calculateNormalVector = not (vnRef > 0);
        std::shared_ptr<VertexNormal> vn = (not calculateNormalVector ? vn_[vnRef] : 0);
        int prev = 0;
        for (int curr = 1; curr < s.getVRefs().size(); ++curr, ++prev)
        {
            std::shared_ptr<trim::Triangle> t = 
                std::make_shared<trim::Triangle>(
                    v_[s.getVRefs()[prev]]->getVertex(),
                    v_[s.getVRefs()[curr]]->getVertex(),
                    median);
            
            if (calculateNormalVector)
                t->calculateNormalVector();
            else
                t->setNormalVector(vn->getVertex());

            tm.addTriangle(t);
        }
    }

    trim::TriangleModel& ObjFile::convertToTriangleModel(trim::TriangleModel &tm)
    {
        for (auto surface : s_)
        {
            int numberOfVerticles = surface->getVRefs().size();
            if (numberOfVerticles == 3)
                addSimpleTriangle(tm, *surface);
            else if (numberOfVerticles == 4)
                add2Triangle(tm, *surface);
            else if (numberOfVerticles > 4)
                addMultiTriangle(tm, *surface);
            else
            {
                std::cout << "Something wrong. Can't consturct surface from. " << numberOfVerticles << std::endl;
            }
            
        }
        return tm;
    }
}
