#include "demo-common.h"

#include <future>
#include <mutex>
#include <shared_mutex>
#include <thread>

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

/*DEMO(shared_mutex)
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
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    };

    std::thread thread1(changer);
    std::thread thread2(printer);

    thread1.join();
    thread2.join();
}*/

DEMO(sleep)
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

DEMO(promise)
{
    // std::promise хранит объект (или исключение), который в дальнейшем может быть
    // ассинхронно затребован через std::future.

    const auto worker = [](std::promise<int> promise) {
        int result = 1 + 2;
        promise.set_value(result);
    };

    std::promise<int> promise;
    std::future<int> future = promise.get_future();

    std::thread thread(worker, std::move(promise));
    thread.detach();

    std::cout << "Future contains: " << future.get() << "\n";
}

DEMO(packaged_task)
{
    // std::packaged_task хранит функцию и позволяет получить доступ к ее результату через std::future.

    const auto func = []() {
        int count = 0;
        for (size_t i = 1; i <= 4; ++i)
            count += i;
        return count;
    };

    std::packaged_task<int(void)> task(func);

    std::future<int> future = task.get_future();

    task(); // целесообразно выполнять в другом потоке

    std::cout << "task result: " << future.get() << "\n";
}

DEMO(async)
{
    // std::async запускает функцию асинхронно (или сохраняет функцию для ленивого вычисления)
    // и возвращает std::future, через который можно получить результат вызова функции.

    const auto func = [](int a) {
        return a + a;
    };

    std::future<int> future1 = std::async(std::launch::async, func, 1); // запускает func в отдельном потоке
    std::future<int> future2 = std::async(std::launch::deferred, func, 2); // запустит func, когда будет востребован ее результат
    std::future<int> future3 = std::async(func, 3); // может быть использована любая политика запуска (эквивалентно std::launch::async | std::launch::deferred)

    std::cout << "future1: " << future1.get() << "\n";
    std::cout << "future2: " << future2.get() << "\n";
    std::cout << "future3: " << future3.get() << "\n";
}

RUN_DEMOS
