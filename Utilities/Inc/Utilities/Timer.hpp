
#pragma once

class Timer
{
public:
    Timer();
    ~Timer();

    void Start();

    // Returns time as seconds
    double Stop();

    void Pause();
    void Resume();

private:
    Timer(const Timer&);
    Timer& operator=(const Timer&);

    struct Impl;
    Impl* mpImpl;
};