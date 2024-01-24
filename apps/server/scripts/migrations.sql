PRAGMA foreign_keys=off;

DROP TABLE IF EXISTS users_channels;
DROP TABLE IF EXISTS channels_invitations;
DROP TABLE IF EXISTS friends_invitations;
DROP TABLE IF EXISTS friendships;
DROP TABLE IF EXISTS users_groups;
DROP TABLE IF EXISTS messages;
DROP TABLE IF EXISTS reactions;
DROP TABLE IF EXISTS users;
DROP TABLE IF EXISTS channels;
DROP TABLE IF EXISTS groups;
DROP TABLE IF EXISTS users_groups;
DROP TABLE IF EXISTS blacklist_tokens;

PRAGMA foreign_keys=on;

CREATE TABLE users
(
    id                  TEXT    NOT NULL PRIMARY KEY,
    email               TEXT    NOT NULL,
    password            TEXT    NOT NULL,
    nickname            TEXT    NOT NULL,
    active              INTEGER NOT NULL,
    email_verified      INTEGER NOT NULL,
    verification_code   TEXT    NOT NULL,
    reset_password_code TEXT    NULL,
    created_at          TEXT    NOT NULL,
    updated_at          TEXT    NOT NULL,
    avatar_url          TEXT    NULL
);

CREATE TABLE channels
(
    id         TEXT NOT NULL PRIMARY KEY,
    name       TEXT NOT NULL,
    creator    TEXT NOT NULL,
    created_at TEXT NOT NULL,
    updated_at TEXT NOT NULL,
    avatar_url TEXT NULL,
    FOREIGN KEY (creator) REFERENCES users (id) ON DELETE CASCADE
);

CREATE TABLE users_channels
(
    id                   TEXT NOT NULL PRIMARY KEY,
    created_at           TEXT NOT NULL,
    last_read_message_id TEXT NULL,
    "user"               TEXT NOT NULL,
    channel              TEXT NOT NULL,
    FOREIGN KEY ("user") REFERENCES users (id) ON DELETE CASCADE,
    FOREIGN KEY (channel) REFERENCES channels (id) ON DELETE CASCADE
);

CREATE TABLE channels_invitations
(
    id         TEXT NOT NULL PRIMARY KEY,
    created_at TEXT NOT NULL,
    sender     TEXT NOT NULL,
    recipient  TEXT NOT NULL,
    channel    TEXT NOT NULL,
    FOREIGN KEY (sender) REFERENCES users (id) ON DELETE CASCADE,
    FOREIGN KEY (recipient) REFERENCES users (id) ON DELETE CASCADE,
    FOREIGN KEY (channel) REFERENCES channels (id) ON DELETE CASCADE
);

CREATE TABLE friends_invitations
(
    id         TEXT NOT NULL PRIMARY KEY,
    created_at TEXT NOT NULL,
    sender     TEXT NOT NULL,
    recipient  TEXT NOT NULL,
    FOREIGN KEY (sender) REFERENCES users (id) ON DELETE CASCADE,
    FOREIGN KEY (recipient) REFERENCES users (id) ON DELETE CASCADE
);

CREATE TABLE friendships
(
    id          TEXT NOT NULL PRIMARY KEY,
    created_at  TEXT NOT NULL,
    "user"      TEXT NOT NULL,
    user_friend TEXT NOT NULL,
    "group"     TEXT NOT NULL,
    FOREIGN KEY ("user") REFERENCES users (id) ON DELETE CASCADE,
    FOREIGN KEY (user_friend) REFERENCES users (id) ON DELETE CASCADE,
    FOREIGN KEY ("group") REFERENCES groups (id) ON DELETE CASCADE
);

CREATE TABLE groups
(
    id         TEXT NOT NULL PRIMARY KEY,
    created_at TEXT NOT NULL
);

CREATE TABLE users_groups
(
    id                   TEXT NOT NULL PRIMARY KEY,
    created_at           TEXT NOT NULL,
    last_read_message_id TEXT NULL,
    "user"               TEXT NOT NULL,
    "group"              TEXT NOT NULL,
    FOREIGN KEY ("user") REFERENCES users (id) ON DELETE CASCADE,
    FOREIGN KEY ("group") REFERENCES groups (id) ON DELETE CASCADE
);

CREATE TABLE messages
(
    id         TEXT NOT NULL PRIMARY KEY,
    created_at TEXT NOT NULL,
    updated_at TEXT NOT NULL,
    content    TEXT NOT NULL,
    sender     TEXT NOT NULL,
    channel    TEXT,
    "group"    TEXT,
    FOREIGN KEY (sender) REFERENCES users (id) ON DELETE CASCADE,
    FOREIGN KEY (channel) REFERENCES channels (id) ON DELETE CASCADE,
    FOREIGN KEY ("group") REFERENCES groups (id) ON DELETE CASCADE
);

CREATE TABLE reactions
(
    id      TEXT NOT NULL PRIMARY KEY,
    name    TEXT NOT NULL,
    "user"  TEXT NOT NULL,
    message TEXT NOT NULL,
    FOREIGN KEY ("user") REFERENCES users (id) ON DELETE CASCADE,
    FOREIGN KEY (message) REFERENCES messages (id) ON DELETE CASCADE
);

CREATE TABLE blacklist_tokens
(
    id         TEXT NOT NULL PRIMARY KEY,
    token      TEXT NOT NULL,
    expires_at TEXT NOT NULL
);
