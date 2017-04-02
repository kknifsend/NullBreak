

#include <iostream>
#include <random>
#include <chrono>

#include "TransformFit.hpp"

#include <Eigen/Dense>
using namespace Eigen;

///////////////////////////////////////////////

template <class T>
void Check(const Transformation::Params& rOrigParams,
           const Transformation::Params& rEstParams,
           const Transformation::Points<T>& rPoints0,
           const Transformation::Points<T>& rPoints1);

template <class T>
double ComputeErrorSum(
    const Transformation::Params& rParams,
    const Transformation::Points<T>& rPoints0,
    const Transformation::Points<T>& rPoints1);

std::default_random_engine& GetGenerator();

template <class T>
void GenerateData(
    size_t numOfPoints,
    Transformation::Points<T>& rPoints);

template <class T>
void GenerateTransform(
    const Transformation::Points<T>& rPoints0,
    Transformation::Points<T>& rPoints1,
    Transformation::Params& rParams);

///////////////////////////////////////////////

int main()
{
    const size_t numOfPoints = 100000;
    // Generate data
    Transformation::Points<Vector3d> points0, points1;
    GenerateData(numOfPoints, points0);

    // Generate transform
    Transformation::Params origParams;
    GenerateTransform(points0, points1, origParams);

    // Perform fit
    std::cout << "Running Fit..\n";
    Transformation::Params estParams;
    Transformation::Fit(points0, points1, estParams);

    std::cout << "Result Statistics\n";
    Check(origParams, estParams, points0, points1);

    std::cin.get();
    return 0;
}

///////////////////////////////////////////////

template <class T>
void Check(
    const Transformation::Params& rOrigParams,
    const Transformation::Params& rEstParams,
    const Transformation::Points<T>& rPoints0,
    const Transformation::Points<T>& rPoints1)
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
    std::cout << deltaTrans[2] << "> \n";
    std::cout << "    Norm: " << deltaTrans.norm() << " m\n";

    // Determine rotation difference
    const Eigen::Quaterniond deltaRotation =
        rOrigParams.mRotation * rEstParams.mRotation.conjugate();
    const Eigen::Matrix3d deltaMatrix =
        deltaRotation.toRotationMatrix() - Eigen::Matrix3d::Identity();
    std::cout << "Delta Rotation Norm: " << deltaMatrix.norm() << "\n";

    // Determine scale difference
    const double deltaScale = fabs(rOrigParams.mScale - rEstParams.mScale);
    std::cout << "Delta Scale: " << deltaScale << "\n";

    const double origErrorSum = ComputeErrorSum(rOrigParams, rPoints0, rPoints1);
    const double estErrorSum = ComputeErrorSum(rEstParams, rPoints0, rPoints1);

    std::cout << "Original Error Sum: " << origErrorSum << " m\n";
    std::cout << "Est. Error Sum: " << estErrorSum << " m\n";
}

///////////////////////////////////////////////

template <class T>
double ComputeErrorSum(
    const Transformation::Params& rParams,
    const Transformation::Points<T>& rPoints0,
    const Transformation::Points<T>& rPoints1)
{
    const size_t numOfPoints = rPoints0.size();
    const size_t numOfPoints1 = rPoints1.size();
    if ((numOfPoints != numOfPoints1) || (numOfPoints == 0))
    {
        return -1.0;
    }

    Eigen::Affine3d dblTransform = Eigen::Affine3d::Identity();
    dblTransform.translate(rParams.mTranslation);
    dblTransform.rotate(rParams.mRotation);
    dblTransform.scale(rParams.mScale);

    const auto transform = dblTransform.cast<T::Scalar>();

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
    Transformation::Points<T>& rPoints)
{
    rPoints.clear();
    std::default_random_engine& rGenerator = GetGenerator();

    const T::Scalar extents = (T::Scalar)100.0;
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
    const Transformation::Points<T>& rPoints0,
    Transformation::Points<T>& rPoints1,
    Transformation::Params& rParams)
{
    rPoints1.clear();
    if (rPoints0.empty())
    {
        return;
    }
    std::default_random_engine& rGenerator = GetGenerator();

    const double extent = 10.0;
    const std::uniform_real_distribution<double> distExtent(-extent, extent);

    // Generate translation and rotation axis
    Vector3d translation = Vector3d::Zero();
    Vector3d axis = Vector3d::Zero();
    for (uint8_t i = 0; i < 3; ++i)
    {
        translation[i] = distExtent(rGenerator);
        axis[i] = distExtent(rGenerator);
    }
    axis.normalize();

    // Generate scale
    const double minScale = 0.5, maxScale = 2.0;
    const std::uniform_real_distribution<double> distScale(minScale, maxScale);
    const double scale = distScale(rGenerator);

    // Generate rotation angle
    const double minAngle = 0.0, maxAngle = 2.0 * M_PI;
    const std::uniform_real_distribution<double> distAngle(minAngle, maxAngle);
    const Eigen::AngleAxis<double> rotation(distAngle(rGenerator), axis);
    const Eigen::Quaternion<double> quaternion(rotation);

    // Compose transformation
    Affine3d transform = Affine3d::Identity();
    transform.translate(translation);
    transform.rotate(rotation);
    transform.scale(scale);

    // Apply transformation
    const size_t numOfPoints = rPoints0.size();
    rPoints1.resize(numOfPoints);
    for (size_t i = 0; i < numOfPoints; ++i)
    {
        const Vector3d transformedPoint0 = (transform * rPoints0[i].cast<double>());
        rPoints1[i] = transformedPoint0.cast<T::Scalar>();
    }

    rParams.mTranslation = translation;
    rParams.mRotation = quaternion;
    rParams.mScale = scale;
}

///////////////////////////////////////////////
