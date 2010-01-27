#!/bin/sh

[ -d dist/stemlib/Greek ] || mkdir dist/stemlib/Greek
[ -d dist/stemlib/Greek/derivs ] || mkdir dist/stemlib/Greek/derivs
[ -d dist/stemlib/Greek/derivs/indices ] || mkdir dist/stemlib/Greek/derivs/indices
[ -d dist/stemlib/Greek/endtables ] || mkdir dist/stemlib/Greek/endtables
[ -d dist/stemlib/Greek/endtables/indices ] || mkdir dist/stemlib/Greek/endtables/indices
[ -d dist/stemlib/Greek/endtables/out ] || mkdir dist/stemlib/Greek/endtables/out

PWD=`pwd`
export MORPHLIB=$PWD/stemlib
export PATH=$PATH:$PWD/src/gkdict:$PWD/src/gener:$PWD/src/gkends
cd stemlib/Greek
make clean
# make stemlib twice - 2nd make picks up additional stems
make
make 
cd -
cp -fr stemlib/Greek/derivs/indices dist/stemlib/Greek/derivs
cp -fr stemlib/Greek/endtables/indices dist/stemlib/Greek/endtables
cp -fr stemlib/Greek/endtables/out dist/stemlib/Greek/endtables
cp -fr stemlib/Greek/rule_files dist/stemlib/Greek
cp -fr stemlib/Greek/steminds dist/stemlib/Greek
cp -fr stemlib/Greek/stemsrc dist/stemlib/Greek

# remove unnecessary binaries from distribution
rm dist/stemlib/Greek/stemsrc/flatlems
rm dist/stemlib/Greek/stemsrc/headnolen
rm dist/stemlib/Greek/stemsrc/stripref
rm dist/stemlib/Greek/stemsrc/zapfirstf
rm dist/stemlib/Greek/stemsrc/goodstem
