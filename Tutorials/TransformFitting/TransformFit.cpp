
#include "TransformFit.hpp"

#include <Eigen/Dense>

namespace Transform
{

///////////////////////////////////////////////

template void Fit<Eigen::Vector3d>(
    const Points<Eigen::Vector3d>& rPoints0,
    const Points<Eigen::Vector3d>& rPoints1,
    Params& rParams);

template void Fit<Eigen::Vector3f>(
    const Points<Eigen::Vector3f>& rPoints0,
    const Points<Eigen::Vector3f>& rPoints1,
    Params& rParams);

template <class T>
void Fit(
    const Points<T>& rPoints0, const Points<T>& rPoints1,
    Params& rParams)
{
    const size_t numOfPoints0 = rPoints0.size();
    const size_t numOfPoints1 = rPoints1.size();
    if (numOfPoints0 != numOfPoints1)
    {
        return;
    }

    Fit(rPoints0.data(), rPoints1.data(), numOfPoints0, rParams);
}

///////////////////////////////////////////////

template void Fit<Eigen::Vector3d>(
    const Eigen::Vector3d* pPoints0,
    const Eigen::Vector3d* pPoints1,
    size_t numOfPoints,
    Params& rParams);

template void Fit<Eigen::Vector3f>(
    const Eigen::Vector3f* pPoints0,
    const Eigen::Vector3f* pPoints1,
    size_t numOfPoints,
    Params& rParams);

template <class T>
void Fit(
    const T* pPoints0, const T* pPoints1,
    size_t numOfPoints, Params& rParams)
{
    // We need a minimum number of 3 points
    if (numOfPoints < 3)
    {
        return;
    }
    else if (numOfPoints == 3)
    {
        // Use minimal implementation
        return;
    }


    T centroid0 = T::Zero();
    T centroid1 = T::Zero();
    for (size_t i = 0; i < numOfPoints; ++i)
    {
        centroid0 += pPoints0[i];
        centroid1 += pPoints1[i];
    }
    const T::Scalar invNumOfPoints = (T::Scalar)(1.0 / numOfPoints);
    centroid0 *= invNumOfPoints;
    centroid1 *= invNumOfPoints;

    typedef Eigen::Matrix<T::Scalar, 3, 3> Matrix;
    Matrix matrixSum = Matrix::Zero();
    for (size_t i = 0; i < numOfPoints; ++i)
    {
        matrixSum += (pPoints0[i] - centroid0) * (pPoints1[i] - centroid1).transpose();
    }

    const Eigen::JacobiSVD<Matrix> svd(
        matrixSum, Eigen::ComputeThinU | Eigen::ComputeThinV);
    const Matrix u = svd.matrixU();
    const Matrix v = svd.matrixV();
    const Matrix rotation = u * v.transpose();

    rParams.mTranslation = (centroid1 - centroid0).cast<double>();
    rParams.mRotation = Eigen::Quaterniond(rotation.cast<double>());
}

//template <>
//void Fit(
//    const Points<Eigen::Vector2f>& rPoints0,
//    const Points<Eigen::Vector2f>& rPoints1,
//    Params& rParams);
//
//template <>
//void Fit(
//    const Points<Eigen::Vector2d>& rPoints0,
//    const Points<Eigen::Vector2d>& rPoints1,
//    Params& rParams);

}