#include "demo-common.h"

#include <atomic>
#include <condition_variable>
#include <future>
#include <mutex>
#include <shared_mutex>
#include <thread>

DEMO(thread)
{
    // std::thread - объект, представляющий поток.

    const auto task = []() {
        std::cout << "new thread\n";
    };

    std::thread thread1(task);
    std::thread thread2(task);

    // std::thread поддерживает перемещение, копирование запрещено
    std::thread thread3(std::move(thread1));

    thread2.join(); // ожидает, пока поток закончит свое выполнение

    thread3.detach(); // позволяет потоку выполняться независимо, делать join больше не нужно

    size_t n = std::thread::hardware_concurrency(); // количество физически поддерживаемых потоков
    std::cout << n << " concurrent threads are supported\n";
}

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

    std::shared_mutex mutex;

    const auto writer = [&]() {
        std::unique_lock lock(mutex);
        std::cout << "Writer: lock\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Writer: free\n";
    };

    const auto reader = [&](int number) {
        std::shared_lock locK(mutex);
        std::cout << "Reader #" << number << ": lock\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Reader #" << number << ": free\n";
    };

    std::thread writer_thread(writer);

    std::vector<std::thread> reader_threads;
    for (size_t i = 0; i < 3; ++i)
        reader_threads.emplace_back(reader, i);

    writer_thread.join();
    for (auto& thread : reader_threads)
        thread.join();
}

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

DEMO(atomic)
{
    // std::atomic позволяет создать атомарную переменную. Поддерживаются
    // * все целочисленные типы
    // * все указатели
    // * bool

    std::atomic<size_t> count = 0;

    const auto task = [&count]() {
        for (size_t i = 0; i < 10000; ++i)
            count++;
    };

    std::vector<std::thread> threads;
    for (size_t i = 0; i < 6; ++i)
        threads.emplace_back(task);

    for (auto& thread : threads)
        thread.join();

    std::cout << count << "\n";
}

DEMO(condition_variable)
{
    // std::condition_variable позволяет заблокировать работу потока,
    // пока не будет получен сигнал из управляющего потока.

    // Поток, желающий изменить переменную должен:
    // 1) заблокировать std::mutex
    // 2) произвести изменения, пока std::mutex заблокирован
    // 3) вызвать notify_one или notify_all

    // Поток, ждущий на условной переменной должен:
    // 1) заблокировать std::mutex через std::unique_lock<std::mutex>
    // 2) вызвать wait, wait_for или wait_until. Эти операции заставляют поток заснуть до того,
    //    как в другом потоке будет вызван notify_one или notify_all или произойдет ложное срабатывание
    // 3) когда поток проснется, мьютекс будет автоматически заблокирован

    bool worker_notified = false;
    bool main_thread_notified = false;

    std::condition_variable cv;
    std::mutex mutex;

    int count = 0;

    const auto worker_task = [&](){
        std::cout << "Worker thread starts\n";

        std::unique_lock lock(mutex);

        // Второй аргумент - защита от ложных срабатываний.
        // Переданная функция должна вернуть true, если срабатывание верно.
        cv.wait(lock, [&]{return worker_notified;});

        std::cout << "Worker got signal and starts work\n";

        // После выхода из wait lock уже заблокирован
        count = count * 2 + 1;

        std::cout << "Worker ends work\n";

        main_thread_notified = true;
        cv.notify_all(); // посылает уведомление всем ждущим потокам
    };

    std::thread worker(worker_task);

    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    std::cout << "Main thread starts work\n";

    {
        std::lock_guard lock(mutex);
        count = 100;
        worker_notified = true;
    }

    std::cout << "Main thread ends work\n";

    cv.notify_one(); // посылает уведомление одному любому ждущему потоку

    // Ожидание результата от worker
    {
        std::unique_lock lock(mutex);
        cv.wait(lock, [&]{return main_thread_notified;});
        std::cout << "Main thread got signal\n";
        std::cout << "Result: " << count << "\n";
    }

    worker.join();
}

RUN_DEMOS
