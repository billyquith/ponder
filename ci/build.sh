#!/usr/bin/env bash

case `uname -s` in
Darwin)
    if [ ! -z `which ninja` ]; then
        GEN=Ninja;
    else
        GEN=Xcode;
        BUILDARGS="-- -quiet"; # otherwise xcodebuild very verbose
    fi
    ;;
Linux)
    #if [ ! -z `which ninja` ]; then GEN=Ninja; else GEN="Unix Makefiles"; fi
    GEN="Unix Makefiles"
    ;;
esac

cmake --version

function proj # opts
{
    [[ -d $BDIR ]] || mkdir $BDIR
    cd $BDIR
    if ! cmake .. -G"$GEN" "$1"; then exit 1; fi
}

function build_test # configs
{
    for cfg in $1
    do
        echo "Build and testing config: $cfg"
        if ! cmake --build . --config $cfg $BUILDARGS; then exit 1; fi
        if ! ctest -C $cfg -V; then exit 1; fi
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

run static_lua "Debug Release" "-DBUILD_SHARED_LIBS=OFF -DBUILD_TEST_LUA=ON"
run shared_lua "Debug Release" "-DBUILD_SHARED_LIBS=ON -DBUILD_TEST_LUA=ON"
