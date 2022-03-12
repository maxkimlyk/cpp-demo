#include "demo-common.h"

#include <boost/contract.hpp>

int get_next_odd(const int x)
{
    int result;

    const boost::contract::old_ptr<int> old_x = BOOST_CONTRACT_OLDOF(x);
    const boost::contract::check c = boost::contract::function()
                                         // checked before function body
                                         .precondition([&]() { BOOST_CONTRACT_ASSERT(x >= 0); })
                                         // checked after function body
                                         .postcondition([&]() { BOOST_CONTRACT_ASSERT(result > x); })
                                         // checked after throwing an exception
                                         .except([&]() { BOOST_CONTRACT_ASSERT(x == *old_x); });

    if (x % 2 == 0)
    {
        return result = x + 1;
    }
    return result = x + 2;
}

DEMO(non_member_function_contract)
{
    get_next_odd(1);
}

int singleton = 0;

class Class : private boost::contract::constructor_precondition<Class>
{
public:
    // class invariant checked after constructor, before destructor, before and after each method that checks contracts
    void invariant() const
    {
        BOOST_CONTRACT_ASSERT(x >= 0);
    }

    // static class invariant
    static void static_invariant()
    {
        BOOST_CONTRACT_ASSERT(singleton >= 0);
    }

    Class(int ini)
        : boost::contract::constructor_precondition<Class>(
              [&]() { // *this* cannot be used there, object isn't yet created!
                  BOOST_CONTRACT_ASSERT(ini == 0);
              })
    {
        // constructors can have only postcondition and except inside body
        const boost::contract::check c =
            boost::contract::constructor(this).postcondition([&] { BOOST_CONTRACT_ASSERT(x == 0); });
    }

    ~Class()
    {
        const boost::contract::check c = boost::contract::destructor(this);
    }

    void set(int v)
    {
        const boost::contract::check c = boost::contract::public_function(this);
        x = v;
        singleton = v;
    }

    virtual int virtual_function(int param, boost::contract::virtual_* v = 0)
    {
        int result;

        // When v is present, always pass it as the first argument to boost::contract::public_function and BOOST_CONTRACT_OLDOF.
        // Always pass result to boost::contract::public_function right after v for non-void functions.
        boost::contract::check c =
            boost::contract::public_function(v, result, this)
                .precondition([&]() { BOOST_CONTRACT_ASSERT(param >= 0); })
                .postcondition([&](int const& result) { BOOST_CONTRACT_ASSERT(result >= param + 1); });

        return result = param + 1;
    }

    static void static_public_function()
    {
        boost::contract::check c = boost::contract::public_function<Class>();
    }

protected:
    int x = 0;
    static int static_x;
};

DEMO(class_contract)
{
    Class cls(0);
    cls.set(1);
    cls.virtual_function(1);
}

class Derived : public Class
{
public:
    typedef BOOST_CONTRACT_BASE_TYPES(public Class) base_types;

    void invariant() const
    {
        BOOST_CONTRACT_ASSERT(x < 10);
    }

    Derived() : Class(0)
    {
    }

    int virtual_function(int param, boost::contract::virtual_* v = 0) override
    {
        // When override_... is present, always pass it as template parameter to boost::contract::public_function.
        // When v is present, always pass it as the first argument to boost::contract::public_function and BOOST_CONTRACT_OLDOF.
        // Always pass result to boost::contract::public_function right after v for non-void functions.

        int result;
        boost::contract::check c =
            boost::contract::public_function<override_virtual_function> // pass override tag
            (v, result, &Derived::virtual_function, this, param);

        return result = param + 2;
    }

    BOOST_CONTRACT_OVERRIDE(virtual_function) // defines override_virtual_function
};

DEMO(derived_class_contract)
{
    Derived derived;
    derived.virtual_function(1);
    derived.set(11);
}

DEMO(throw_on_failures)
{
    boost::contract::set_precondition_failure(
        boost::contract::set_postcondition_failure(boost::contract::set_invariant_failure(
            boost::contract::set_old_failure([](boost::contract::from where) { throw; }))));

    try {
        get_next_odd(-1);
    } catch (const boost::contract::assertion_failure&) {
        std::cout << "Caught assertion failure";
    }
}


