#pragma once

#include <orm/tiny/model.hpp>

namespace Models
{
using Orm::Tiny::Model;

class User final : public Model<User>
{
    friend Model;
    using Model::Model;

    inline static const QStringList u_fillable{"email", "password", "nickname"};
};
}
