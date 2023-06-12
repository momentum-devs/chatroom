#ifndef _DATABASE_INCLUDED_
#define _DATABASE_INCLUDED_

#include <odb/sqlite/database.hxx>

class Database : public odb::sqlite::database
{
public:
    Database(const std::string& name, bool clear = false, int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
             bool foreign_keys = true, const std::string& vfs = "",
             odb::details::transfer_ptr<odb::sqlite::connection_factory> =
                 odb::details::transfer_ptr<odb::sqlite::connection_factory>());
};

#endif
