## Setup local development:

<br/>

#### Download project

```git clone git@github.com:momentum-devs/chatroom.git```

```git submodule init --update --recursive```

#### Install build tools:

```sudo apt install sccache clang-15 libc++-15-dev libc++abi-15-dev build-essential libboost-all-dev libgl1-mesa-dev libglx-dev lld ninja-build```

#### Install Qt:

```sudo apt install qt6-base-dev libqt6sql6-psql libqt6qml6 qt6-declarative-dev qt6-tools-dev qt6-tools-dev-tools qt6-l10n-tools qt6-svg-dev qml6-module-qtquick-controls qml6-module-qtqml-workerscript qml6-module-qtquick-templates qml6-module-qt5compat-graphicaleffects```

#### Install ODB:

```sudo apt install libcutl-1.10 postgresql libpq-dev sqlite3 libsqlite3-dev odb```

#### Install libs:

```cd externals && ./install_externals.sh```

### Run docker compose:

```chmod 777 start_dev.sh```

```./start_dev.sh```
