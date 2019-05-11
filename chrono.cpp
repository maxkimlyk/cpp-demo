#include "demo-common.h"

#include <chrono>
#include <thread>

DEMO(time_measurement)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count()
              << " seconds passed\n";

    std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - start).count()
              << " milliseconds passed\n";
}

DEMO(time_arithmetics)
{
    auto t1 = std::chrono::system_clock::now();
    auto t2 = t1 + std::chrono::hours{1} + std::chrono::minutes{2};

    std::cout << std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count()
              << " seconds bettween two timestamps\n";
}

RUN_DEMOS
