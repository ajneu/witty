#!/usr/bin/env bash

##
# NOTE: you man also choose to not run this script. Then ->
#       just use:
#
#       sudo apt-get install witty witty-dev
##

SYSTEMS_BOOST=1 # set to 0 to use own boost

OWN_BOOST=${HOME}/local/desktop_rootfilesystem/boost_1_60_0  # this is only used if SYSTEMS_BOOST is set to 0 !





SCRIPT_PATH=$(readlink -f "$BASH_SOURCE")
SCRIPT_DIR=$(dirname "$SCRIPT_PATH")

mkdir -p $SCRIPT_DIR/witty_build
cd       $SCRIPT_DIR/witty_build





if [[ $SYSTEMS_BOOST -ne 0 ]]; then
    BOOST_PKG=libboost-all-dev
else
    BOOST_PKG=
fi

if cat /etc/*-release | grep jessie &> /dev/null; then
    ## # Debian Jessie
    sudo apt-get install $BOOST_PKG doxygen libqt4-dev libgraphicsmagick1-dev libgraphicsmagick3      libssl-dev libhpdf-dev libfcgi-dev libpq-dev libpangoft2-1.0-0 libpango1.0-dev
else
    ## # e.g. Debian Stretch
    sudo apt-get install $BOOST_PKG doxygen libqt4-dev libgraphicsmagick1-dev libgraphicsmagick-q16-3 libssl-dev libhpdf-dev libfcgi-dev libpq-dev libpangoft2-1.0-0 libpango1.0-dev
fi

if [[ ! -f                                      3.3.5.tar.gz ]]; then
   wget https://github.com/kdeforche/wt/archive/3.3.5.tar.gz
   tar xf                                       3.3.5.tar.gz
fi

cd                                           wt*

mkdir -p build
cd       build

INSTALL=$SCRIPT_DIR/witty_install

if [[ $SYSTEMS_BOOST -ne 0 ]]; then
    BOOST_PATH=
else
    BOOST_PATH=-DBOOST_ROOT=${OWN_BOOST}
fi

cmake -DWT_WRASTERIMAGE_IMPLEMENTATION=GraphicsMagick -DWT_CPP_11_MODE=-std=c++11 \
      ${BOOST_PATH}                                            \
      -DCMAKE_INSTALL_PREFIX=${INSTALL}                        \
      -DCONFIGURATION=${INSTALL}/etc/wt/wt_config.xml          \
      -DRUNDIR=${INSTALL}/var/run/wt                           \
      -DWTHTTP_CONFIGURATION=${INSTALL}/etc/wt/wthttpd         \
      -DCONFIGDIR=${INSTALL}/etc/wt ..

make -j8 -l8
make install
