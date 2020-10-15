#ifndef VertexData_hpp
#define VertexData_hpp
/**
  *
  * Copyright 2020 Zoltan Bacskai
  * 
  **/
 
#include <iostream>
#include <iomanip>
#include <vector>

#include<TriangleModel.hpp>
#include <eigen3/Eigen/Dense>

namespace obj {

class ObjVertexBase {
    protected:
        Eigen::RowVector4f v_;
    public:
        operator const Eigen::RowVector4f&() const {
            return v_;
        }

        std::istream& process_coordinates(std::istream  &input)
        {
            float c[4];
            input >> c[0];
            input >> c[1];
            input >> c[2];
            c[3] = 1.0;
            input >> c[3];

            v_ << c[0], c[1], c[2], c[3];
            return input;
        }
};

class GeometricVertex : public ObjVertexBase{
    public:
        GeometricVertex() {}
        friend std::istream &operator>>( std::istream  &input, GeometricVertex &v );
        void storeInModel(trim::TriangleModel& tm);
};

class TextureVertex : public ObjVertexBase {
    public:
        TextureVertex() {}
        friend std::istream &operator>>( std::istream  &input, TextureVertex &v ); 
        void storeInModel(trim::TriangleModel& tm);
};

class VertexNormal : public ObjVertexBase {
    public:
        VertexNormal() {}
        friend std::istream &operator>>( std::istream  &input, VertexNormal &v ); 
        void storeInModel(trim::TriangleModel& tm);
};

class Surface {
    private:
        std::vector<int> vRefs_;
        std::vector<int> tvRefs_;
        std::vector<int> vnRefs_;
    public:
        Surface() {};
        const std::vector<int>& getVRefs() const;
        const std::vector<int>& getTvRefs() const;
        const std::vector<int>& getVnRefs() const;

        friend std::istream &operator>>( std::istream  &input, Surface &s ); 

        void makeRefsAbsolute(int vCount, int tvCount, int vnCount);

        void debug() {
            std::cout << "surface " << std::endl;
            for (unsigned int i=0; i < vRefs_.size(); ++i)
            {
                std::cout << vRefs_[i] << '/'
                          << tvRefs_[i] << '/'
                          << vnRefs_[i] << std::endl;
            }
        }
};

}
#endif
