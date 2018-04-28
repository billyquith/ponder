###############################################################################
##
## This file is part of the Ponder library.
##
## The MIT License (MIT)
##
## Copyright (C) 2015-2016-17 Nick Trout.
##
## Permission is hereby granted, free of charge, to any person obtaining a copy
## of this software and associated documentation files (the "Software"), to deal
## in the Software without restriction, including without limitation the rights
## to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
## copies of the Software, and to permit persons to whom the Software is
## furnished to do so, subject to the following conditions:
##
## The above copyright notice and this permission notice shall be included in
## all copies or substantial portions of the Software.
##
## THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
## IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
## FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
## AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
## LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
## OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
## THE SOFTWARE.
##
###############################################################################

# set project's name
project(luadev)

find_package(Lua 5.3 REQUIRED)

# all source files
set(TEST_SRCS
    main.cpp
)

include_directories(
    ${PONDER_SOURCE_DIR}/include
    ${LUA_INCLUDE_DIR}
)

link_directories(
    ${PONDER_BINARY_DIR}
)

add_executable(luadev ${TEST_SRCS})

target_compile_features(luadev PUBLIC cxx_range_for cxx_variadic_templates) # required

target_compile_definitions(luadev PUBLIC PONDER_USING_LUA=1)

target_link_libraries(luadev ponder ${LUA_LIBRARIES})


