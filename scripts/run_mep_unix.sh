#!/bin/bash

SCRIPT_DIR=$(cd -- $(dirname "$0") && pwd)
echo $SCRIPT_DIR
PROJECT_NAME="Build"
TARGET_BASE_DIR="$SCRIPT_DIR/../$PROJECT_NAME/examples"
TARGET=""
MODE=Debug

project_generator() {
    cmake -S $SCRIPT_DIR/.. -B $SCRIPT_DIR/../$PROJECT_NAME
}

while test $# -gt 0; do
    case "$1" in
        -g|--generate)
            project_generator;
            echo "[MEP] Project generated"
            shift
        ;;
        -r|--regenerate)
            project_generator;
            echo "[MEP] Project regenerated"
            shift
        ;;
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

if [ "$TARGET" == "" ]; then
    echo "[MEP] No target specified, exiting."
    exit 0
fi

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
