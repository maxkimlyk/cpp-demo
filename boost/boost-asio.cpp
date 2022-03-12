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

DEMO(request)
{
    const std::string_view address = "178.62.9.171";

    boost::system::error_code ec;
    boost::asio::io_context context;
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(address, ec), 80);

    boost::asio::ip::tcp::socket socket(context);
    socket.connect(endpoint, ec);

    if (ec)
    {
        std::cout << "Failed to connect to address " << address << std::endl;
        return;
    }

    const std::string_view request = "GET / HTTP/1.1\r\n"
                                     "Host: myip.ru\r\n"
                                     "Connection: close\r\n\r\n";

    socket.write_some(boost::asio::buffer(request.data(), request.size()), ec);
    socket.wait(socket.wait_read);

    size_t bytes = socket.available();
    std::cout << "Bytes available: " << bytes << std::endl;

    if (bytes > 0)
    {
        std::vector<char> buffer(bytes);
        socket.read_some(boost::asio::buffer(buffer.data(), buffer.size()), ec);

        for (size_t i = 0; i < bytes; ++i)
        {
            std::cout << buffer[i];
        }
    }
}

void ListenToData(boost::asio::ip::tcp::socket& socket)
{
    static std::vector<char> buffer(1024);
    socket.async_read_some(boost::asio::buffer(buffer.data(), buffer.size()),
                           [&](boost::system::error_code ec, size_t length) {
                               if (!ec)
                               {
                                   std::cout << "\n\nRead " << length << " bytes\n\n";

                                   for (size_t i = 0; i < length; ++i)
                                   {
                                       std::cout << buffer[i];
                                   }

                                   ListenToData(socket);
                               }
                           });
}

DEMO(request_async)
{
    const std::string_view address = "178.62.9.171";

    boost::system::error_code ec;
    boost::asio::io_context context;
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::make_address(address, ec), 80);

    boost::asio::ip::tcp::socket socket(context);
    socket.connect(endpoint, ec);

    if (ec)
    {
        std::cout << "Failed to connect to address " << address << std::endl;
        return;
    }

    const std::string_view request = "GET / HTTP/1.1\r\n"
                                     "Host: myip.ru\r\n"
                                     "Connection: close\r\n\r\n";

    socket.write_some(boost::asio::buffer(request.data(), request.size()), ec);

    ListenToData(socket);

    context.run();
}


