#include <iostream>
#include <string>

#include <boost/program_options.hpp>

int main(int argc, char** argv)
{
    int level = 0;

    boost::program_options::positional_options_description pp;
    pp.add("input", 1);
    pp.add("output", -1);

    boost::program_options::options_description desc("Params");

    // clang-format off
    desc.add_options()
        ("help", "print help message")
        ("level,l", boost::program_options::value<int>(&level)->default_value(100), "level")
        ("count,c", boost::program_options::value<int>(), "set the count")
        ("string,S", boost::program_options::value<std::vector<std::string>>(), "set of strings")
        ("multi-paragrath-option",
            boost::program_options::value<int>(),
            "    Here we have long description for this option.\n\nIt's possible to make many paragraphs.")
        ("intented-option",
            boost::program_options::value<int>(),
            "Here we have intent before each value.\n    Value1: \tLong long long long long long long description for value 1.")
        ;
    // clang-format on

    boost::program_options::variables_map params;
    boost::program_options::store(
        boost::program_options::command_line_parser(argc, argv).options(desc).positional(pp).run(),
        params);
    boost::program_options::notify(params);

    if (params.count("help"))
    {
        std::cout << "??? input output* [PARAMS]" << std::endl;
        std::cout << desc << std::endl;
        return 1;
    }

    std::cout << "level: " << level << std::endl;

    if (params.count("count"))
    {
        std::cout << "count=" << params["count"].as<int>() << std::endl;
    }

    if (params.count("string"))
    {
        for (const auto& str : params["string"].as<std::vector<std::string>>())
        {
            std::cout << "S: " << str << std::endl;
        }
    }

    return 0;
}
