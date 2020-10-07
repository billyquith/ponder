
project(lua C)

cmake_minimum_required(VERSION 3.4)

include_directories(src ${CMAKE_CURRENT_BINARY_DIR})

set(SRC_LUA
	src/lapi.c
	src/lcode.c
	src/lctype.c
	src/ldebug.c
	src/ldo.c
	src/ldump.c
	src/lfunc.c
	src/lgc.c
	src/llex.c
	src/lmem.c
	src/lobject.c
	src/lopcodes.c
	src/lparser.c
	src/lstate.c
	src/lstring.c
	src/ltable.c
	src/ltm.c
	src/lundump.c
	src/lvm.c
	src/lzio.c)

set(SRC_LIB
	src/lauxlib.c
	src/lbaselib.c
	src/lbitlib.c
	src/lcorolib.c
	src/ldblib.c
	src/liolib.c
	src/lmathlib.c
	src/loslib.c
	src/lstrlib.c
	src/ltablib.c
	src/lutf8lib.c
	src/loadlib.c
	src/linit.c)

set(SRC_LUACL src/lua.c)

set(SRC_LUAC src/luac.c)

add_library(liblua ${SRC_LUA} ${SRC_LIB})
set_target_properties(liblua PROPERTIES OUTPUT_NAME lua)

if(UNIX)
    set(LINK_LIBS m)
endif()

# add_library(libluadll SHARED ${SRC_LUA} ${SRC_LIB})
# target_compile_definitions(libluadll PRIVATE _CRT_SECURE_NO_WARNINGS LUA_BUILD_AS_DLL)
# set_target_properties(libluadll PROPERTIES OUTPUT_NAME lua53)

add_executable(lua ${SRC_LUACL})
target_link_libraries(lua liblua ${LINK_LIBS})

add_executable(luac ${SRC_LUAC})
target_link_libraries(luac liblua ${LINK_LIBS})
