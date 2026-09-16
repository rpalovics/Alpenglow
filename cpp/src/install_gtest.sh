cd ../dep

# Preserve the deliberately minimal gtest directory
mv gtest gtest_old

wget https://github.com/google/googletest/archive/release-1.7.0.zip
unzip release-1.7.0.zip
mv googletest-release-1.7.0 gtest

cd gtest
GTEST_DIR=$(pwd)

mkdir build
cd build

g++ -isystem ${GTEST_DIR}/include \
    -I${GTEST_DIR} \
    -pthread \
    -c ${GTEST_DIR}/src/gtest-all.cc

ar -rv libgtest.a gtest-all.o

cd ../../src
