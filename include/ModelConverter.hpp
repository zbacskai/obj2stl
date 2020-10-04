#
#ifndef ModelConverter_hpp
#define ModelConverter_hpp

#include <string>

namespace trim {
class TriangleModel;
}

namespace mc {

class ModelConverter {
private:
    std::string conversionParameters_;
public:
    ModelConverter(const std::string& conversionParameters);
    void convert(trim::TriangleModel &tm);
    ~ModelConverter();
};

}

#endif