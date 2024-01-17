# run with sudo

[ -z "$BUILD_TYPE" ] && export BUILD_TYPE='Debug'

# shellcheck disable=SC2016
COMMON_CMAKE_FLAGS='-DCMAKE_BUILD_TYPE:STRING=$BUILD_TYPE -DCMAKE_CXX_COMPILER=/usr/bin/clang++-16 -GNinja'

cd googletest || exit
rm -rf build
mkdir build
cd build || exit
eval cmake .. "$COMMON_CMAKE_FLAGS"
ninja
ninja install
cd ../..

cd json || exit
rm -rf build
mkdir build
cd build || exit
eval cmake .. -DJSON_BuildTests=OFF "$COMMON_CMAKE_FLAGS"
ninja
ninja install
cd ../..

cd cpp-jwt || exit
rm -rf build
mkdir build
cd build || exit
eval cmake ..  -DCPP_JWT_BUILD_EXAMPLES=OFF -DCPP_JWT_BUILD_TESTS=OFF "$COMMON_CMAKE_FLAGS"
ninja
ninja install
cd ../..

#cd aws-sdk-cpp || exit
#rm -rf build
#mkdir build
#cd build || exit
#eval cmake ..  -DCMAKE_BUILD_TYPE=Debug \
#               -DCMAKE_INSTALL_PREFIX=<path-to-install> \
#               -DBUILD_ONLY="s3"
#ninja
#ninja install
#cd ../..
