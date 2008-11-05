#!/bin/sh

# specifing arch flag here rather than in CCFLAGS because CCFLAGS not set
# consistently in makefiles
export CC="gcc -arch ppc"
export LDFLAGS="-arch ppc"

cd src
make clean
# -e flag to allow override of CC and LDFLAGS from the environment
make -e
make install
cd -
strip bin/morpheus
cp bin/morpheus dist/bin/platform/Darwin_ppc-gcc4
svn commit dist/bin/platform/Darwin_ppc-gcc4/morpheus -m "build-auto"

