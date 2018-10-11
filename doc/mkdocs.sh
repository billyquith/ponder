#!/usr/bin/env bash

pushd ../build
cmake --build . --target ponder_docs

case `uname -s` in
CYGWIN*)
	cygstart doc/api/html/index.html
	;;
darwin*)
	open doc/api/html/index.html
	;;
esac

popd
