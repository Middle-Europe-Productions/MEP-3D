#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
TARGET_BASE_DIR="$SCRIPT_DIR/../build/examples"
TARGET=""
MODE=Debug

while test $# -gt 0; do
    case "$1" in
        -m|--mode)
            shift
            echo "[MEP] Mode avalible on MSVC"
            shift
        ;;
        -t|--target)
            shift
            if test $# -gt 0; then
                TARGET=$1
            fi
            shift
        ;;
        *)
            break
        ;;
    esac
done

if [ -z "$SCRIPT_DIR/$TARGET" ]; then
    echo "[MEP] Missing target!" 
    exit 1
fi

echo "[MEP] Building $TARGET" 

if [ ! -d "$TARGET_BASE_DIR/$TARGET/" ]; then
    echo "[MEP] Target $TARGET_BASE_DIR/$TARGET does not exist!"
    exit
fi

make BUILD=$MODE -f $TARGET_BASE_DIR/$TARGET/Makefile

if [ $? -gt 0 ]; then
   echo "[MEP] Compilation failed!"
   exit 1
fi

echo "[MEP] Launching $TARGET"
(cd $SCRIPT_DIR/../output/$TARGET && ./$TARGET $@)
