#!/usr/bin/env bash
# Run from project root

cd deps

# curl -R -O http://www.lua.org/ftp/lua-5.3.4.tar.gz
# tar zxf lua-5.3.4.tar.gz
cd lua-5.3.4

case `uname -s` in
Darwin) make macosx install INSTALL_TOP=../..;;
Linux) make linux install INSTALL_TOP=../..;;
*) echo "Unhandled OS"; exit 1;;
esac
