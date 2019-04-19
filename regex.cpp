#include "common.h"

#include <string>
#include <regex>

DEMO(regex)
{
    std::string str1 = "hello";
    std::string str2 = "world";
    std::string str3 = "iostream";

    std::regex re(".*l.");

    for (auto& str : {str1, str2})
        if (std::regex_match(str, re))
            std::cout << str << "\n";
}

RUN_DEMOS
