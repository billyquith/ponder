#!/usr/bin/env bash
# Update documentation

DOCDIR=tmpdocs

function clean () {
    if [ -d $DOCDIR ]; then
        rm -rf $DOCDIR
    fi
}

clean

git clone https://github.com/billyquith/ponder.git $DOCDIR -b master --depth=1

pushd $DOCDIR
mkdir build && cd build
    cmake -G Ninja ..
cd ..

python doc/api/docinsert.py 
if [ $? -ne 0 ]; then exit 1; fi
    
cd build
    ninja doc
cd ..
popd

rm docs/*
cp -r $DOCDIR/doc/api/html/* docs/

clean

