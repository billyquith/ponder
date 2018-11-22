#!/usr/bin/env bash
# Update online documentation

set -e

PROJDIR=tmp-proj
DOCDIR=tmp-docs
DOCOUT=$PROJDIR/build/doc/api/html

[ -d $PROJDIR ] && rm -rf $PROJDIR
[ -d $DOCDIR ] && rm -rf $DOCDIR

# document what is comitted
git clone https://github.com/billyquith/ponder.git $PROJDIR -b master --depth=1
git clone https://github.com/billyquith/ponder.git $DOCDIR -b gh-pages --depth=1

pushd $PROJDIR
mkdir build && cd build

# OS specifc project creation
case $(uname -s) in
CYGWIN*)
	if [ ! -z "$(ninja --version)" ]; then
		echo "Use Ninja"
		cmake .. -G Ninja
	else
		"Use make"
		cmake .. -G "Unix Makefiles"
	fi
	;;
*)	cmake .. -G Ninja
	;;
esac

cmake --build . --target ponder_docs
cd ..
popd

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

[ -d $PROJDIR ] && rm -rf $PROJDIR
[ -d $DOCDIR ] && rm -rf $DOCDIR
