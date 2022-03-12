#include <boost/spirit/home/x3.hpp>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>

#include <demo-common.h>

using namespace boost::spirit;

DEMO(parser)
{
    const std::string str = "23.4, 14.2, 199.09";

    const auto print = [](auto& ctx) { std::cout << "double: " << _attr(ctx) << std::endl; };

    auto begin = str.begin();
    const auto end = str.end();
    const auto parser = x3::double_[print] % ','; // % means list
    const auto skipper = x3::ascii::space;
    std::vector<double> parsed;

    const bool match = phrase_parse(begin, end, parser, skipper, parsed);
    if (begin != end)
    {
        std::cout << "Not full match" << std::endl;
    }

    std::cout << "Match: " << std::boolalpha << match << std::endl;
    std::cout << "Parsed: ";
    std::copy(parsed.begin(), parsed.end(), std::ostream_iterator<double>(std::cout, ", "));
    std::cout << std::endl;
}

namespace models
{
struct named_value
{
    std::string name;
    double value;
};

} // namespace models

BOOST_FUSION_ADAPT_STRUCT(models::named_value, name, value)

namespace parsers
{
x3::rule<class named_value, models::named_value> const named_value = "named_value";

auto const quoted_string = x3::lexeme['"' >> +(x3::ascii::char_ - '"') >> '"'];

const auto named_value_def = quoted_string >> '=' >> x3::double_;

BOOST_SPIRIT_DEFINE(named_value);

} // namespace parsers

DEMO(rule)
{
    const std::string str = "\"mass of basement\" = 12.95";

    auto begin = str.begin();
    const auto end = str.end();
    const auto skipper = x3::ascii::space;
    models::named_value result;

    const bool match = phrase_parse(begin, end, parsers::named_value, skipper, result);

    if (match)
    {
        std::cout << "Parsed: " << result.name << "=" << result.value << std::endl;
    }
}

RUN_DEMOS
