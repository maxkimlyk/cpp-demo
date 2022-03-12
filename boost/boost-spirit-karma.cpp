#include <complex>

#include <boost/spirit/include/karma.hpp>

#include <demo-common.h>

using namespace boost::spirit;

DEMO(generator)
{
    {
        std::string str;
        karma::generate(std::back_inserter(str), karma::int_ % ", ", std::vector<int>{1, 2, 3, 4, 5});
        std::cout << str << std::endl;
    }

    const auto print_complex = [](auto complex) {
        std::string str;
        karma::generate(std::back_inserter(str),
                        // clang-format off
                        !karma::double_(0.0) << '(' << karma::double_ << ", " << karma::double_ << ')'
                        |
                        karma::omit[double_] << double_
                        // clang-format on
                        ,
                        complex.imag(), complex.real(), complex.imag());
        std::cout << str << std::endl;
    };

    print_complex(std::complex<double>(1, 0));
    print_complex(std::complex<double>(1, 0.5));

}

RUN_DEMOS
