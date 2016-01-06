#!/usr/bin/env bash

## also kill backgrounded tasks
## when stopping script with Ctrl-C
trap "trap - SIGTERM && kill -- -$$" SIGINT SIGTERM # EXIT
## http://stackoverflow.com/a/2173421

SCRIPT_PATH=$(readlink -f "$BASH_SOURCE")
SCRIPT_DIR=$(dirname "$SCRIPT_PATH")


cd ${SCRIPT_DIR}
./build/go --http-address 0.0.0.0 --http-port 9090 --docroot ./docroot/ --approot ./approot/ &
PID_go=$!



firefox http://localhost:9090 &
PID_firefox=$!




echo "Hit enter to stop"
read

if [[ "$(ps -q $PID_firefox -o comm=)" ]]; then
    kill $PID_firefox # only kills firefox, if it was not already open
fi
kill $PID_go
echo
echo "...finished!"
