# chatroom

### Setup local development:

1. ```sudo apt-get install build-essential qt6-base-dev librange-v3-dev lld clang-16```

6. ```cd externals && ./build_TinyORM.sh```

### Run database:

```systemctl stop postgresql```

```docker-compose up```

### [DOCS](docs/README.md)

### TODO:

* create user
* delete user
* update user
* create chatroom
* delete chatroom
* update chatroom
* join chatroom
* leave chatroom
* send message to chatroom
* receive message in chatroom
* direct messages between users
* add user to friends
* user has history of direct messages and messages in chatroom
* active users info
* message encryption
* show list of friends and chatrooms
* add admin user role in chatroom
* add reaction to messages
