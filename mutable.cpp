#include "common.h"

DEMO(mutable_lambda)
{
    int i = 0;

    /*
    auto func1 = [i]()  {
        std::cout << i++ << " "; // error: atempt to change field of const object!
        std::cout << i << "\n";
    };
    */

   auto func2 = [i]() mutable {
       std::cout << i++ << " "; // OK
       std::cout << i << "\n";
   };

   func2();
   func2();
}

RUN_DEMOS
