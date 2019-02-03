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

    // Use count
    std::cout << "sp2.use_count() = " << sp2.use_count() << "\n";
}

DEMO(weak_ptr_demo)
{
    // std::weak_ptr models temporary ownership: when an object needs to be accessed
    // only if it exists, and it may be deleted at any time by someone else.

    std::shared_ptr<SomeClass> sp = std::make_shared<SomeClass>();

    // Creating
    std::weak_ptr<SomeClass> wp(sp); // link with shared_ptr sp

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

DEMO(enable_shared_from_this)
{
    // Derivating from std::enable_shared_from_this allow by a shared_ptr to an object
    // to safely generate additional shared_ptr's.

    struct Good : std::enable_shared_from_this<Good>
    {
        std::shared_ptr<Good> get_ptr()
        {
            return shared_from_this();
        }
    };

    struct Bad
    {
        std::shared_ptr<Bad> get_ptr()
        {
            return std::shared_ptr<Bad>(this);
        }

        ~Bad()
        {
            std::cout << "call ~Bad()\n";
        }
    };

    // Two shared_ptr's share the same object
    std::shared_ptr<Good> gp1 = std::make_shared<Good>();
    std::shared_ptr<Good> gp2 = gp1->get_ptr();
    std::cout << "gp2.use_count() = " << gp2.use_count() << "\n";

    // But shared_from_this() is only for creating new shared_ptr from existing one
    try
    {
        Good not_so_good;
        std::shared_ptr<Good> gp = not_so_good.get_ptr(); // call shared_from_this()
    }
    catch (std::bad_weak_ptr& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }

    // Each shared_ptr thinks that it is only one owner of the object
    std::shared_ptr<Bad> bp1 = std::make_shared<Bad>();
    std::shared_ptr<Bad> bp2 = bp1->get_ptr();
    std::cout << "bp2.use_count() = " << bp2.use_count() << "\n";
    // so we have double deletion of Bad.
}

RUN_DEMOS
