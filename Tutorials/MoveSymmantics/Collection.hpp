#pragma once

#include <cassert>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <initializer_list>
#include <memory>
#include <cstdint>

#define ENABLE_MOVE 0

template <class T>
class Collection
{
public:
    Collection();
    Collection(size_t numOfElements);
    Collection(size_t numOfElements, const T& rValue);
    Collection(const std::initializer_list<T>& rInitializer);
    Collection(const Collection& rCollection);
    ~Collection();

    Collection& operator=(const Collection& rCollection);

    Collection(Collection&& rCollection);
    Collection& operator=(Collection&& rCollection);

    T& operator[](int32_t index);
    const T& operator[](int32_t index) const;
    size_t size() const;
private:
    size_t mSize;
    std::unique_ptr<T[]> mpData;
};

////////////////////////////////////////////////////////

template <class T>
Collection<T>::Collection()
    : mSize(0), mpData(nullptr)
{
}

////////////////////////////////////////////////////////

template <class T>
Collection<T>::Collection(size_t numOfElements)
    : mSize(numOfElements), mpData(new T[mSize])
{
    std::cout << "Constructor size_t\n";
}

////////////////////////////////////////////////////////

template <class T>
Collection<T>::Collection(size_t numOfElements, const T& rValue)
    : mSize(numOfElements), mpData(new T[mSize])
{
    std::cout << "Constructor size_t, value\n";

    T* pData = mpData.get();
    std::fill(pData, pData + mSize, rValue);
}

////////////////////////////////////////////////////////

template <class T>
Collection<T>::Collection(const std::initializer_list<T>& rInitializer)
    : mSize(rInitializer.size()), mpData(new T[mSize])
{
    std::cout << "Constructor initializer_list\n";

    T* pData = mpData.get();

    std::copy(rInitializer.begin(), rInitializer.end(), pData);
}

////////////////////////////////////////////////////////

template <class T>
Collection<T>::Collection(const Collection& rCollection)
    : mSize(rCollection.mSize), mpData(new T[mSize])
{
    std::cout << "Copy Constructor\n";

    T* pData = mpData.get();
    T* pOtherData = rCollection.mpData.get();

    std::copy(pOtherData, pOtherData + mSize, pData);
}

////////////////////////////////////////////////////////

template <class T>
Collection<T>& Collection<T>::operator=(const Collection& rCollection)
{
    std::cout << "Assignment Operator\n";

    if (&rCollection != this)
    {
        mSize = rCollection.mSize;
        mpData.reset(new T[mSize]);

        T* pData = mpData.get();
        T* pOtherData = rCollection.mpData.get();
        std::copy(pOtherData, pOtherData + mSize, pData);
    }
    return *this;
}

////////////////////////////////////////////////////////

template <class T>
Collection<T>::~Collection()
{
    if (mpData != nullptr)
    {
        std::cout << "~Collection()\n";
    }
}

////////////////////////////////////////////////////////

#if ENABLE_MOVE
template <class T>
Collection<T>::Collection(Collection<T>&& rCollection)
{
    std::cout << "Move Constructor\n";

    mSize = rCollection.mSize;

    T* pData = rCollection.mpData.release();
    mpData.reset(pData);
}

////////////////////////////////////////////////////////

template <class T>
Collection<T>& Collection<T>::operator=(Collection<T>&& rCollection)
{
    std::cout << "Move Assignment Operator\n";

    mSize = rCollection.mSize;

    T* pData = rCollection.mpData.release();
    mpData.reset(pData);
}

#endif

////////////////////////////////////////////////////////

template <class T>
T& Collection<T>::operator[](int32_t index)
{
    assert((index >= 0) && (index < mSize));
    return mpData[index];
}

////////////////////////////////////////////////////////

template <class T>
const T& Collection<T>::operator[](int32_t index) const
{
    assert((index >= 0) && (index < mSize));
    return mpData[index];
}

////////////////////////////////////////////////////////

template <class T>
size_t Collection<T>::size() const
{
    return mSize;
}