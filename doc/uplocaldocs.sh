#!/usr/bin/env bash
# Update online documentation

set -e

if [ ! -f ../build/doc/api/html/index.html ]; then
	echo "Please build docs"
	exit 1
fi

DOCDIR=tmp-docs
DOCOUT=../build/doc/api/html

[ -d $DOCDIR ] && rm -rf $DOCDIR

# document what is comitted
git clone https://github.com/billyquith/ponder.git $DOCDIR -b gh-pages --depth=1

# reset and copy Doxygen docs
[ -d $DOCDIR ] && rm -r $DOCDIR/*
cp -r $DOCOUT/* $DOCDIR

pushd $DOCDIR
git add --all --verbose .
git status
if [[ "$1" == "-d" ]]; then exit 0; fi
read -p "Commit message: " MESSAGE
git commit -m "${MESSAGE:-"Update docs"}"
git push
popd

[ -d $DOCDIR ] && rm -rf $DOCDIR
