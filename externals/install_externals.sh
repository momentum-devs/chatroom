[ -z "$BUILD_TYPE" ] && export BUILD_TYPE='Debug'

# shellcheck disable=SC2016
COMMON_CMAKE_FLAGS='-DCMAKE_BUILD_TYPE:STRING=$BUILD_TYPE -DCMAKE_CXX_COMPILER=/usr/bin/clang++-16 -GNinja'

cd odb || exit
sudo ./configure.ac CXX=/usr/bin/clang++-16 || exit
make install
cd ../..

cd googletest || exit
sudo rm -rf build
mkdir build
cd build || exit
eval cmake .. "$COMMON_CMAKE_FLAGS"
ninja
sudo ninja install
cd ../..

cd json || exit
sudo rm -rf build
mkdir build
cd build || exit
eval cmake .. -DJSON_BuildTests=OFF "$COMMON_CMAKE_FLAGS"
ninja
sudo ninja install
cd ../..

cd cpp-jwt || exit
sudo rm -rf build
mkdir build
cd build || exit
eval cmake ..  -DCPP_JWT_BUILD_EXAMPLES=OFF -DCPP_JWT_BUILD_TESTS=OFF "$COMMON_CMAKE_FLAGS"
ninja
sudo ninja install
cd ../..
