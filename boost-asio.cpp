#include <iostream>
#include <chrono>
#include <thread>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "demo-common.h"

DEMO(timer)
{
    boost::asio::io_context io;
    boost::asio::steady_timer timer(io, std::chrono::seconds(1));
    std::cout << "Waiting 1 second ..." << std::endl;
    timer.wait();
    std::cout << "Done" << std::endl;
}


DEMO(timer_async)
{
    boost::asio::io_context io;
    boost::asio::steady_timer timer(io, std::chrono::seconds(1));

    const auto timerfunc = [&timer](const boost::system::error_code& e) mutable {
        std::cout << "Done" << std::endl;
    };

    std::cout << "Waiting 1 second ..." << std::endl;
    timer.async_wait(timerfunc);
    io.run();
}

class printer
{
public:
    static constexpr auto period = std::chrono::milliseconds(200);

    printer(boost::asio::io_context& io)
        : strand_(boost::asio::make_strand(io)), timer1_(io, period), timer2_(io, period), count_(0)
    {
        timer1_.async_wait(boost::asio::bind_executor(strand_, boost::bind(&printer::print1, this)));

        timer2_.async_wait(boost::asio::bind_executor(strand_, boost::bind(&printer::print2, this)));
    }

    void print1()
    {
        if (count_ < 6)
        {
            std::cout << "Timer 1: " << count_ << std::endl;
            ++count_;
            timer1_.expires_at(timer1_.expiry() + period);
            timer1_.async_wait(
                boost::asio::bind_executor(strand_, boost::bind(&printer::print1, this)));
        }
    }

    void print2()
    {
        if (count_ < 6)
        {
            std::cout << "Timer 2: " << count_ << std::endl;
            ++count_;
            timer2_.expires_at(timer2_.expiry() + period);
            timer2_.async_wait(
                boost::asio::bind_executor(strand_, boost::bind(&printer::print2, this)));
        }
    }

private:
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;
    boost::asio::steady_timer timer1_;
    boost::asio::steady_timer timer2_;
    int count_;
};

DEMO(strand)
{
    boost::asio::io_context io;
    printer p(io);

    std::thread t(boost::bind(&boost::asio::io_context::run, &io));
    io.run();
    t.join();
}

RUN_DEMOS
