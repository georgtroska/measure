#!/bin/bash -e

PREFIX_DIR=`mktemp -t -d measure-inst.XXXXXXXX`
echo "Install prefix: ${PREFIX_DIR}"

./autogen.sh
./configure --prefix="${PREFIX_DIR}"

make -j4 install


export PATH="${PREFIX_DIR}/bin:${PATH}"
export LD_LIBRARY_PATH="${PREFIX_DIR}/lib:${LD_LIBRARY_PATH}"

measure
root -l -b -q src/measure.C


make uninstall
rmdir "${PREFIX_DIR}"/*/*/*
rmdir "${PREFIX_DIR}"/*/*
rmdir "${PREFIX_DIR}"/*
rmdir "${PREFIX_DIR}"

make maintainer-clean


echo OK
