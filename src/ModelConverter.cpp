#include <ModelConverter.hpp>
#include <TriangleModel.hpp>
#include <Utils.hpp>

#include <eigen3/Eigen/Dense>

#include <vector>
#include <memory>
#include <sstream>
#include <iostream>
#include <cmath>

#include <eigen3/Eigen/Dense>

namespace mc {

    class ConversionBase 
    {
        protected:
            Eigen::Matrix4f m_; 
        public:
            virtual Eigen::Matrix4f& getMatrix() = 0;
            virtual ~ConversionBase() {};
    };

    typedef  std::vector<std::shared_ptr<mc::ConversionBase>> ConversionBaseVector;

    class Scale : public ConversionBase 
    {      
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

    class Rotate : public ConversionBase {      
    private:
        static const float pi;
    public:
        Rotate(float tx, float ty, float tz)  {
            // Rotate Z axis
            float cosThetaZ = cos(tz*pi/180.0);
            float sinThetaZ = sin(tz*pi/180.0);
            m_ << cosThetaZ, sinThetaZ, 0.0, 0.0,
                  -1.0 * sinThetaZ, cosThetaZ, 0.0, 0.0,
                  0.0, 0.0, 1.0, 0.0,
                  0.0, 0.0, 0.0, 1.0;

            // Rotate Y axis
            Eigen::Matrix4f rotY;
            float cosThetaY = cos(ty*pi/180.0);
            float sinThetaY = sin(ty*pi/180.0);
            rotY << -1.0 * sinThetaY, cosThetaY, 0.0, 0.0,
                    0.0, 0.0, 1.0, 0.0,
                    cosThetaY, sinThetaY, 0.0, 0.0,
                    0.0, 0.0, 0.0, 1.0;

            m_ = m_ * rotY;

            // Rotate X axis
            Eigen::Matrix4f rotX;
            float cosThetaX = cos(tx*pi/180.0);
            float sinThetaX = sin(tx*pi/180.0);
            rotX << 0.0, 0.0, 1.0, 0.0,
                    cosThetaX, sinThetaX, 0.0, 0.0,
                    -1.0 * sinThetaX, cosThetaX, 0.0, 0.0,
                    0.0, 0.0, 0.0, 1.0;

            m_ = m_ * rotX;
        };
        ~Rotate() {};
        virtual Eigen::Matrix4f& getMatrix() {
            return m_;
        };

    };

    const float Rotate::pi = 3.14159265;
} // end of namespace mc

namespace {

    void createConversionMatrix(const std::string& conversionType,
                                const std::string& paramStr,
                                mc::ConversionBaseVector &conversionStack,
                                mc::ConversionBaseVector &conversionStackNormals)
    {
        Eigen::RowVector3f v = utils::strCoord2RowVector(paramStr);
                        
        if (conversionType == "scale")
        {
            conversionStack.push_back(std::make_shared<mc::Scale>(v(0), v(1), v(2)));
            conversionStackNormals.push_back(std::make_shared<mc::Scale>(v(0), v(1), v(2)));
        }
        else if (conversionType == "translation")
            conversionStack.push_back(std::make_shared<mc::Translation>(v(0), v(1), v(2)));
        else if (conversionType == "rotate")
        {
            conversionStack.push_back(std::make_shared<mc::Rotate>(v(0), v(1), v(2)));
            conversionStackNormals.push_back(std::make_shared<mc::Scale>(v(0), v(1), v(2)));
        }
        else
        {
            std::stringstream ss;
            ss << "Invalid conversion method: " << conversionType;
            throw ss.str();
        }
    }

    void buildConversionStack(const std::string& conversionParameters,
                              mc::ConversionBaseVector &conversionStack,
                              mc::ConversionBaseVector &conversionStackNormals)
    {
        std::stringstream inputfs(conversionParameters);
        std::string convstr;
        while (std::getline(inputfs, convstr, '/'))
        {
            std::stringstream conversionDesc(convstr);
            std::string cDesc;
            std::vector<std::string> cDescList;

            while(std::getline(conversionDesc, cDesc, '='))
                cDescList.push_back(cDesc);

            createConversionMatrix(cDescList[0], cDescList[1], conversionStack, conversionStackNormals);
        }
    }

    Eigen::Matrix4f buildConversionMatrix(const mc::ConversionBaseVector &conversionStack)
    {
        Eigen::Matrix4f cv = conversionStack[0]->getMatrix();
        for(unsigned int i = 1; i < conversionStack.size(); ++i)
            cv *= conversionStack[i]->getMatrix();

        return std::move(cv);
    }
}

namespace mc {


ModelConverter::ModelConverter(const std::string& conversionParameters) : 
                        conversionParameters_(conversionParameters)
{

}
    
void ModelConverter::convert(trim::TriangleModel &tm)
{
    mc::ConversionBaseVector conversionStack;
    mc::ConversionBaseVector conversionStackNormals;
    buildConversionStack(conversionParameters_, conversionStack, conversionStackNormals);

    Eigen::Matrix4f &&cv = buildConversionMatrix(conversionStack);
    Eigen::Matrix4f &&cvNormal = buildConversionMatrix(conversionStack);
    
    tm.applyTransformatioMatrix(cv, cvNormal);
}

ModelConverter::~ModelConverter()
{

}

}
