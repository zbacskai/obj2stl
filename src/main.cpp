#include <iostream>
#include <obj/VertexData.hpp>
#include <obj/ObjFile.hpp>
#include <TriangleModel.hpp>
#include <stl/StlAscii.hpp>
#include <stl/StlBin.hpp>

#include <boost/program_options.hpp>
#include <memory>
#include <FileReaderInterface.hpp>
#include <FileWriterInterface.hpp>
#if 0
#include <ModelConverter.hpp>
#include <CheckPoint.hpp>
#endif

namespace po = boost::program_options;

class FileFactory {
    public:
        static std::shared_ptr<meshconvert::FileReaderInterface> getFileReader(
            const std::string &iFormat, const std::string &iFileName)
        {
            if (iFormat == "obj")
                return std::make_shared<obj::ObjFile>(iFileName.c_str());
            else
                throw std::string("Supported input file formats: obj");
        }

        static std::shared_ptr<meshconvert::FileWriterInterface> getFileWriter(
            const std::string &iFormat, const std::string &iFileName)
        {
            if (iFormat == "stl-ascii")
                return std::make_shared<stl::ascii::StlAscii>(iFileName.c_str());
            else if (iFormat == "stl-bin")
                return std::make_shared<stl::bin::StlBin>(iFileName.c_str());
            else
                throw std::string("Supported output file formats: stl-ascii");
        }
};

int main(int argc, char* argv[]) {
    std::string transformationOptions = "";
    std::string pointInside = "";
    po::positional_options_description opts;
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "help using the sf")
            ("iff", "only obj supported at the moment")
            ("if", "path to the input file")
            ("off", "stl-ascii or stl-bin supported")
            ("of", "output-file name")
            ("transformations", po::value<std::string>(&transformationOptions),
                                "transformations in given order separated by /\n"
                                "possible options scale=2,1,0/rotate=-23.1,2.1,0.0/translation=1.2,3.1,0.0 \n"
                                "where the transformtion order can be pased in any order and the \n"
                                "numbers represent x,y,z rotations and x,y,z translations")
            ("point_in_model", po::value<std::string>(&pointInside),
                                "Check if given point is inside of object")
            ;

    opts.add("iff", 1);
    opts.add("if", 1);
    opts.add("off", 1);
    opts.add("of", 1);
    opts.add("transformations", 1);
    
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(opts).run(), vm);
    po::notify(vm);    

    if (vm.count("help") 
        or not vm.count("iff")
        or not vm.count("if")
        or not vm.count("off")
        or not vm.count("of"))
    {
        std::cout << "usage: " << argv[0] << " <input file format> <input file> <output file format> <output file>" <<std::endl;
        std::cout << desc << "\n";
        return 1;
    }

    try {

        std::shared_ptr<meshconvert::FileReaderInterface> fr = 
            FileFactory::getFileReader(vm["iff"].as<std::string>(),
                                       vm["if"].as<std::string>());

        std::shared_ptr<meshconvert::FileWriterInterface> fw = 
            FileFactory::getFileWriter(vm["off"].as<std::string>(),
                                       vm["of"].as<std::string>());

        trim::TriangleModel tm;
        fr->parse(tm);
        if (transformationOptions != "")
        {   
#if 0
            mc::ModelConverter mc(transformationOptions);
            mc.convert(tm);
#endif
        }
        if (pointInside != "")
        {
#if 0
            trim::Vertex v;
            v.setFromStr(pointInside);
            chp::CheckPoint c(v[0],v[1], v[2]);
            c.isInModel(tm);
#endif
        }
        else
            fw->write(tm);
    }
    catch (std::string& e) {
        std::cout << e << std::endl;
    }
        
    return 0;
}
