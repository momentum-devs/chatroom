# chatroom

[![build and test](https://github.com/momentum-devs/chatroom/actions/workflows/buildAndTest.yml/badge.svg?branch=main)](https://github.com/momentum-devs/chatroom/actions/workflows/buildAndTest.yml?query=branch%3Amain)
[![codecov](https://codecov.io/github/momentum-devs/chatroom/branch/main/graph/badge.svg?token=0RTV4JFH2U)](https://codecov.io/github/momentum-devs/chatroom)

### Setup local development:

#### Download project

```git clone git@github.com:momentum-devs/chatroom.git```

```git submodule init --update --recursive```

#### Install build tools:

```sudo apt install sccache clang-15 libc++-15-dev libc++abi-15-dev build-essential libboost-all-dev libgl1-mesa-dev libglx-dev lld ninja-build```

#### Install Qt:

```sudo apt install qt6-base-dev libqt6sql6-psql libqt6qml6 qt6-declarative-dev qt6-tools-dev qt6-tools-dev-tools qt6-l10n-tools qml6-module-qtquick-controls qml6-module-qtqml-workerscript qml6-module-qtquick-templates```

#### Install ODB:

```sudo apt install libcutl-1.10 postgresql libpq-dev sqlite3 libsqlite3-dev odb```

#### Install libs:

```cd externals && ./install_externals.sh```

### Run database:

```chmod 777 start_dev.sh```

```./start_dev.sh```

### [DOCS](docs/README.md)

### TODO:

// CLIENT:

* create channel invitation
* reject channel invitation
* leave channel

* send friend invitation
* show list of friends
* friend online/offline
* accept friend invitation
* reject friend invitation
* remove from friends
* user online/offline

* direct messages between users
* messages in channel
* user has history of direct messages and messages in channel
* push messages, invitations, online/offline instead of polling

// SERVER:

* generate verification code
* sender email from config
* direct messages between users
* messages in channel
* user has history of direct messages and messages in channel
