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
    // Creating
    std::unique_ptr<SomeClass> up1(new SomeClass(/* constr args */));
    auto up2 = std::make_unique<SomeClass>(/* constr args */);

    // Access
    up1->method();
    (*up1).method();

    //auto copy = up1; // error: unable to copy unique_ptr
    auto moved = std::move(up1); // OK
    moved->method();

    // Get usual pointer
    SomeClass* ptr = up2.get();

    // Array
    std::unique_ptr<SomeClass[]> up_to_array(new SomeClass[2]);

    // Custom deleter
    auto custom_delete_func = [](int*) { std::cout << "custom_delete_func\n"; };
    std::unique_ptr<int, std::function<void(int*)>> up3(new int(1), custom_delete_func);

    // Function return unique_ptr
    auto func = []() { return std::make_unique<int>(10); };
    auto up4 = func(); // moving here
}

DEMO(shared_ptr_demo)
{
    // Creating
    std::shared_ptr<SomeClass> sp1(new SomeClass(/* constr args */));
    auto sp2 = std::make_shared<SomeClass>(/* constr args */);

    auto copy = sp2; // copying is allowed
}

DEMO(weak_ptr_demo)
{
    // std::weak_ptr models temporary ownership: when an object needs to be accessed
    // only if it exists, and it may be deleted at any time by someone else.

    std::shared_ptr<SomeClass> sp = std::make_shared<SomeClass>();

    // Creating
    std::weak_ptr<SomeClass> wp(sp);// link with shared_ptr sp

    // Access
    if (auto spt = wp.lock()) // lock() creates shared_ptr for access
        spt->method();
    else
        std::cout << "wp is expired\n";

    sp.reset(); // delete shared_ptr

    // Access throwing exception
    try
    {
        std::shared_ptr<SomeClass> spt(wp); // get shared_ptr from weak_ptr
    }
    catch (const std::bad_weak_ptr& e)
    {
        std::cout << "Catched " << e.what() << "\n";
    }
}

RUN_DEMOS
