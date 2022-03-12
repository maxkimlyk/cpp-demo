#include <boost/parameter.hpp>

#include "demo-common.h"

struct Config {
    bool saturate = false;
};

std::ostream& operator<<(std::ostream& os, const Config& cfg) {
    return os << "{saturate=" << cfg.saturate << "}";
}

BOOST_PARAMETER_NAME(lhs);
BOOST_PARAMETER_NAME(rhs);
BOOST_PARAMETER_NAME(config);

// clang-format off
BOOST_PARAMETER_FUNCTION(
        (int),
        sum,
        tag,
        (required
          (lhs, *)
          (rhs, *)
         )
        (optional
          (config, *, Config{})
        )
    )
    {
      std::cout << "lhs=" << lhs << ", rhs=" << rhs << ", config=" << config << std::endl;
      return 0;
    }
// clang-format on

DEMO(call)
{
    sum(1, 2);
    sum(_rhs=1, _lhs=2);
    sum(1, 2, _config = Config{true});
}


