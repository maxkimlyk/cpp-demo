#include <memory>
#include <iostream>

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

void unique_ptr_example()
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

    // custom deleter
    std::unique_ptr<SomeClass[]> up_to_array(new SomeClass[3]);
}

int main()
{
    unique_ptr_example();
    return 0;
}
