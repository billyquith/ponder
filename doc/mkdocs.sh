#!/usr/bin/env bash

pushd ../build
cmake --build . --target ponder_docs
open doc/api/html/index.html
popd
