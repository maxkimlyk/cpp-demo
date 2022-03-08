#include <algorithm>
#include <iostream>
#include <string>

#include <boost/program_options.hpp>

void config_example();
void envvar_example();

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
        ("config_example", boost::program_options::bool_switch(), "run config example")
        ("envvar_example", boost::program_options::bool_switch(), "run envvar example")
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

    if (params["config_example"].as<bool>())
    {
        config_example();
        return 0;
    }

    if (params["envvar_example"].as<bool>())
    {
        envvar_example();
        return 0;
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

void config_example()
{
    const std::string config_content = "param=1\n"
                                       "[section1]\n"
                                       "param2=value2\n";

    std::istringstream ss(config_content);

    boost::program_options::options_description options;
    options.add_options()("param", boost::program_options::value<std::string>());
    options.add_options()("section1.param2", boost::program_options::value<std::string>());

    boost::program_options::variables_map params;

    const bool allow_unregistered = true;

    boost::program_options::store(
        boost::program_options::parse_config_file(ss, options, allow_unregistered), params);
    boost::program_options::notify(params);

    for (const auto& keyvalue : params)
    {
        const auto& [key, value] = keyvalue;
        std::cout << key << "=" << value.as<std::string>() << std::endl;
    }
}

void envvar_example()
{
    boost::program_options::options_description options;
    options.add_options()("editor", boost::program_options::value<std::string>());
    options.add_options()("pwd", boost::program_options::value<std::string>());

    const auto name_mapper = [&options](std::string str) -> std::string {
        std::transform(str.begin(), str.end(), str.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        const bool approx = false;
        if (options.find_nothrow(str, approx) == nullptr) {
            return "";
        }
        return str;
    };

    boost::program_options::variables_map params;
    boost::program_options::store(boost::program_options::parse_environment(options, name_mapper),
                                  params);
    boost::program_options::notify(params);

    for (const auto& keyvalue : params)
    {
        const auto& [key, value] = keyvalue;
        std::cout << key << "=" << value.as<std::string>() << std::endl;
    }
}
