#pragma once

#include <Eigen/Dense>
#include <vector>

namespace Transformation
{

struct Params
{
    Eigen::Vector3d mTranslation;
    Eigen::Quaterniond mRotation;
    double mScale;

    Params()
        : mTranslation(Eigen::Vector3d::Zero()),
          mRotation(Eigen::Quaterniond::Identity()), mScale(1.0)
    {}
};

template<class T>
using Points = std::vector<T, Eigen::aligned_allocator<T>>;
typedef Points<Eigen::Vector3d> Points3d;
typedef Points<Eigen::Vector2d> Points2d;
typedef Points<Eigen::Vector3d> Points3f;
typedef Points<Eigen::Vector2d> Points2f;

template <class T>
void Fit(const Points<T>& rPoints0, const Points<T>& rPoints1, Params& rParams);

// Finds a transformation from pointset 0 to pointset 1.
template <class T>
void Fit(
    const T* pPoints0, const T* pPoints1,
    size_t numOfPoints, Params& rParams);

}