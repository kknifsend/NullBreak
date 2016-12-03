
#include "Utilities\Timer.hpp"

#include <chrono>

struct Timer::Impl
{
    typedef std::chrono::high_resolution_clock Clock;
    Clock::time_point mStart;

    bool mIsPaused;
    Clock::duration mTimeBeforePause;

    Impl()
        : mIsPaused(false)
    {}

    void Start();
    double Stop();

    void Pause();
    void Resume();
};

////////////////////////////////////////////

Timer::Timer()
    : mpImpl(new Impl) {}

////////////////////////////////////////////

Timer::~Timer()
{
    delete mpImpl;
}

////////////////////////////////////////////

void Timer::Start()
{
    mpImpl->Start();
}

////////////////////////////////////////////

void Timer::Impl::Start()
{
    mIsPaused = false;
    mTimeBeforePause = std::chrono::seconds(0);
    mStart = Clock::now();
}

////////////////////////////////////////////

double Timer::Stop()
{
    return mpImpl->Stop();
}

////////////////////////////////////////////

double Timer::Impl::Stop()
{
    Clock::duration duration = mTimeBeforePause;
    if (mIsPaused == false)
    {
        duration += (Clock::now() - mStart);
    }

    const auto timeNs =
        std::chrono::duration_cast<std::chrono::nanoseconds>(duration);

    return (double)timeNs.count() * 1e-9;
}

////////////////////////////////////////////

void Timer::Pause()
{
    mpImpl->Pause();
}

////////////////////////////////////////////

void Timer::Impl::Pause()
{
    mTimeBeforePause = Clock::now() - mStart;

    mIsPaused = true;
}

////////////////////////////////////////////

void Timer::Resume()
{
    mpImpl->Resume();
}

////////////////////////////////////////////

void Timer::Impl::Resume()
{
    mStart = Clock::now();

    mIsPaused = false;
}

////////////////////////////////////////////
