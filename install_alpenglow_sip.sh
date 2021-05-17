export VER=4.19.25
mkdir -p sip_build
pushd sip_build
wget https://www.riverbankcomputing.com/static/Downloads/sip/${VER}/sip-${VER}.tar.gz
tar -xvzf sip-${VER}.tar.gz
pushd sip-${VER}
python configure.py --sip-module alpenglow.sip --sysroot=${CONDA_PREFIX}

make
make install
popd
popd