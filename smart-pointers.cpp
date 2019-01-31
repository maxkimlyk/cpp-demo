#include <memory>
#include "common.h"

struct SomeClass
{
    void method()
    {
        std::cout << "SomeClass::method\n";
    }

    ~SomeClass()
    {
        std::cout << "~SomeClass\n";
    }
};

DEMO(unique_ptr_demo)
{
    // creating
    std::unique_ptr<SomeClass> up1(new SomeClass(/* constr args */));
    auto up2 = std::make_unique<SomeClass>(/* constr args */);

    // access
    up1->method();
    (*up1).method();

    //auto copy = up1; // error: unable to copy unique_ptr
    auto moved = std::move(up1); // OK
    moved->method();

    // get usual pointer
    SomeClass* ptr = up2.get();

    // array
    std::unique_ptr<SomeClass[]> up_to_array(new SomeClass[2]);

    // custom deleter
    auto custom_delete_func = [](int*) { std::cout << "custom_delete_func\n"; };
    std::unique_ptr<int, std::function<void(int*)>> up3(new int(1), custom_delete_func);

    // function return unique_ptr
    auto func = []() { return std::make_unique<int>(10); };
    auto up4 = func(); // moving here
}

DEMO(shared_ptr_demo)
{
    // creating
    std::shared_ptr<SomeClass> sp1(new SomeClass(/* constr args */));
    auto sp2 = std::make_shared<SomeClass>(/* constr args */);

    auto copy = sp2; // copying is allowed
}

RUN_DEMOS
