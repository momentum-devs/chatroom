cd googletest || exit
rm build
mkdir build
cd build || exit
cmake .. -DCMAKE_BUILD_TYPE:STRING='Debug' -DCMAKE_CXX_COMPILER=/usr/bin/clang++-16
make -j 20
sudo make install
cd ../..

cd json || exit
rm build
mkdir build
cd build || exit
cmake .. -DCMAKE_BUILD_TYPE:STRING='Debug' -DCMAKE_CXX_COMPILER=/usr/bin/clang++-15
make -j 20
sudo make install
cd ../..

cd tabulate || exit
rm build
mkdir build
cd build || exit
cmake .. -DUSE_CPP17=ON -DCMAKE_BUILD_TYPE:STRING='Debug' -DCMAKE_CXX_COMPILER=/usr/bin/clang++-15
make -j 20
sudo make install
cd ../..

cd TinyORM || exit
sed '146d' -i cmake/CommonModules/TinyCommon.cmake || exit
rm build
mkdir build
cd build || exit
cmake .. -DTOM=ON -DCMAKE_BUILD_TYPE:STRING='Debug' -DCMAKE_CXX_COMPILER=/usr/bin/clang++-15
make -j 20
sudo make install
