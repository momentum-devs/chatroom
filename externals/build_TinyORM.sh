cd TinyORM || exit
mkdir build
cd build || exit
cmake .. -DTOM=OFF -DCMAKE_BUILD_TYPE:STRING='Debug' -DCMAKE_CXX_COMPILER=/usr/bin/clang++-15
make -j 20
sudo make install
