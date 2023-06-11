# chatroom

[![build and test](https://github.com/momentum-devs/chatroom/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/momentum-devs/chatroom/actions/workflows/cmake.yml?query=branch%3Amain)
[![codecov](https://codecov.io/github/momentum-devs/chatroom/branch/main/graph/badge.svg?token=0RTV4JFH2U)](https://codecov.io/github/momentum-devs/chatroom)

### Setup local development:

1. ```sudo apt install sccache clang-15 libc++-15-dev libc++abi-15-dev build-essential libboost-all-dev libgl1-mesa-dev libglx-dev librange-v3-dev lld ninja-build qt6-base-dev libqt6sql6-psql libqt6qml6 qt6-declarative-dev qt6-tools-dev qt6-tools-dev-tools qt6-l10n-tools qml6-module-qtquick-controls qml6-module-qtqml-workerscript qml6-module-qtquick-templates```

2. ```cd externals && ./build_TinyORM.sh```

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
