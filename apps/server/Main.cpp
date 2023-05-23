#include <orm/db.hpp>
#include <QDebug>

#include "../common/EnvironmentParser.h"
#include "infrastructure/DatabaseConfig.h"
#include "loguru.hpp"

using Orm::DB;

int main(int argc, char* argv[])
{
    loguru::g_preamble_date = false;

    loguru::init(argc, argv);

    common::EnvironmentParser environmentParser;

    auto dbUsername = environmentParser.parseString("DB_USERNAME");
    auto dbPassword = environmentParser.parseString("DB_PASSWORD");
    auto dbHost = environmentParser.parseString("DB_HOST");
    auto dbPort = environmentParser.parseString("DB_PORT");
    auto dbName = environmentParser.parseString("DB_NAME");

    const auto dbConfig = server::infrastructure::DatabaseConfig{
        dbUsername, dbPassword, dbHost, dbPort, dbName,
    };

    auto manager = DB::create({
        {"driver", "QSQLITE"},
        {"database", qEnvironmentVariable("DB_DATABASE", "HelloWorld.sqlite3")},
        {"check_database_exists", true},
    });

    auto posts = DB::select("select * from posts");

    while (posts.next())
        qDebug() << posts.value("id").toULongLong() << posts.value("name").toString();

    return 0;
}
