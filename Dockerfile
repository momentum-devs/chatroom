FROM ubuntu:23.10

RUN apt update -y \
    && apt install -y lld-16 ninja-build  build-essential libstdc++-13-dev git\
      clang-16 clang-tools-16 llvm-16 lcov cmake g++-12 \
      libboost-all-dev libgl1-mesa-dev libglx-dev libcurl4-openssl-dev \
      qt6-base-dev libqt6sql6-psql libqt6qml6 qt6-declarative-dev qt6-tools-dev qt6-tools-dev-tools qt6-l10n-tools qt6-svg-dev \
      qml6-module-qtquick-controls qml6-module-qtqml-workerscript qml6-module-qtquick-templates \
      postgresql libcutl-1.10 libpq-dev sqlite3 libsqlite3-dev odb libhiredis-dev

COPY externals externals

RUN cd externals \
    && ./install_externals.sh

CMD ["/bin/bash"]