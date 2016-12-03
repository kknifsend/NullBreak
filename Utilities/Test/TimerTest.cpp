
#include <chrono>
#include <iostream>
#include <thread>
using namespace std;

#include "Utilities\Timer.hpp"

int main()
{
    Timer timer;
    {
        std::cout << "Sleeping for 1 second.. ";
        timer.Start();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        const double elapsedSeconds = timer.Stop();

        std::cout << "ElapsedTime = " << elapsedSeconds << " s\n";
    }

    {
        std::cout << "Sleeping for 2 second.. ";
        timer.Start();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        timer.Pause();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        timer.Resume();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        const double elapsedSeconds = timer.Stop();

        std::cout << "ElapsedTime = " << elapsedSeconds << " s\n";
    }

    std::cin.get();
    return 0;
}