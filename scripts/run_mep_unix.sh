#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

if [ -z "$SCRIPT_DIR/$1" ]; then
    echo "[MEP] Missing target!" 
    exit 1
fi

echo "[MEP] Building $1" 

if [ ! -d "$SCRIPT_DIR/$1/" ]; then
    echo "[MEP] Target $SCRIPT_DIR/$1 does not exist!"
    exit
fi

make -f $SCRIPT_DIR/$1/Makefile

if [ $? -gt 0 ]; then
   echo "[MEP] Compilation failed!"
   exit 1
fi

echo "[MEP] Launching $1"
file=$1
shift
(cd $SCRIPT_DIR/$file/ && ./$file $@)
