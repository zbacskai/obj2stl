#include <ModelConverter.hpp>
#include <TriangleModel.hpp>

#include <eigen3/Eigen/Dense>

#include <vector>
#include <memory>
#include <sstream>
#include <iostream>

namespace mc {
    class ConversionBase {
    protected:
        Eigen::Matrix4f m_; 
    public:
        virtual Eigen::Matrix4f& getMatrix() = 0;
        virtual ~ConversionBase() {};
    };

    class Scale : public ConversionBase {      
    public:
        Scale(float kx, float ky, float kz)  {
            m_ <<  kx, 0.0, 0.0, 0.0,
                  0.0,  ky, 0.0, 0.0,
                  0.0, 0.0,  kz, 0.0,
                  0.0, 0.0, 0.0, 1.0;
        };
        ~Scale() {};
        virtual Eigen::Matrix4f& getMatrix() {
            return m_;
        };

    };

    class Translation : public ConversionBase {      
    public:
        Translation(float dx, float dy, float dz)  {
            m_ << 1.0, 0.0, 0.0, 0.0,
                  0.0, 1.0, 0.0, 0.0,
                  0.0, 0.0, 1.0, 0.0,
                   dx,  dy,  dz, 1.0;
        };
        ~Translation() {};
        virtual Eigen::Matrix4f& getMatrix() {
            return m_;
        };

    };
}

namespace {

    void buildConversionStack(const std::string conversionParameters,
                              std::vector<std::shared_ptr<mc::ConversionBase>> &conversionStack)
    {
        std::stringstream inputfs(conversionParameters);
        std::string convstr;
        while (std::getline(inputfs, convstr, '/'))
        {
            std::stringstream conversion_desc(convstr);
            std::string c_desc;
            std::vector<std::string> cdesc_list;

            while(std::getline(conversion_desc, c_desc, '='))
                cdesc_list.push_back(c_desc);

            std::stringstream param_desc(cdesc_list[1]);
            std::string param;
            std::vector<float> pl;

            while(std::getline(param_desc, param, ','))
                pl.push_back(std::atof(param.c_str()));
            
            if (cdesc_list[0] == "scale")
                conversionStack.push_back(std::make_shared<mc::Scale>(pl[0], pl[1], pl[2]));
            else if (cdesc_list[0] == "translation")
                conversionStack.push_back(std::make_shared<mc::Translation>(pl[0], pl[1], pl[2]));

        }
    }
}

namespace mc {


ModelConverter::ModelConverter(const std::string& conversionParameters) : conversionParameters_(conversionParameters)
{

}
    
void ModelConverter::convert(trim::TriangleModel tm)
{
    std::vector<std::shared_ptr<ConversionBase>> conversionStack;
    buildConversionStack(conversionParameters_, conversionStack);

    Eigen::Matrix4f cv = conversionStack[0]->getMatrix();
    for(unsigned int i = 1; i < conversionStack.size(); ++i)
        cv *= conversionStack[i]->getMatrix();

    for (auto triangle : tm.getTriangles())
    {
        Eigen::RowVector4f normalVec;
        normalVec << triangle->getNormalVector().p[0],
                     triangle->getNormalVector().p[1],
                     triangle->getNormalVector().p[2],
                     1.0;

        normalVec = normalVec * cv;
        triangle->setNormalVector({normalVec(0),
                                   normalVec(1),
                                   normalVec(2)});

        for(unsigned int i = 0; i < 3; ++i)
        {
            Eigen::RowVector4f vertexVec;
            vertexVec << triangle->operator[](i).p[0],
                     triangle->operator[](i).p[1],
                     triangle->operator[](i).p[2],
                     1.0;

            vertexVec = vertexVec * cv;
            triangle->setVertex(i, {vertexVec(0), vertexVec(1), vertexVec(2)});
    
        }
    }

}

ModelConverter::~ModelConverter()
{

}

}