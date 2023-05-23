cd TinyORM || exit
mkdir build
cd build || exit
cmake .. -DCMAKE_PREFIX_PATH=/home/michal/Qt5/5.15.2/gcc_64 -DTOM=OFF -DCMAKE_BUILD_TYPE:STRING='Debug'
make -j 4
sudo make install
