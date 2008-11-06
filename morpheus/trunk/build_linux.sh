#!/bin/sh

rm -f dist/bin/platform/Linux_x86-gcc3/morpheus

cd src
make clean
make -e 
cd -
strip bin/morpheus
cp bin/morpheus dist/bin/platform/Linux_x86-gcc4
svn commit dist/bin/platform/Linux_x86-gcc4/morpheus -m "build-auto"
