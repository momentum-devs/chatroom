DROP TABLE IF EXISTS "users" CASCADE;

CREATE TABLE "users"
(
    "id"                TEXT    NOT NULL PRIMARY KEY,
    "email"             TEXT    NOT NULL,
    "password"          TEXT    NOT NULL,
    "nickname"          TEXT    NOT NULL,
    "active"            BOOLEAN NOT NULL,
    "email_verified"    BOOLEAN NOT NULL,
    "verification_code" TEXT    NOT NULL,
    "created_at"        TEXT    NOT NULL,
    "updated_at"        TEXT    NOT NULL
);

DROP TABLE IF EXISTS "channels" CASCADE;

CREATE TABLE "channels"
(
    "id"         TEXT NOT NULL PRIMARY KEY,
    "name"       TEXT NOT NULL,
    "creator"    TEXT NOT NULL,
    "created_at" TEXT NOT NULL,
    "updated_at" TEXT NOT NULL
);

ALTER TABLE "channels"
    ADD CONSTRAINT "creator_fk"
        FOREIGN KEY ("creator")
            REFERENCES "users" ("id") ON DELETE CASCADE
            INITIALLY DEFERRED;


DROP TABLE IF EXISTS "users_channels" CASCADE;

CREATE TABLE "users_channels"
(
    "id"                   TEXT NOT NULL PRIMARY KEY,
    "created_at"           TEXT NOT NULL,
    "last_read_message_id" TEXT NULL,
    "user"                 TEXT NOT NULL,
    "channel"              TEXT NOT NULL
);

ALTER TABLE "users_channels"
    ADD CONSTRAINT "user_fk"
        FOREIGN KEY ("user")
            REFERENCES "users" ("id") ON DELETE CASCADE
            INITIALLY DEFERRED,
    ADD CONSTRAINT "channel_fk"
        FOREIGN KEY ("channel")
            REFERENCES "channels" ("id") ON DELETE CASCADE
            INITIALLY DEFERRED;

DROP TABLE IF EXISTS "channels_invitations" CASCADE;

CREATE TABLE "channels_invitations"
(
    "id"         TEXT NOT NULL PRIMARY KEY,
    "created_at" TEXT NOT NULL,
    "sender"     TEXT NOT NULL,
    "recipient"  TEXT NOT NULL,
    "channel"    TEXT NOT NULL
);

ALTER TABLE "channels_invitations"
    ADD CONSTRAINT "sender_fk"
        FOREIGN KEY ("sender")
            REFERENCES "users" ("id") ON DELETE CASCADE
            INITIALLY DEFERRED,
    ADD CONSTRAINT "recipient_fk"
        FOREIGN KEY ("recipient")
            REFERENCES "users" ("id") ON DELETE CASCADE
            INITIALLY DEFERRED,
    ADD CONSTRAINT "channel_fk"
        FOREIGN KEY ("channel")
            REFERENCES "channels" ("id") ON DELETE CASCADE
            INITIALLY DEFERRED;

DROP TABLE IF EXISTS "friends_invitations" CASCADE;

CREATE TABLE "friends_invitations"
(
    "id"         TEXT NOT NULL PRIMARY KEY,
    "created_at" TEXT NOT NULL,
    "sender"     TEXT NOT NULL,
    "recipient"  TEXT NOT NULL
);

ALTER TABLE "friends_invitations"
    ADD CONSTRAINT "sender_fk"
        FOREIGN KEY ("sender")
            REFERENCES "users" ("id") ON DELETE CASCADE
            INITIALLY DEFERRED,
    ADD CONSTRAINT "recipient_fk"
        FOREIGN KEY ("recipient")
            REFERENCES "users" ("id") ON DELETE CASCADE
            INITIALLY DEFERRED;

DROP TABLE IF EXISTS "friendships" CASCADE;

CREATE TABLE "friendships"
(
    "id"          TEXT NOT NULL PRIMARY KEY,
    "created_at"  TEXT NOT NULL,
    "user"        TEXT NOT NULL,
    "user_friend" TEXT NOT NULL
);

ALTER TABLE "friendships"
    ADD CONSTRAINT "user_fk"
        FOREIGN KEY ("user")
            REFERENCES "users" ("id") ON DELETE CASCADE
            INITIALLY DEFERRED,
    ADD CONSTRAINT "user_friend_fk"
        FOREIGN KEY ("user_friend")
            REFERENCES "users" ("id") ON DELETE CASCADE
            INITIALLY DEFERRED;

DROP TABLE IF EXISTS "groups" CASCADE;

CREATE TABLE "groups"
(
    "id"         TEXT NOT NULL PRIMARY KEY,
    "created_at" TEXT NOT NULL
);

DROP TABLE IF EXISTS "users_groups" CASCADE;

CREATE TABLE "users_groups"
(
    "id"                   TEXT NOT NULL PRIMARY KEY,
    "created_at"           TEXT NOT NULL,
    "last_read_message_id" TEXT NULL,
    "user"                 TEXT NOT NULL,
    "group"                TEXT NOT NULL
);

ALTER TABLE "users_groups"
    ADD CONSTRAINT "user_fk"
        FOREIGN KEY ("user")
            REFERENCES "users" ("id") ON DELETE CASCADE
            INITIALLY DEFERRED,
    ADD CONSTRAINT "group_fk"
        FOREIGN KEY ("group")
            REFERENCES "groups" ("id") ON DELETE CASCADE
            INITIALLY DEFERRED;

DROP TABLE IF EXISTS "messages" CASCADE;

CREATE TABLE "messages"
(
    "id"         TEXT NOT NULL PRIMARY KEY,
    "created_at" TEXT NOT NULL,
    "updated_at" TEXT NOT NULL,
    "content"    TEXT NOT NULL,
    "sender"     TEXT NOT NULL,
    "channel"    TEXT,
    "group"      TEXT
);

ALTER TABLE "messages"
    ADD CONSTRAINT "sender_fk"
        FOREIGN KEY ("sender")
            REFERENCES "users" ("id") ON DELETE CASCADE
            INITIALLY DEFERRED,
    ADD CONSTRAINT "channel_fk"
        FOREIGN KEY ("channel")
            REFERENCES "channels" ("id") ON DELETE CASCADE
            INITIALLY DEFERRED,
    ADD CONSTRAINT "group_fk"
        FOREIGN KEY ("group")
            REFERENCES "groups" ("id") ON DELETE CASCADE
            INITIALLY DEFERRED;
