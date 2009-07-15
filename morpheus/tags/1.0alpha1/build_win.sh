#!/usr/bin/sh
#requires MinGW GCC, Gnu make, GnuWin32 flex

cd src
make clean
make
make install
cd -
strip bin/morpheus.exe
cp bin/morpheus.exe dist/bin/platform/WINNT_x86-gcc3
svn commit dist/bin/platform/WINNT_x86-gcc3/morpheus.exe -m "build-auto"
