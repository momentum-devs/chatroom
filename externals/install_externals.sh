[ -z "$BUILD_TYPE" ] && export BUILD_TYPE='Debug'

COMMON_CMAKE_FLAGS='-DCMAKE_BUILD_TYPE:STRING=$BUILD_TYPE -DCMAKE_CXX_COMPILER=/usr/bin/clang++-16 -GNinja'

cd googletest || exit
sudo rm -rf build
mkdir build
cd build || exit
eval cmake .. $COMMON_CMAKE_FLAGS
ninja
sudo ninja install
cd ../..

cd json || exit
sudo rm -rf build
mkdir build
cd build || exit
eval cmake .. -DJSON_BuildTests=OFF $COMMON_CMAKE_FLAGS
ninja
sudo ninja install
cd ../..

cd tabulate || exit
sudo rm -rf build
mkdir build
cd build || exit
eval cmake .. -DUSE_CPP17=ON $COMMON_CMAKE_FLAGS
ninja
sudo ninja install
cd ../..

cd TinyORM || exit
#sed -i '17,21{H;d};22{g;s/.*/#include <variant>\n/}' include/orm/ormconcepts.hpp
sudo rm -rf build
mkdir build
cd build || exit
eval cmake .. -DTOM:BOOL=ON -D BUILD_TESTS:BOOL=OFF $COMMON_CMAKE_FLAGS
ninja
sudo ninja install
cd ../..

cd cpp-jwt || exit
sudo rm -rf build
mkdir build
cd build || exit
eval cmake ..  -DCPP_JWT_BUILD_EXAMPLES=OFF -DCPP_JWT_BUILD_TESTS=OFF $COMMON_CMAKE_FLAGS
ninja
sudo ninja install
cd ../..
