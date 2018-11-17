#!/usr/bin/env bash

pushd ../build
cmake --build . --target ponder_docs

case `uname -s` in
CYGWIN*)
	cygstart doc/api/html/index.html
	;;
Darwin*)
	open doc/api/html/index.html
	;;
esac

popd
