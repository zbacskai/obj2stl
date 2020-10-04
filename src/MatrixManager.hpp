#ifndef MatrixManager_hpp
#define MatrixManager_hpp

#include <TriangleModel.hpp>

#include <vector>

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

} // end of namespace trim

#endif