#include "demo-common.h"

#include <string_view>
#include <string>

#include <boost/algorithm/searching/boyer_moore.hpp>
#include <boost/algorithm/searching/boyer_moore_horspool.hpp>
#include <boost/algorithm/searching/knuth_morris_pratt.hpp>
#include <boost/algorithm/clamp.hpp>
#include <boost/algorithm/gather.hpp>

DEMO(searching)
{
    // find string in text

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

DEMO(clamp)
{
    // clamp value between a pair of boundaries
    std::cout << "clamp(5.6, 10.0, 20.0) = " << boost::algorithm::clamp(5.6, 10.0, 20.0) << std::endl;
    std::cout << "clamp(11.2, 10.0, 20.0) = " << boost::algorithm::clamp(11.2, 10.0, 20.0) << std::endl;
    std::cout << "clamp(55.0, 10.0, 20.0) = " << boost::algorithm::clamp(55.0, 10.0, 20.0) << std::endl;

    std::vector<int> values = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::cout << std::endl << "clamped range: ";
    boost::algorithm::clamp_range(values, std::ostream_iterator<int>(std::cout, ", "), 3, 7);
    std::cout << std::endl;
}

DEMO(gather)
{
    // gathers elements that satisfy predicate around pivot
    std::vector<int> values = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    const auto is_odd = [](int v) { return v % 2 == 1; };
    auto pivot_iter = std::find(values.begin(), values.end(), 5);
    boost::algorithm::gather(values.begin(), values.end(), values.begin() + 5, is_odd);

    std::cout << "Gathered odds around " << *pivot_iter << ": ";
    std::copy(values.begin(), values.end(), std::ostream_iterator<int>(std::cout, ", "));
    std::cout << std::endl;
}



RUN_DEMOS
