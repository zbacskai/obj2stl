#
#ifndef TriangleModel_hpp
#define TriangleModel_hpp

#include <vector>
#include <memory>
#include <algorithm>
#include <eigen3/Eigen/Dense>

namespace trim
{

typedef int32_t VertexRef;
typedef uint16_t MatrixSelector;

class TriangleData
{
private:
    VertexRef _vertex[3];
    uint16_t _matrixSelector;
    uint16_t _reserved;
public:
    TriangleData(VertexRef a, VertexRef b,
                 VertexRef c);

    friend class TriangleModel;

    VertexRef operator()(int index) const;

    float operator()(int vertexIndex, int coordinate) const;
};

class TriangleModel
{
private:
    MatrixSelector _vertexMatrix;
    MatrixSelector _texturesMatrix;
    MatrixSelector _normalMatrix;
    unsigned int _lastIndexVertex;
    unsigned int _lastIndexTexture;
    unsigned int _lastIndexNormal;
    std::vector<TriangleData> _triangles;
    std::vector<TriangleData> _triangleTextures;
    std::vector<TriangleData> _triangleNormals;

    unsigned int addRow(MatrixSelector m, const Eigen::RowVector4f& v, unsigned int& lastIndex);

    TriangleModel(const TriangleModel&); // not implemented
    TriangleModel& operator=(const TriangleModel&); //not implemented
public:
    TriangleModel();
    void addTriangle(const TriangleData &verticles,
                     const TriangleData& triangleTexture = TriangleData(0, 0, 0),
                     const TriangleData& triangleNormals = TriangleData(0, 0, 0));
    const std::vector<TriangleData>& getTriangles() const;
    const std::vector<TriangleData>& getTriangleTextures() const;
    const std::vector<TriangleData>& getTriangleNormals() const;

    typedef Eigen::Matrix<float, Eigen::Dynamic, 4> ModelMatrix;
    typedef Eigen::Matrix4f TransFormationMatrix;

    const ModelMatrix& getVerticleMatrix() const;
    const ModelMatrix& getTextureMatrix() const;
    const ModelMatrix& getNormalMatrix() const;

    unsigned int addVertex(const Eigen::RowVector4f& v);
    unsigned int addTexture(const Eigen::RowVector4f& v);
    unsigned int addNormalVector(const Eigen::RowVector4f& v);

    void applyTransformatioMatrix(const TransFormationMatrix& trMatrix,
                                  const TransFormationMatrix& trMatrixNormal);

    ~TriangleModel();
};

} //end od namespace trim
#endif