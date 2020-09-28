#include <iostream>
#include <obj/VertexData.hpp>
#include <obj/ObjFile.hpp>
#include <TriangleModel.hpp>
#include <stl/StlAscii.hpp>

#include <boost/program_options.hpp>
#include <memory>
#include <FileReaderInterface.hpp>
#include <FileWriterInterface.hpp>

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
            else
                throw std::string("Supported output file formats: stl-ascii");
        }
};

int main(int argc, char* argv[]) {
    po::positional_options_description opts;
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "help using the sf")
            ("iff", "only obj supported at the moment")
            ("if", "path to the input file")
            ("off", "stl-ascii or stl-bin supported")
            ("of", "output-file name");

    opts.add("iff", 1);
    opts.add("if", 1);
    opts.add("off", 1);
    opts.add("of", 1);
    
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
        fr->parse();
        fr->convertToTriangleModel(tm);
        fw->write(tm);
    }
    catch (std::string& e) {
        std::cout << e << std::endl;
    }
        
    return 0;
}
