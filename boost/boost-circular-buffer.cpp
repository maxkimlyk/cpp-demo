#include "demo-common.h"

#include <iterator>

#include <boost/circular_buffer.hpp>

DEMO(circular_buffer)
{
    constexpr size_t capacity = 4;
    boost::circular_buffer<int> buffer(capacity);

    const auto print_buffer = [&buffer]() {
        std::cout << "buffer: ";
        std::copy(buffer.begin(), buffer.end(), std::ostream_iterator<int>(std::cout, ", "));
        std::cout << std::endl;
    };

    std::cout << "pushing 5 times" << std::endl;
    for (int i = 1; i <= 5; ++i)
    {
        buffer.push_back(i);
    }

    print_buffer();

    std::cout << "pop front and back" << std::endl;
    buffer.pop_front();
    buffer.pop_back();

    print_buffer();
}


