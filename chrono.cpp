#include "common.h"

#include <chrono>
#include <thread>

DEMO(chrono)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count()
              << " seconds passed\n";
}

RUN_DEMOS
