
#include "TransformFit.hpp"

#include <Eigen/Dense>

#include <iostream>

using namespace Eigen;

namespace Transformation
{

///////////////////////////////////////////////

template void Fit<Vector3d>(
    const Points<Vector3d>& rPoints0,
    const Points<Vector3d>& rPoints1,
    Params& rParams);

template void Fit<Vector3f>(
    const Points<Vector3f>& rPoints0,
    const Points<Vector3f>& rPoints1,
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

/*
https://pdfs.semanticscholar.org/b191/4c55ec9d21588c35dbf6a85f2d9631bb94cf.pdf
http://www.ltu.se/cms_fs/1.51590!/svd-fitting.pdf
https://igl.ethz.ch/projects/ARAP/svd_rot.pdf
*/
template void Fit<Vector3d>(
    const Vector3d* pPoints0,
    const Vector3d* pPoints1,
    size_t numOfPoints,
    Params& rParams);

template void Fit<Vector3f>(
    const Vector3f* pPoints0,
    const Vector3f* pPoints1,
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

    Vector3d centroid0 = Vector3d::Zero();
    Vector3d centroid1 = Vector3d::Zero();
    for (size_t i = 0; i < numOfPoints; ++i)
    {
        centroid0 += pPoints0[i].cast<double>();
        centroid1 += pPoints1[i].cast<double>();
    }
    const double invNumOfPoints = 1.0 / (double)numOfPoints;
    centroid0 *= invNumOfPoints;
    centroid1 *= invNumOfPoints;

    Matrix3d matrixSum = Matrix3d::Zero();
    double sqrNormSum0 = 0.0, sqrNormSum1 = 0.0;
    for (size_t i = 0; i < numOfPoints; ++i)
    {
        const Vector3d offsetPoint0 = (pPoints0[i].cast<double>() - centroid0);
        const Vector3d offsetPoint1 = (pPoints1[i].cast<double>() - centroid1);

        sqrNormSum0 += offsetPoint0.squaredNorm();
        sqrNormSum1 += offsetPoint1.squaredNorm();

        matrixSum += offsetPoint1 * offsetPoint0.transpose();
    }

    const JacobiSVD<Matrix3d> svd(
        matrixSum, ComputeFullU | ComputeFullV);
    const Matrix3d rotation = svd.matrixU() * svd.matrixV().transpose();

    const double scale = sqrt(sqrNormSum1 / sqrNormSum0);
    const Eigen::Vector3d translation = centroid1 - scale * rotation * centroid0;

    rParams.mRotation = Quaterniond(rotation);
    rParams.mTranslation = translation;
    rParams.mScale = scale;
}

//template <>
//void Fit(
//    const Points<Vector2f>& rPoints0,
//    const Points<Vector2f>& rPoints1,
//    Params& rParams);
//
//template <>
//void Fit(
//    const Points<Vector2d>& rPoints0,
//    const Points<Vector2d>& rPoints1,
//    Params& rParams);

}