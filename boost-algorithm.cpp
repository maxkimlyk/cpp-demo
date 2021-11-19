#include "demo-common.h"

#include <string_view>
#include <string>

#include <boost/algorithm/algorithm.hpp>
#include <boost/algorithm/searching/boyer_moore.hpp>
#include <boost/algorithm/searching/boyer_moore_horspool.hpp>
#include <boost/algorithm/searching/knuth_morris_pratt.hpp>

DEMO(searching)
{
    // finds string in text

    const std::string_view text = "There are only two kinds of languages: "
                                  "the ones people complain about and the ones nobody uses";

    const std::string_view pattern = "complain about";

    const auto make_report = [&text](auto from_iter, auto to_iter) -> std::string {
        if (from_iter == text.end())
        {
            return "not found";
        }
        return "found: '" + std::string(from_iter, to_iter) + "'";
    };

    const auto [bm_from, bm_to] = boost::algorithm::boyer_moore_search(text.begin(), text.end(), pattern.begin(), pattern.end());
    std::cout << "boyer_moore_search: " << make_report(bm_from, bm_to) << std::endl;

    const auto [bmh_from, bmh_to] = boost::algorithm::boyer_moore_horspool_search(text.begin(), text.end(), pattern.begin(), pattern.end());
    std::cout << "boyer_moore_horspool_search: " << make_report(bmh_from, bmh_to) << std::endl;

    const auto [kmp_from, kmp_to] = boost::algorithm::knuth_morris_pratt_search(text.begin(), text.end(), pattern.begin(), pattern.end());
    std::cout << "knuth_morris_pratt_search: " << make_report(kmp_from, kmp_to) << std::endl;
}

RUN_DEMOS
