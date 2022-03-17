#include <typeinfo>
#include <typeindex>

#include <boost/type_index.hpp>

#include <demo-common.h>

namespace my
{
class SuperClass
{
};

} // namespace my

DEMO(type_index)
{
  my::SuperClass obj;

  std::cout << "typeid(obj).name: " << typeid(obj).name() << std::endl;
  std::cout << "boost typeindex pretty_name (runtime): " << boost::typeindex::type_id_runtime(obj).pretty_name() << std::endl;
  std::cout << "boost typeindex pretty_name (static): " << boost::typeindex::type_id<my::SuperClass>().pretty_name() << std::endl;

  std::cout << "boost typeindex with cvr: " << boost::typeindex::type_id_with_cvr<const my::SuperClass&>() << std::endl;

}
