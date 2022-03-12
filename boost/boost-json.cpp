#include <boost/json.hpp>

#include <boost/json/src.hpp>

#include <demo-common.h>

DEMO(value)
{
    boost::json::object obj;
    obj["text"] = "some_text";
    obj["nothing"] = nullptr;
    obj["flag"] = true;
    auto& subobj = obj["subobj"].emplace_object();
    subobj["field1"] = false;
    subobj["field2"] = "ok";
    obj["list"] = {1, 2, 3};
    obj["subobj2"] = {{"one", 1}, {"three", 3}};

    std::cout << obj << std::endl;
    std::string as_str = serialize(obj);

    boost::json::value parsed = boost::json::parse(as_str);
    bool flag = boost::json::value_to<bool>(parsed.at("flag"));
}

struct my_struct
{
    int count;
    std::string name;
    bool used;

    bool operator==(const my_struct& other) const
    {
        return count == other.count && name == other.name && used == other.used;
    }
};

void tag_invoke(boost::json::value_from_tag, boost::json::value& json, const my_struct& value)
{
    json = {
        {"count", value.count},
        {"name", value.name},
        {"used", value.used},
    };
}

my_struct tag_invoke(boost::json::value_to_tag<my_struct>, const boost::json::value& json)
{
    const boost::json::object& obj = json.as_object();
    my_struct result;
    result.count = boost::json::value_to<int>(obj.at("count"));
    result.name = boost::json::value_to<std::string>(obj.at("name"));
    result.used = boost::json::value_to<bool>(obj.at("used"));
    return result;
}

DEMO(value_conversion)
{
    my_struct obj = {1, "obj", true};

    auto json = boost::json::value_from(obj);
    std::cout << json << std::endl;

    my_struct parsed = boost::json::value_to<my_struct>(json);

    if (obj == parsed)
    {
        std::cout << "obj == parsed" << std::endl;
    }
}


