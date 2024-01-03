#include "CreateGroupCommandHandlerImpl.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "fmt/format.h"

#include "loguru.hpp"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
CreateGroupCommandHandlerImpl::CreateGroupCommandHandlerImpl(
    std::shared_ptr<domain::GroupRepository> groupRepositoryInit)
    : groupRepository{std::move(groupRepositoryInit)}
{
}

CreateGroupCommandHandlerResult CreateGroupCommandHandlerImpl::execute() const
{
    LOG_S(INFO) << "Creating group...";

    std::stringstream uuid;
    uuid << boost::uuids::random_generator()();

    const auto groupId = uuid.str();

    const auto group = groupRepository->createGroup({groupId});

    LOG_S(INFO) << fmt::format("Group with id \"{}\" created.", group->getId());

    return {*group};
}
}
