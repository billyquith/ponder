#!/usr/bin/env bash

case `uname -s` in
Darwin) if [ ! -z `which ninja` ]; then GEN=Ninja; else GEN=Xcode; fi;;
Linux) if [ ! -z `which ninja` ]; then GEN=Ninja; else GEN="Unix Makefiles"; fi;;
esac

cmake --version

function proj # opts
{
    [[ -d $BDIR ]] || mkdir $BDIR
    cd $BDIR
    cmake .. -G$GEN "$1"
}

function build_test # configs
{
    for cfg in $1
    do
        echo "Build and testing config: $cfg"
        cmake --build . --config $cfg
        ctest -C $cfg -V
    done
    cd ..
}

function run # dirname, configs, opts
{
    BDIR=bd_$1
    proj "$3"
    build_test "$2"
}

run static "Debug Release" "-DBUILD_SHARED_LIBS=OFF"
run shared "Debug Release" "-DBUILD_SHARED_LIBS=ON"

run static "Debug Release" "-DBUILD_SHARED_LIBS=OFF -DBUILD_TEST_LUA=ON"
run shared "Debug Release" "-DBUILD_SHARED_LIBS=ON -DBUILD_TEST_LUA=ON"
