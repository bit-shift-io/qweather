#!/bin/bash
mkdir -p build
cd build
cmake -DKDE_INSTALL_USE_QT_SYS_PATHS=ON ..
make
#sudo make install
$SHELL