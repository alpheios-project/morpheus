#!/bin/sh

# specifing arch flag here rather than in CCFLAGS because CCFLAGS not set
# consistently in makefiles
export CC="gcc -arch i386 -isysroot /Developer/SDKs/MacOSX10.4u.sdk -mmacosx-version-min=10.4"
export LDFLAGS="-arch i386 -Wl,-syslibroot,/Developer/SDKs/MacOSX10.4u.sdk -mmacosx-version-min=10.4"

cd src
make clean
# -e flag to allow override of CC and LDFLAGS from the environment
make -e
make install
cd -
strip bin/morpheus
cp bin/morpheus dist/bin/platform/Darwin_x86-gcc4
svn commit dist/bin/platform/Darwin_x86-gcc4/morpheus -m "build-auto"

