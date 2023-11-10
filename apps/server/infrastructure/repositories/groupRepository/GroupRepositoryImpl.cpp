#include "GroupRepositoryImpl.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <format>

#include "Group.odb.h"
#include "server/infrastructure/errors/GroupRepositoryError.h"

namespace server::infrastructure
{
GroupRepositoryImpl::GroupRepositoryImpl(std::shared_ptr<odb::pgsql::database> dbInit,
                                         std::shared_ptr<GroupMapper> groupMapperInit)
    : db{std::move(dbInit)}, groupMapper{std::move(groupMapperInit)}
{
}

std::shared_ptr<domain::Group> GroupRepositoryImpl::createGroup(const domain::CreateGroupPayload& payload) const
{
    try
    {
        {
            const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

            const auto group = std::make_shared<Group>(payload.id, currentDate);

            odb::transaction transaction(db->begin());

            db->persist(group);

            transaction.commit();

            return groupMapper->mapToDomainGroup(group);
        }
    }
    catch (const std::exception& error)
    {
        throw errors::GroupRepositoryError{error.what()};
    }
}

std::optional<std::shared_ptr<domain::Group>>
GroupRepositoryImpl::findGroupById(const domain::FindGroupByIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<Group> query;

        std::shared_ptr<Group> group(db->query_one<Group>(query::id == payload.id));

        transaction.commit();

        if (!group)
        {
            return std::nullopt;
        }

        return groupMapper->mapToDomainGroup(group);
    }
    catch (const std::exception& error)
    {
        throw errors::GroupRepositoryError{error.what()};
    }
}

void GroupRepositoryImpl::deleteGroup(const domain::DeleteGroupPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        db->erase<Group>(payload.group.getId());

        transaction.commit();
    }
    catch (const std::exception& error)
    {
        throw errors::GroupRepositoryError{error.what()};
    }
}

void GroupRepositoryImpl::deleteGroups(const domain::DeleteGroupsPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<Group> Query;

        db->erase_query<Group>(Query::id.in_range(payload.groupsIds.begin(), payload.groupsIds.end()));

        transaction.commit();
    }
    catch (const std::exception& error)
    {
        throw errors::GroupRepositoryError{error.what()};
    }
}
}
