#include <obj/VertexData.hpp>

#include <algorithm> 
#include <iterator> 
#include <regex>

#include <boost/tokenizer.hpp>
#include <string>
#include <iostream>

namespace obj {
    
const double GeometricVertex::getX() const {
    return x_;
}

const double GeometricVertex::getY() const {
    return y_;
}

const double GeometricVertex::getZ() const {
    return z_;
}

const double GeometricVertex::getW() const {
    return w_;
}

std::istream& operator>>( std::istream  &input, GeometricVertex &v )
{
    v.w_ = 1.0;
    input >> v.x_;
    input >> v.y_;
    input >> v.z_;
    input >> v.w_;

    return input;
}

const double TextureVertex::getI() const {
    return i_;
}

const double TextureVertex::getJ() const {
    return j_;
}

const double TextureVertex::getK() const {
    return k_;
}

std::istream& operator>>( std::istream  &input, TextureVertex &v )
{
    input >> v.i_;
    input >> v.j_;
    input >> v.k_;

    return input;
}

const double VertexNormal::getU() const {
    return u_;
}

const double VertexNormal::getV() const {
    return v_;
}

const double VertexNormal::getW() const {
    return w_;
}

std::istream& operator>>( std::istream  &input, VertexNormal &v )
{
    input >> v.u_;
    input >> v.v_;
    input >> v.w_;

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
  typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
  std::string vertexRef;
  while (std::getline(input, vertexRef, ' '))
  {
    boost::char_separator<char> sep{"/"};
    tokenizer tok{vertexRef, sep};

    int metIndex=0;
    int out_val[3] = {0,0,0};
    for (auto beg = tok.begin(); beg != tok.end() and metIndex<3; ++beg, ++metIndex)
        if (*beg != "")
            out_val[metIndex] = std::atoi((*beg).c_str());
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