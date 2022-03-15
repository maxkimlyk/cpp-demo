// requires -libdl in POSIX

#include <algorithm>

#define BOOST_STACKTRACE_USE_ADDR2LINE
#include <boost/stacktrace.hpp>

#include <demo-common.h>

void f(int value)
{
    std::cout << boost::stacktrace::stacktrace();
}

DEMO(stacktrace)
{
    std::vector<int> v = {1};
    std::for_each(v.begin(), v.end(), [](int value) { f(value); });
}
