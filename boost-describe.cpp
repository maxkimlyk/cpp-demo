#include <sstream>

#include <boost/describe.hpp>
#include <boost/mp11.hpp>

#include "demo-common.h"

BOOST_DEFINE_ENUM(Language, GREEK, ENGLISH);

enum GreekLetter
{
    ALPHA = 1,
    BETA = 2,
    GAMMA = 3,
    DELTA = 4,
};
BOOST_DESCRIBE_ENUM(GreekLetter, ALPHA, BETA, GAMMA, DELTA);

struct Point
{
    int x;
    int y;
};
BOOST_DESCRIBE_STRUCT(Point,
                      (),    // list of base classes
                      (x, y) // list of public members
);

class Cls : private Point
{
public:
    int pub;

protected:
    int prot;

private:
    int priv;

public:
    int f() const;

private:
    BOOST_DESCRIBE_CLASS(Cls,
                         (Point),  // list of base classes
                         (pub, f), // public members
                         (prot),   // protected members
                         (priv)    // private members
    );
};

struct X
{
    int f();
    int f() const;
    void f(int x);
};
BOOST_DESCRIBE_STRUCT(X, (),
                      (
                          // overloaded members must be described with their signature
                          (int())f, (int() const)f, (void(int))f));

DEMO(describe_enum)
{
    const auto to_string = [](auto e) {
        using EnumType = std::decay_t<decltype(e)>;
        char const* r = nullptr;

        boost::mp11::mp_for_each<boost::describe::describe_enumerators<EnumType>>(
            [&r, &e](auto descriptor) {
                if (e == descriptor.value)
                    r = descriptor.name;
            });

        if (r == nullptr)
        {
            throw std::runtime_error("unexpected value");
        }

        return r;
    };

    GreekLetter l = GAMMA;
    std::cout << l << ": " << to_string(l) << std::endl;
}

template <class ObjT>
std::string to_string(const ObjT& obj)
{
    using Bases = boost::describe::describe_bases<ObjT, boost::describe::mod_any_access>;
    using Members = boost::describe::describe_members<ObjT, boost::describe::mod_any_access>;

    std::ostringstream os;
    os << "{";

    bool first = true;
    const auto print_comma = [&]() {
        if (!first)
            os << ", ";
        else
            first = false;
    };

    boost::mp11::mp_for_each<Bases>([&](auto descr) {
        using BaseT = typename decltype(descr)::type;
        print_comma();
        os << to_string((BaseT const&)obj);
    });

    boost::mp11::mp_for_each<Members>([&](auto descr) {
        print_comma();
        os << descr.name << " = " << obj.*descr.pointer;
    });

    os << "}";

    return os.str();
};

DEMO(describe_struct)
{
    Cls obj;
    std::cout << to_string(obj) << std::endl;
}

RUN_DEMOS
