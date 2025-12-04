#!/bin/bash -ex

readonly BASE_DIR=$(cd $(dirname $0); pwd)
readonly BASENAME="$(basename $0)"
readonly BUILD_DIR=$BASE_DIR/build

function help(){
    echo "$BASENAME  [option]"
    echo "    -c    : clean and exit"
    echo "    -f    : format and exit"
    echo "    -h    : show this message"

    exit $1
}

function build() {
    if [ ! -d $BUILD_DIR ]; then
        mkdir $BUILD_DIR
    fi

    cd $BUILD_DIR

    cmake ..
    make -j
    #make -C build test ARGS="-V"
    $BUILD_DIR/example_libs_test

    cd - > /dev/null
}

function clean() {
    rm -rf $BUILD_DIR

    exit
}

function format() {
    if [ ! -d $BUILD_DIR ]; then
        mkdir $BUILD_DIR
    fi

    cd $BUILD_DIR

    cmake ..
    make format

    cd - > /dev/null

    exit
}

CLEAN=false
FORMAT=false
BUILD=true

while getopts ":cf" flag; do
    case $flag in 
    c) CLEAN=true;;
    f) FORMAT=true ;; 
    h) help 0 ;; 
    \?) help 1 ;; 
    esac
done

$CLEAN && clean
$FORMAT && format
$BUILD && build
