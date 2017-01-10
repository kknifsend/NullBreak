

#include <iostream>
#include <random>
#include <chrono>

#include "TransformFit.hpp"

#include <Eigen/Dense>

///////////////////////////////////////////////

template <class T>
void Check(const Transform::Params& rOrigParams,
           const Transform::Params& rEstParams,
           const Transform::Points<T>& rPoints0,
           const Transform::Points<T>& rPoints1);

template <class T>
double ComputeErrorSum(
    const Transform::Params& rParams,
    const Transform::Points<T>& rPoints0,
    const Transform::Points<T>& rPoints1);

std::default_random_engine& GetGenerator();

template <class T>
void GenerateData(
    size_t numOfPoints,
    Transform::Points<T>& rPoints);

template <class T>
void GenerateTransform(
    const Transform::Points<T>& rPoints0,
    Transform::Points<T>& rPoints1,
    Transform::Params& rParams);

///////////////////////////////////////////////

int main()
{
    const size_t numOfPoints = 100000;

    // Generate data
    Transform::Points<Eigen::Vector3d> points0, points1;
    GenerateData(numOfPoints, points0);

    // Generate transform
    Transform::Params origParams;
    GenerateTransform(points0, points1, origParams);

    // Perform fit
    std::cout << "Running Fit..\n";
    Transform::Params estParams;
    Transform::Fit(points0, points1, estParams);

    std::cout << "Result Statistics\n";
    Check(origParams, estParams, points0, points1);

    std::cin.get();
    return 0;
}

///////////////////////////////////////////////

template <class T>
void Check(
    const Transform::Params& rOrigParams,
    const Transform::Params& rEstParams,
    const Transform::Points<T>& rPoints0,
    const Transform::Points<T>& rPoints1)
{
    const size_t numOfPoints = rPoints0.size();
    const size_t numOfPoints1 = rPoints1.size();
    if (numOfPoints != numOfPoints1)
    {
        std::cout << "Error: Size mismatch!\n";
        return;
    }

    // Determine translation difference
    const Eigen::Vector3d deltaTrans =
        rOrigParams.mTranslation -rEstParams.mTranslation;

    std::cout << "Delta Translation <";
    std::cout << deltaTrans[0] << ", ";
    std::cout << deltaTrans[1] << ", ";
    std::cout << deltaTrans[2] << "> ";
    std::cout << "Norm: " << deltaTrans.norm() << " m\n";

    // Determine rotation difference
    const Eigen::Quaterniond deltaRotation =
        rOrigParams.mRotation * rEstParams.mRotation.conjugate();
    const Eigen::Matrix3d deltaMatrix =
        deltaRotation.toRotationMatrix() - Eigen::Matrix3d::Identity();
    std::cout << "Delta Rotation Norm: " << deltaMatrix.norm() << "\n";

    const double origErrorSum = ComputeErrorSum(rOrigParams, rPoints0, rPoints1);
    const double estErrorSum = ComputeErrorSum(rEstParams, rPoints0, rPoints1);

    std::cout << "Original Error Sum: " << origErrorSum << " m\n";
    std::cout << "Est. Error Sum: " << estErrorSum << " m\n";
}

///////////////////////////////////////////////

template <class T>
double ComputeErrorSum(
    const Transform::Params& rParams,
    const Transform::Points<T>& rPoints0,
    const Transform::Points<T>& rPoints1)
{
    const size_t numOfPoints = rPoints0.size();
    const size_t numOfPoints1 = rPoints1.size();
    if ((numOfPoints != numOfPoints1) || (numOfPoints == 0))
    {
        return -1.0;
    }

    typedef Eigen::Transform<T::Scalar, 3, Eigen::Isometry> Isometry3;
    Isometry3 transform = Isometry3::Identity();
    transform.translate(rParams.mTranslation);
    transform.rotate(rParams.mRotation);

    double errorSum = 0.0;
    for (size_t i = 0; i < numOfPoints; ++i)
    {
        const auto delta = (transform * rPoints0[i]) - rPoints1[i];
        errorSum += delta.norm();
    }

    return errorSum;
}

///////////////////////////////////////////////

std::default_random_engine& GetGenerator()
{
    static const uint32_t seed = (uint32_t)std::chrono::steady_clock::now()
                                 .time_since_epoch().count();
    static std::default_random_engine generator(seed);

    return generator;
}

///////////////////////////////////////////////

template <class T>
void GenerateData(
    size_t numOfPoints,
    Transform::Points<T>& rPoints)
{
    rPoints.clear();
    std::default_random_engine& rGenerator = GetGenerator();

    const double extents = 100.0;
    const std::uniform_real_distribution<T::Scalar> distExtent(-extents, extents);

    constexpr uint32_t dim = std::max(T::ColsAtCompileTime, T::RowsAtCompileTime);
    rPoints.resize(numOfPoints);
    for (size_t i = 0; i < numOfPoints; ++i)
    {
        for (size_t j = 0; j < dim; ++j)
        {
            rPoints[i][j] = distExtent(rGenerator);
        }
    }
}

///////////////////////////////////////////////

template <class T>
void GenerateTransform(
    const Transform::Points<T>& rPoints0,
    Transform::Points<T>& rPoints1,
    Transform::Params& rParams)
{
    rPoints1.clear();
    if (rPoints0.empty())
    {
        return;
    }

    std::default_random_engine& rGenerator = GetGenerator();

    const double extents = 100.0;
    const std::uniform_real_distribution<T::Scalar> distExtent(-extents, extents);

    T translation = T::Zero();
    T axis = T::Zero();
    constexpr uint32_t dim = std::max(T::ColsAtCompileTime, T::RowsAtCompileTime);
    for (size_t i = 0; i < dim; ++i)
    {
        translation[i] = distExtent(rGenerator);
        axis[i] = distExtent(rGenerator);
    }
    axis.normalize();
    //translation.setZero();

    const std::uniform_real_distribution<double> distAngle(0.0, 2.0 * M_PI);
    const double angleRad = distAngle(rGenerator);
    //const double angleRad = M_PI / 2.0;
    const Eigen::AngleAxis<T::Scalar> rotation(angleRad, axis);
    const Eigen::Quaternion<T::Scalar> quaternion(rotation);

    typedef Eigen::Transform<T::Scalar, 3, Eigen::Isometry> Isometry3;
    Isometry3 transform = Isometry3::Identity();
    transform.translate(translation);
    transform.rotate(rotation);

    // Apply transformation
    const size_t numOfPoints = rPoints0.size();
    rPoints1.resize(numOfPoints);
    for (size_t i = 0; i < numOfPoints; ++i)
    {
        rPoints1[i] = transform * rPoints0[i];
    }

    rParams.mTranslation = translation.cast<double>();
    rParams.mRotation = quaternion.cast<double>();
}

///////////////////////////////////////////////
