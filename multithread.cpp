#include "demo-common.h"

#include <thread>
#include <mutex>
#include <shared_mutex>

DEMO(mutex)
{
    size_t count = 0;

    std::mutex mutex;

    const auto thread_proc = [&count, &mutex](const std::string& label) {
        std::cout << label << " started\n";
        for (size_t i = 0; i < 100000; ++i)
        {
            std::lock_guard<std::mutex> lock(mutex);
            count++;
        }
    };

    std::thread thread1(thread_proc, "thread1");
    std::thread thread2(thread_proc, "thread2");

    // деструктор std::thread сам не вызовет join,
    // и, если поток не закончен, будет вызван std::terminate.
    thread1.join();
    thread2.join();

    std::cout << "count: " << count << "\n";
}

DEMO(unique_lock)
{
    std::mutex mutex1, mutex2, mutex3;

    std::unique_lock<std::mutex> lock1(mutex1); // блокирует мьютекс при создании

    std::unique_lock<std::mutex> lock2(mutex2, std::defer_lock); // эти мьютексы не заблокируются
    std::unique_lock<std::mutex> lock3(mutex3, std::defer_lock); // при создании std::unique_lock

    std::lock(lock2, lock3); // блокирует оба лока
}

DEMO(shared_mutex)
{
    // std::shared_mutex поддерживает два режима блокировки
    // * для чтения - через std::shared_lock или метод lock_shared
    // * для изменения - через std::unique_lock или метод lock

    size_t count = 0;
    std::shared_mutex mutex;

    const size_t count_max = 10;

    const auto changer = [&]() {
        for (size_t i = 0; i < count_max; ++i)
        {
            std::unique_lock lock(mutex);
            count++;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    };

    const auto printer = [&]() {
        while (count != count_max)
        {
            std::shared_lock locK(mutex);
            std::cout << "current count: " << count << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    };

    std::thread thread1(changer);
    std::thread thread2(printer);

    thread1.join();
    thread2.join();
}

DEMO(sleep)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

RUN_DEMOS
