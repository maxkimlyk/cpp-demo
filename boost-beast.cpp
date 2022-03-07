#include <string>

#include <boost/beast.hpp>

#include "demo-common.h"

DEMO(http_client_sync)
{
    const std::string host = "info.cern.ch";
    const std::string port = "80";
    const std::string target = "/";
    const int version = 10;

    boost::asio::io_context ioc;

    boost::asio::ip::tcp::resolver resolver(ioc);
    boost::beast::tcp_stream stream(ioc);

    std::cout << "Resolving address" << std::endl;
    const auto results = resolver.resolve(host, port);

    std::cout << "Connecting" << std::endl;
    stream.connect(results);

    std::cout << "Making request" << std::endl;
    boost::beast::http::request<boost::beast::http::string_body> request(
        boost::beast::http::verb::get, target, version);
    request.set(boost::beast::http::field::host, host);
    request.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

    boost::beast::http::write(stream, request);

    boost::beast::flat_buffer buffer;
    boost::beast::http::response<boost::beast::http::dynamic_body> res;

    std::cout << "Getting response" << std::endl;
    boost::beast::http::read(stream, buffer, res);

    std::cout << res << std::endl;

    boost::beast::error_code ec;
    stream.socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

    if (ec && ec != boost::beast::errc::not_connected)
        throw boost::beast::system_error{ec};
}

RUN_DEMOS
