#!/usr/bin/env bash

## also kill backgrounded tasks
## when stopping script with Ctrl-C
trap "trap - SIGTERM && kill -- -$$" SIGINT SIGTERM # EXIT
## http://stackoverflow.com/a/2173421

./script_build.sh
./script_run.sh
