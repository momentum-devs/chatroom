#include "UserGroupRepositoryImpl.h"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <format>

#include "server/infrastructure/errors/ResourceNotFoundError.h"
#include "server/infrastructure/errors/UserGroupRepositoryError.h"
#include "UserGroup.odb.h"

namespace server::infrastructure
{
UserGroupRepositoryImpl::UserGroupRepositoryImpl(std::shared_ptr<odb::sqlite::database> dbInit,
                                                 std::shared_ptr<UserGroupMapper> userGroupMapperInit,
                                                 std::shared_ptr<UserMapper> userMapperInit,
                                                 std::shared_ptr<GroupMapper> groupMapperInit)
    : db{std::move(dbInit)},
      userGroupMapper{std::move(userGroupMapperInit)},
      userMapper{std::move(userMapperInit)},
      groupMapper{std::move(groupMapperInit)}
{
}

domain::UserGroup UserGroupRepositoryImpl::createUserGroup(const domain::CreateUserGroupPayload& payload) const
{
    try
    {
        {
            const auto currentDate = to_iso_string(boost::posix_time::second_clock::universal_time());

            const auto user = userMapper->mapToPersistenceUser(payload.user);

            const auto group = groupMapper->mapToPersistenceGroup(payload.group);

            UserGroup userGroup{payload.id, odb::nullable<std::string>(), user, group, currentDate};

            odb::transaction transaction(db->begin());

            db->persist(userGroup);

            transaction.commit();

            return userGroupMapper->mapToDomainUserGroup(userGroup);
        }
    }
    catch (const std::exception& error)
    {
        throw errors::UserGroupRepositoryError{error.what()};
    }
}

std::optional<domain::UserGroup>
UserGroupRepositoryImpl::findUserGroup(const domain::FindUserGroupPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<UserGroup> Query;

        std::shared_ptr<UserGroup> userGroup(
            db->query_one<UserGroup>(Query::user->id == payload.userId && Query::group->id == payload.groupId));

        transaction.commit();

        if (!userGroup)
        {
            return std::nullopt;
        }

        return userGroupMapper->mapToDomainUserGroup(*userGroup);
    }
    catch (const std::exception& error)
    {
        throw errors::UserGroupRepositoryError{error.what()};
    }
}

std::optional<domain::UserGroup>
UserGroupRepositoryImpl::findUserGroupById(const domain::FindUserGroupByIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<UserGroup> Query;

        std::shared_ptr<UserGroup> userGroup(db->query_one<UserGroup>(Query::id == payload.id));

        transaction.commit();

        if (!userGroup)
        {
            return std::nullopt;
        }

        return userGroupMapper->mapToDomainUserGroup(*userGroup);
    }
    catch (const std::exception& error)
    {
        throw errors::UserGroupRepositoryError{error.what()};
    }
}

std::vector<domain::UserGroup>
UserGroupRepositoryImpl::findUsersGroupsByUserId(const domain::FindUsersGroupsByUserIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<UserGroup> Query;

        auto result = db->query<UserGroup>(Query::user->id == payload.userId);

        std::vector<domain::UserGroup> domainUserGroups;

        for (auto& userGroup : result)
        {
            domainUserGroups.push_back(userGroupMapper->mapToDomainUserGroup(userGroup));
        }

        transaction.commit();

        return domainUserGroups;
    }
    catch (const std::exception& error)
    {
        throw errors::UserGroupRepositoryError{error.what()};
    }
}

std::vector<domain::UserGroup>
UserGroupRepositoryImpl::findUsersGroupsByGroupId(const domain::FindUsersGroupsByGroupIdPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        typedef odb::query<UserGroup> Query;

        typedef odb::result<UserGroup> Result;

        Result result = db->query<UserGroup>(Query::group->id == payload.groupId);

        std::vector<domain::UserGroup> domainUserGroups;

        for (const auto& userGroup : result)
        {
            domainUserGroups.push_back(userGroupMapper->mapToDomainUserGroup(userGroup));
        }

        transaction.commit();

        return domainUserGroups;
    }
    catch (const std::exception& error)
    {
        throw errors::UserGroupRepositoryError{error.what()};
    }
}

domain::UserGroup UserGroupRepositoryImpl::updateUserGroup(const domain::UpdateUserGroupPayload& payload) const
{
    try
    {
        {
            odb::transaction transaction(db->begin());

            typedef odb::query<UserGroup> query;

            std::shared_ptr<UserGroup> userGroup(db->query_one<UserGroup>(query::id == payload.userGroup.getId()));

            if (!userGroup)
            {
                throw errors::ResourceNotFoundError{
                    std::format("UserGroup with id \"{}\" not found.", payload.userGroup.getId())};
            }

            const auto lastReadMessageId = payload.userGroup.getLastReadMessageId();

            if (lastReadMessageId)
            {
                userGroup->setLastReadMessageId(*lastReadMessageId);

                db->update(*userGroup);
            }

            transaction.commit();
        }

        return *findUserGroupById({payload.userGroup.getId()});
    }
    catch (const std::exception& error)
    {
        throw errors::UserGroupRepositoryError{error.what()};
    }
}

void UserGroupRepositoryImpl::deleteUserGroup(const domain::DeleteUserGroupPayload& payload) const
{
    try
    {
        odb::transaction transaction(db->begin());

        db->erase<UserGroup>(payload.userGroup.getId());

        transaction.commit();
    }
    catch (const std::exception& error)
    {
        throw errors::UserGroupRepositoryError{error.what()};
    }
}

}
