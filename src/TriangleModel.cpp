#include <TriangleModel.hpp>

#include <Exception.hpp>

#include <memory>
#include <limits> 

#include <iostream>

namespace trim {

template <class T>
struct Singleton {
    static T& instance()
    {
        static T t;
        return t;
    }
};

class MatrixManager : public Singleton <MatrixManager> {
    private:
        std::vector<MatrixSelector> _matrixReturned;
        std::vector<std::shared_ptr<TriangleModel::ModelMatrix>> _matrixes;
    public:
        MatrixManager() {};
        MatrixSelector allocateNewMatrix();
        void releaseMatrix(MatrixSelector m);
        TriangleModel::ModelMatrix& getMatrix(MatrixSelector m);
};

MatrixSelector MatrixManager::allocateNewMatrix()
{
    unsigned mRows=1048576;
    unsigned mCols=4;
    if (_matrixReturned.empty())
    {
        if (std::numeric_limits<MatrixSelector>::max() == _matrixes.size())
            throw exception::GenericException("No more slots left for Model Matrixes");

        _matrixes.push_back(std::make_shared<TriangleModel::ModelMatrix>(mRows, mCols));
        return (_matrixes.size() - 1);
    }
    
    MatrixSelector m = _matrixReturned.back();
    _matrixes[m] = std::make_shared<TriangleModel::ModelMatrix>(mRows, mCols);
    _matrixReturned.pop_back();
    return m;
}

void MatrixManager::releaseMatrix(MatrixSelector m)
{
    if (not (m < _matrixes.size()))
        throw exception::GenericException("Index out of range while trying too release a Model Matrix");
    
    _matrixReturned.push_back(m);
    _matrixes[m].reset();
}

TriangleData::TriangleData(VertexRef a, VertexRef b, VertexRef c)
{
    _vertex[0] = a;
    _vertex[1] = b;
    _vertex[2] = c;
}

VertexRef TriangleData::operator()(int index) const
{
    return _vertex[index];
}

float TriangleData::operator()(int vertexIndex, int coordinate) const
{
    return MatrixManager::instance().getMatrix(_matrixSelector)(_vertex[vertexIndex], coordinate);
}

TriangleModel::ModelMatrix& MatrixManager::getMatrix(MatrixSelector m)
{
    if (not (m < _matrixes.size()))
        throw exception::GenericException("Index out of range while trying too access a Model Matrix");

    return *_matrixes[m];
}

TriangleModel::TriangleModel() : _vertexMatrix(0), _texturesMatrix(0), _normalMatrix(0),
                                 _lastIndexVertex(0), _lastIndexTexture(0), _lastIndexNormal(0)
{
    _vertexMatrix = MatrixManager::instance().allocateNewMatrix();
    _texturesMatrix = MatrixManager::instance().allocateNewMatrix();
    _normalMatrix = MatrixManager::instance().allocateNewMatrix();
}

unsigned int TriangleModel::addRow(MatrixSelector m, const Eigen::RowVector4f& v, unsigned int& lastIndex)
{
    ModelMatrix &mm = MatrixManager::instance().getMatrix(m);
    if (lastIndex >= mm.rows())
        mm.conservativeResize(mm.rows() * 1.5, mm.cols());

    mm.row(lastIndex) = v;  
    return lastIndex++;
}

void TriangleModel::addTriangle(const TriangleData& triangle,
                                const TriangleData& triangleTexture,
                                const TriangleData& triangleNormals)
{
    _triangles.push_back(triangle);
    _triangles.back()._matrixSelector = _vertexMatrix;
    _triangleTextures.push_back(triangleTexture);
    _triangleTextures.back()._matrixSelector = _texturesMatrix;
    _triangleNormals.push_back(triangleNormals);
    _triangleNormals.back()._matrixSelector = _normalMatrix;
}
    
const std::vector<TriangleData>& TriangleModel::getTriangles() const
{
    return _triangles;
}

const std::vector<TriangleData>& TriangleModel::getTriangleTextures() const
{
    return _triangleTextures;
}

const std::vector<TriangleData>& TriangleModel::getTriangleNormals() const
{
    return _triangleNormals;
}

const TriangleModel::ModelMatrix& TriangleModel::getVerticleMatrix() const
{
    return MatrixManager::instance().getMatrix(_vertexMatrix);
}

const TriangleModel::ModelMatrix& TriangleModel::getTextureMatrix() const
{
    return MatrixManager::instance().getMatrix(_texturesMatrix);
}
    
const TriangleModel::ModelMatrix& TriangleModel::getNormalMatrix() const
{
    return MatrixManager::instance().getMatrix(_normalMatrix);
}

unsigned int TriangleModel::addVertex(const Eigen::RowVector4f& v)
{
    return addRow(_vertexMatrix, v, _lastIndexVertex);
}

unsigned int TriangleModel::addTexture(const Eigen::RowVector4f& v)
{
    return addRow(_texturesMatrix, v, _lastIndexTexture);
}

unsigned int TriangleModel::addNormalVector(const Eigen::RowVector4f& v)
{
    return addRow(_normalMatrix, v, _lastIndexNormal);
}

void TriangleModel::applyTransformatioMatrix(const TransFormationMatrix& trMatrix,
                                             const TransFormationMatrix& trMatrixNormal)
{
    ModelMatrix& mv = MatrixManager::instance().getMatrix(_vertexMatrix);
    mv *= trMatrix;
    ModelMatrix& tx = MatrixManager::instance().getMatrix(_texturesMatrix);
    tx *= trMatrix;
    ModelMatrix& mn = MatrixManager::instance().getMatrix(_normalMatrix);
    mn *= trMatrixNormal;
}

TriangleModel::~TriangleModel()
{
    MatrixManager::instance().releaseMatrix(_vertexMatrix);
    MatrixManager::instance().releaseMatrix(_texturesMatrix);
    MatrixManager::instance().releaseMatrix(_normalMatrix);
}

}// end of namespace trim