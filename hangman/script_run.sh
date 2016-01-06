#!/usr/bin/env bash

## also kill backgrounded tasks
## when stopping script with Ctrl-C
trap "trap - SIGTERM && kill -- -$$" SIGINT SIGTERM # EXIT
## http://stackoverflow.com/a/2173421

SCRIPT_PATH=$(readlink -f "$BASH_SOURCE")
SCRIPT_DIR=$(dirname "$SCRIPT_PATH")


cd ${SCRIPT_DIR}
./build/hangman.wt   --docroot .    -c wt_config.xml   --http-address 0.0.0.0   --http-port 8080 &
PID_hangman=$!


firefox http://localhost:8080 &
PID_firefox=$!




echo "Hit enter to stop"
read

if [[ "$(ps -q $PID_firefox -o comm=)" ]]; then
    kill $PID_firefox # only kills firefox, if it was not already open
fi
kill $PID_hangman
echo
echo "hangman finished!"
