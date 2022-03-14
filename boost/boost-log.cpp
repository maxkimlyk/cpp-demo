#include <fstream>
#include <iomanip>

#include <boost/core/null_deleter.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

#include <demo-common.h>

DEMO(trivial)
{
    BOOST_LOG_TRIVIAL(trace) << "Trace";
    BOOST_LOG_TRIVIAL(debug) << "Debug";
    BOOST_LOG_TRIVIAL(info) << "Info";
    BOOST_LOG_TRIVIAL(warning) << "Warning";
    BOOST_LOG_TRIVIAL(error) << "Error";
    BOOST_LOG_TRIVIAL(fatal) << "Fatal";
}

BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(scope, "Scope", boost::log::attributes::named_scope::value_type)

DEMO(init_sink)
{
    const std::string_view kLogfile = "boost-log.log";

    boost::shared_ptr<boost::log::core> core = boost::log::core::get();
    core->add_global_attribute("LineID", boost::log::attributes::counter<unsigned int>(1));
    core->add_global_attribute("TimeStamp", boost::log::attributes::local_clock());

    boost::log::core::get()->add_global_attribute("Scope", boost::log::attributes::named_scope());

    const auto sink =
        boost::make_shared<boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend>>();
    boost::shared_ptr<std::ostream> console(&std::cout, boost::null_deleter());

    sink->locked_backend()->add_stream(boost::make_shared<std::ofstream>(kLogfile.data()));
    sink->locked_backend()->add_stream(console);

    // clang-format off
    sink->set_formatter(boost::log::expressions::stream
                        << boost::log::expressions::format_date_time<boost::posix_time::ptime>(
                               "TimeStamp", "%Y-%m-%d %H:%M:%S ")
                        << "[" << boost::log::trivial::severity << "] "
                        << boost::log::expressions::if_(
                               boost::log::expressions::has_attr(scope)) [
                             boost::log::expressions::stream << "(" << scope << ") "
                           ]
                        << boost::log::expressions::smessage
                        << boost::log::expressions::if_(
                               boost::log::expressions::has_attr(tag_attr)) [
                             boost::log::expressions::stream << " (" << tag_attr << ") "
                           ]
    );
    // clang-format on

    boost::log::core::get()->add_sink(sink);

    BOOST_LOG_TRIVIAL(info) << "This log is written both in console and in " << kLogfile;
}

DEMO(personal_logger)
{
    boost::log::sources::logger lg;
    BOOST_LOG(lg) << "personal logger message";
}

DEMO(tag)
{
    using namespace boost::log::trivial;

    boost::log::sources::severity_logger<severity_level> lg;
    lg.add_attribute("Tag", boost::log::attributes::constant<std::string>("Tag1"));

    BOOST_LOG_SEV(lg, severity_level::warning) << "Message with tag";
}

DEMO(scope)
{
    using namespace boost::log::trivial;

    boost::log::sources::severity_logger<severity_level> lg;

    BOOST_LOG_NAMED_SCOPE("named_scope");

    BOOST_LOG_SEV(lg, severity_level::warning) << "Message with scope";
}
