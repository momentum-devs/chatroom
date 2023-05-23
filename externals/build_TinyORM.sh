cd TinyORM || exit
mkdir build
cd build || exit
cmake .. -DTOM=OFF -DCMAKE_BUILD_TYPE:STRING='Debug'
make -j 20
sudo make install
