#!/usr/bin/env bash

## also kill backgrounded tasks
## when stopping script with Ctrl-C
trap "trap - SIGTERM && kill -- -$$" SIGINT SIGTERM # EXIT
## http://stackoverflow.com/a/2173421

SYSTEMS_BOOST=1 # set to 0 to use own boost

OWN_BOOST=${HOME}/local/desktop_rootfilesystem/boost_1_60_0  # this is only used if SYSTEMS_BOOST is set to 0 !



SCRIPT_PATH=$(readlink -e "$BASH_SOURCE")
SCRIPT_DIR=$(dirname "$SCRIPT_PATH")
WITTY_CUSTOM_INSTALL_PATH=$(readlink -e $SCRIPT_DIR/../witty_install/)


mkdir -p build
cd       build


if [[ $SYSTEMS_BOOST -ne 0 ]]; then
    BOOST_PATH=
else
    BOOST_PATH=-DBOOST_ROOT=${OWN_BOOST}
fi


if [[ -n "$WITTY_CUSTOM_INSTALL_PATH" ]]; then
    ln -fs $(readlink -e $WITTY_CUSTOM_INSTALL_PATH/share/Wt/resources) $SCRIPT_DIR # include standard Wt resource files
    
    read -r -d '' CMAKE_CALL <<EOF
cmake  $BOOST_PATH \
       -DCMAKE_BUILD_TYPE=Release \
       -DWITTY_CUSTOM_INSTALL_PATH=$WITTY_CUSTOM_INSTALL_PATH \
       ..
EOF
else
    ln -fs $(readlink -e /usr/share/Wt/resources) $SCRIPT_DIR                       # include standard Wt resource files
    
    read -r -d '' CMAKE_CALL <<EOF
cmake  $BOOST_PATH \
       -DCMAKE_BUILD_TYPE=Release \
       ..
EOF
fi


echo -e "Calling Cmake with this command:\n$CMAKE_CALL\n"
$CMAKE_CALL

make -j8 -l8
