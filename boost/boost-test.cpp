#include <vector>

#define BOOST_TEST_MODULE Test Demo
#include <boost/test/included/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/utility/identity_type.hpp>

// Simple test case

BOOST_AUTO_TEST_CASE(auto_case)
{
    int i = 0;
    BOOST_TEST(i == 1);
    BOOST_TEST(i == 2);
    BOOST_TEST_REQUIRE(i == 0);
    BOOST_TEST_WARN(i != 0, "custom failure message");

    BOOST_TEST(1.0 == 1.000001, boost::test_tools::tolerance(1e-4));
    BOOST_CHECK_CLOSE(1.0, 1.000001, 1e-4);

    std::vector<int> a = {1, 2, 3};
    std::set<long> b = {1, 2, 3};

    BOOST_TEST(a == b, boost::test_tools::per_element());

    BOOST_CHECK_NO_THROW(a.size());
    BOOST_CHECK_THROW(a.at(10), std::out_of_range);
    BOOST_CHECK_EXCEPTION(a.at(10), std::out_of_range, [](const auto& ex) { return ex.what() != ""; });
}

// Timed test case
BOOST_AUTO_TEST_CASE(timed_case, *boost::unit_test::timeout(1)) // 1 second
{
    for (;;)
    {
    }
}

// Generated dataset

class random_numbers
{
public:
    enum
    {
        arity = 1
    };

    struct iterator
    {
        iterator() : value(rand() % 10)
        {
        }

        int operator*() const
        {
            return value;
        }

        void operator++()
        {
            value = rand() % 10;
        }

    private:
        int value;
    };

    boost::unit_test::data::size_t size() const
    {
        // size is infinite
        return boost::unit_test::data::BOOST_TEST_DS_INFINITE_SIZE;
    }

    iterator begin() const
    {
        return iterator();
    }
};

namespace boost::unit_test::data::monomorphic
{
template <>
struct is_dataset<random_numbers> : boost::mpl::true_
{
};
} // namespace boost::unit_test::data::monomorphic

BOOST_DATA_TEST_CASE(random_numbers_dataset,
                     random_numbers() ^
                         boost::unit_test::data::xrange(3), // using zip to specify the number of iterations
                     value, iter)
{
    BOOST_TEST(value == 1);
}

// Given dataset

const std::vector<int> kVector = {2, 3, 4};

BOOST_DATA_TEST_CASE(dataset_from_vector, boost::unit_test::data::make(kVector), value)
{
    BOOST_TEST(value == 1);
}

// Type based test case

BOOST_AUTO_TEST_CASE_TEMPLATE(type_based, T,
                              BOOST_IDENTITY_TYPE((std::tuple<int, long, unsigned char>)))
{
    BOOST_TEST((sizeof(T) == 4u || sizeof(T) == 8u || sizeof(T) == 1u));
}

// Decorators

// in order to run test examples invoke boost-test --run_test=@decorator_example
BOOST_AUTO_TEST_CASE(decorated_test_case,
                     *boost::unit_test::label("decorator_example") *
                         boost::unit_test::description("shows ussage of decorators"))
{
    BOOST_TEST(1 == 1);
}

// Fixtures

struct Fixture
{
    Fixture() : data(1)
    {
        // bosot-test -l message  to see this messages
        BOOST_TEST_MESSAGE("Fixture constructor");
    }

    void setup()
    {
        BOOST_TEST_MESSAGE("Fixture setup");
    }

    void teardown()
    {
        BOOST_TEST_MESSAGE("Fixture teardown");
    }

    ~Fixture()
    {
        BOOST_TEST_MESSAGE("Fixture destructor");
    }

    int data;
};

BOOST_FIXTURE_TEST_CASE(fixture_test_case, Fixture)
{
    BOOST_TEST(data == 1);
    ++data;
}

BOOST_AUTO_TEST_CASE(decorated_fixture_test_case, *boost::unit_test::fixture<Fixture>())
{
    // no way to access Fixture::data
}
