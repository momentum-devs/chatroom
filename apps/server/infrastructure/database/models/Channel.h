#pragma once

#include <orm/tiny/model.hpp>

#include "User.h"

namespace Models
{

using Orm::Tiny::Model;
using Orm::Tiny::Relations::BelongsToMany;

class User;

class Channel final : public Model<Channel, User>
{
    friend Model;
    using Model::Model;

public:
    std::unique_ptr<BelongsToMany<Channel, User>> users()
    {
        return belongsToMany<User>({}, {}, "creator_id");
    }

private:
    QHash<QString, RelationVisitor> u_relations{
        {"users", [](auto& v) { v(&Channel::users); }},
    };

    inline static const QStringList u_fillable{"name"};
};

}
