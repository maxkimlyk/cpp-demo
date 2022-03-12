#include <boost/geometry.hpp>

#include "demo-common.h"

#include <boost/geometry/geometries/adapted/boost_tuple.hpp>
BOOST_GEOMETRY_REGISTER_BOOST_TUPLE_CS(cs::cartesian)

using namespace boost::geometry;

DEMO(point)
{
    model::d2::point_xy<double> p1(1, 1), p2(0.5, 0.5);
    std::cout << "Distance p1-p2: " << boost::geometry::distance(p1, p2) << std::endl;

    model::polygon<model::d2::point_xy<double>> poly;
    model::d2::point_xy<double> poly_points[] = {
        {0.0, 0.0}, {0.0, 1.0}, {1.0, 1.0}, {1.0, 0.0}, {0.0, 0.0}};
    append(poly, poly_points);
    std::cout << "P2 in poly: " << std::boolalpha << within(p2, poly) << std::endl;
    std::cout << "Poly area: " << area(poly) << std::endl;
}

DEMO(spherical)
{
    using spherical_point = model::point<double, 2, cs::spherical_equatorial<degree>>;

    spherical_point moscow = {37.618423, 55.751244};
    spherical_point st_petersburg = {30.308611, 59.937500};

    const double earth_radius_km = 6371;

    const double distance_km = distance(moscow, st_petersburg) * earth_radius_km;
    std::cout << "Distance: " << distance_km << " km" << std::endl;
}

DEMO(rtree)
{
    using Index = size_t;
    using Point = model::point<double, 2, cs::cartesian>;
    using Box = model::box<Point>;
    using Value = std::pair<Box, Index>;

    const size_t kMaxElementsInNode = 16;

    index::rtree<Value, index::quadratic<kMaxElementsInNode>> rtree;
    for (unsigned i = 0; i < 10; ++i)
    {
        Box box(Point(i, i), Point(i + 0.5f, i + 0.5f));
        rtree.insert(std::make_pair(box, i));
    }

    Box query_box(Point(2, 2), Point(4, 5));

    std::vector<Value> result;
    rtree.query(index::intersects(query_box), std::back_inserter(result));

    std::cout << wkt(query_box) << " intersects elements with indices: ";
    for (const auto& r : result)
    {
        std::cout << r.second << ", ";
    }
    std::cout << std::endl;

    result.clear();

    const size_t kN = 5;
    const Point query_point(2, 2);

    rtree.query(index::nearest(query_point, kN), std::back_inserter(result));

    std::cout << kN << " nearest for " << wkt(Point(2, 2)) << ": ";
    for (const auto& r : result)
    {
        std::cout << r.second << ", ";
    }
    std::cout << std::endl;
}

DEMO(convex_hull)
{
    using Point = model::d2::point_xy<double>;

    model::polygon<Point> poly{{{0.0, 0.0}, {1.0, 1.0}, {2.0, 0.0}, {2.0, 2.0}, {0.0, 2.0}, {0.0, 0.0}}};
    model::polygon<Point> hull;

    convex_hull(poly, hull);

    std::cout << "For polygon " << dsv(poly) << std::endl;
    std::cout << "convex hull would be " << dsv(hull) << std::endl;
}


