CREATE TABLE "friends"
(
    "id"         text PRIMARY KEY,
    "user_id"    text UNIQUE NOT NULL,
    "friend_id"  text        NOT NULL,
    "created_at" timestamp   NOT NULL,
    "updated_at" timestamp   NOT NULL
);

CREATE TABLE "friends_requests"
(
    "id"           text PRIMARY KEY,
    "sender_id"    text      NOT NULL,
    "recipient_id" text      NOT NULL,
    "created_at"   timestamp NOT NULL,
    "updated_at"   timestamp NOT NULL
);

CREATE TABLE "users"
(
    "id"         text PRIMARY KEY,
    "email"      text UNIQUE NOT NULL,
    "password"   text        NOT NULL,
    "nickname"   text        NOT NULL,
    "created_at" timestamp   NOT NULL,
    "updated_at" timestamp   NOT NULL
);

CREATE TABLE "direct_conversations"
(
    "id"              text PRIMARY KEY,
    "user_id"         text      NOT NULL,
    "recipient_id"    text      NOT NULL,
    "conversation_id" text      NOT NULL,
    "created_at"      timestamp NOT NULL,
    "updated_at"      timestamp NOT NULL
);

CREATE TABLE "channels"
(
    "id"         text PRIMARY KEY,
    "name"       text UNIQUE NOT NULL,
    "creator_id" text        NOT NULL,
    "created_at" timestamp   NOT NULL,
    "updated_at" timestamp   NOT NULL
);

CREATE TABLE "users_channels"
(
    "id"         text PRIMARY KEY,
    "user_id"    text      NOT NULL,
    "channel_id" text      NOT NULL,
    "created_at" timestamp NOT NULL,
    "updated_at" timestamp NOT NULL
);

CREATE TABLE "users_channels_requests"
(
    "id"           text PRIMARY KEY,
    "sender_id"    text      NOT NULL,
    "recipient_id" text      NOT NULL,
    "channel_id"   text      NOT NULL,
    "created_at"   timestamp NOT NULL,
    "updated_at"   timestamp NOT NULL
);

CREATE TABLE "channels_conversations"
(
    "id"              text PRIMARY KEY,
    "channel_id"      text      NOT NULL,
    "name"            text      NOT NULL,
    "conversation_id" text      NOT NULL,
    "created_at"      timestamp NOT NULL,
    "updated_at"      timestamp NOT NULL
);

CREATE TABLE "conversations"
(
    "id"         text PRIMARY KEY,
    "created_at" timestamp NOT NULL,
    "updated_at" timestamp NOT NULL
);

CREATE TABLE "messages"
(
    "id"              text PRIMARY KEY,
    "content"         text      NOT NULL,
    "conversation_id" text      NOT NULL,
    "sender_id"       text      NOT NULL,
    "created_at"      timestamp NOT NULL,
    "updated_at"      timestamp NOT NULL
);

CREATE TABLE "messages_reactions"
(
    "id"            text PRIMARY KEY,
    "user_id"       text      NOT NULL,
    "message_id"    text      NOT NULL,
    "reaction_name" text      NOT NULL,
    "created_at"    timestamp NOT NULL,
    "updated_at"    timestamp NOT NULL
);

ALTER TABLE "friends"
    ADD FOREIGN KEY ("user_id") REFERENCES "users" ("id");

ALTER TABLE "friends"
    ADD FOREIGN KEY ("friend_id") REFERENCES "users" ("id");

ALTER TABLE "friends_requests"
    ADD FOREIGN KEY ("sender_id") REFERENCES "users" ("id");

ALTER TABLE "friends_requests"
    ADD FOREIGN KEY ("recipient_id") REFERENCES "users" ("id");

ALTER TABLE "users_channels_requests"
    ADD FOREIGN KEY ("sender_id") REFERENCES "users" ("id");

ALTER TABLE "users_channels_requests"
    ADD FOREIGN KEY ("recipient_id") REFERENCES "users" ("id");

ALTER TABLE "direct_conversations"
    ADD FOREIGN KEY ("user_id") REFERENCES "users" ("id");

ALTER TABLE "direct_conversations"
    ADD FOREIGN KEY ("recipient_id") REFERENCES "users" ("id");

ALTER TABLE "channels"
    ADD FOREIGN KEY ("creator_id") REFERENCES "users" ("id");

ALTER TABLE "users_channels"
    ADD FOREIGN KEY ("user_id") REFERENCES "users" ("id");

ALTER TABLE "users_channels"
    ADD FOREIGN KEY ("channel_id") REFERENCES "channels" ("id");

ALTER TABLE "channels_conversations"
    ADD FOREIGN KEY ("channel_id") REFERENCES "channels" ("id");

ALTER TABLE "users_channels_requests"
    ADD FOREIGN KEY ("channel_id") REFERENCES "channels" ("id");

ALTER TABLE "channels_conversations"
    ADD FOREIGN KEY ("conversation_id") REFERENCES "conversations" ("id");

ALTER TABLE "direct_conversations"
    ADD FOREIGN KEY ("conversation_id") REFERENCES "conversations" ("id");

ALTER TABLE "messages"
    ADD FOREIGN KEY ("conversation_id") REFERENCES "conversations" ("id");

ALTER TABLE "messages_reactions"
    ADD FOREIGN KEY ("user_id") REFERENCES "users" ("id");

ALTER TABLE "messages_reactions"
    ADD FOREIGN KEY ("message_id") REFERENCES "messages" ("id");

ALTER TABLE "messages"
    ADD FOREIGN KEY ("sender_id") REFERENCES "users" ("id");
