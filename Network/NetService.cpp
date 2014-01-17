#include "NetService.hpp"

namespace net
{
    using namespace boost::asio;

    NetService::NetService():
        io_service_(new io_service)
    {
    }

    NetService::~NetService()
    {
    }

    std::size_t NetService::poll()
    {
        return io_service_->poll();
    }
}
