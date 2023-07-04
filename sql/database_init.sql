DROP TABLE IF EXISTS "users" CASCADE;

CREATE TABLE "users"
(
    "id"         TEXT    NOT NULL PRIMARY KEY,
    "email"      TEXT    NOT NULL,
    "password"   TEXT    NOT NULL,
    "nickname"   TEXT    NOT NULL,
    "active"     BOOLEAN NOT NULL,
    "created_at" TEXT    NOT NULL,
    "updated_at" TEXT    NOT NULL
);

DROP TABLE IF EXISTS "channels" CASCADE;

CREATE TABLE "channels"
(
    "id"         TEXT NOT NULL PRIMARY KEY,
    "name"       TEXT NOT NULL,
    "creator_id" TEXT NOT NULL,
    "created_at" TEXT NOT NULL,
    "updated_at" TEXT NOT NULL
);


DROP TABLE IF EXISTS "users_channels" CASCADE;

CREATE TABLE "users_channels"
(
    "id"         TEXT NOT NULL PRIMARY KEY,
    "created_at" TEXT NOT NULL,
    "updated_at" TEXT NOT NULL,
    "user"       TEXT NOT NULL,
    "channel"    TEXT NOT NULL
);

ALTER TABLE "users_channels"
    ADD CONSTRAINT "user_fk"
        FOREIGN KEY ("user")
            REFERENCES "users" ("id")
            INITIALLY DEFERRED,
    ADD CONSTRAINT "channel_fk"
        FOREIGN KEY ("channel")
            REFERENCES "channels" ("id")
            INITIALLY DEFERRED;
