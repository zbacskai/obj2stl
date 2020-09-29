#include <obj/VertexData.hpp>

#include <algorithm> 
#include <iterator> 
#include <regex>

#include <string>
#include <iostream>

namespace obj {
    
std::istream& operator>>( std::istream  &input, GeometricVertex &v )
{
    input >> v.v_.p[0];
    input >> v.v_.p[1];
    input >> v.v_.p[2];
    v.w_ = 1.0;
    input >> v.w_;
    return input;
}

std::istream& operator>>( std::istream  &input, TextureVertex &v )
{
    input >> v.v_.p[0];
    input >> v.v_.p[1];
    input >> v.v_.p[2];
    return input;
}

std::istream& operator>>( std::istream  &input, VertexNormal &v )
{
    input >> v.v_.p[0];
    input >> v.v_.p[1];
    input >> v.v_.p[2];

    return input;
}

const std::vector<int>& Surface::getVRefs() const {
    return vRefs_;
}
const std::vector<int>& Surface::getTvRefs() const {
    return tvRefs_;
}
const std::vector<int>& Surface::getVnRefs() const {
    return vnRefs_;
}

void Surface::makeRefsAbsolute(int vCount, int tvCount, int vnCount) {
    for (unsigned int i = 0; i < vRefs_.size(); ++i)
    {
        if (vRefs_[i] < 0 and vCount > 0)
            vRefs_[i] += vCount;

        if (tvRefs_[i] < 0 and tvCount > 0)
            tvRefs_[i] += tvCount;

        if (vnRefs_[i] < 0 and vnCount > 0)
            vnRefs_[i] += vnCount;

        if (vRefs_[i] > 0)
            vRefs_[i]-=1;

        if (tvRefs_[i] > 0)
            tvRefs_[i]-=1;

        if (vnRefs_[i] > 0)
            vnRefs_[i]-=1;
    }
}


std::istream &operator>>( std::istream  &input, Surface &s )
{
  std::string vertexRef;
  while (std::getline(input, vertexRef, ' '))
  {
    std::stringstream ss(vertexRef);
    std::string tok;

    int metIndex=0;
    int out_val[3] = {0,0,0};
    while (std::getline(ss, tok, '/') and metIndex <3) {
        if (tok != "")
            out_val[metIndex] = std::atoi((tok).c_str());    
        ++metIndex;
    }

    if (out_val[0] > 0)
    {
        s.vRefs_.push_back(out_val[0]);
        s.tvRefs_.push_back(out_val[1]);
        s.vnRefs_.push_back(out_val[2]);
    }
  }
  return input;
}

}// end of namespace obj