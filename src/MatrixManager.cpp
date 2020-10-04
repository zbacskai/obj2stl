/**
  *
  * Copyright 2020 Zoltan Bacskai
  *
  **/
#include <MatrixManager.hpp>
#include <Exception.hpp>

#include <limits>

namespace trim {

MatrixSelector MatrixManager::allocateNewMatrix()
{
    unsigned mRows=1048576;
    unsigned mCols=4;
    if (_matrixReturned.empty()) {
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

}// end of namespace trim