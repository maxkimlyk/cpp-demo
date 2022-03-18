#include <vector>

#define BOOST_TEST_MODULE Test Demo
#include <boost/test/included/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/data/monomorphic/generators/xrange.hpp>

// Simple test case

BOOST_AUTO_TEST_CASE(case1)
{
    int i = 0;
    BOOST_TEST(i == 1);
    BOOST_TEST(i == 2);

    std::vector<int> v = {1, 2, 3};
    BOOST_TEST(v == (std::vector<int>{1, 2, 4}));
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
                         boost::unit_test::data::xrange(3), // using cross-product to specify the number of iterations
                     value, iter)
{
    BOOST_TEST(value == 1);
}

const std::vector<int> kVector = {2, 3, 4};

BOOST_DATA_TEST_CASE(dataset_from_vector, boost::unit_test::data::make(kVector), value) {
  BOOST_TEST(value == 1);
}
