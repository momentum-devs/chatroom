#include "HashServiceImpl.h"

#include <boost/compute/detail/sha1.hpp>

namespace server::application
{
std::string HashServiceImpl::hash(const std::string& data) const
{
    boost::compute::detail::sha1 dataHash;

    dataHash.process(data);

    return static_cast<std::string>(dataHash);
}
}
