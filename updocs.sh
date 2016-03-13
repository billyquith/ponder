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
cmake -G Ninja
ninja doc
popd

rm docs/*
cp $DOCDIR/doc/api/html/* docs/

clean

echo 'bundle exec jekyll serve --baseurl ""'
