cmake_minimum_required(VERSION 2.6)

# set project's name
project(CAMP_TEST)

# configure some variables
include(${CMAKE_BINARY_DIR}/config.cmake)

# all source files
set(CAMP_TEST_SRCS
    main.cpp
)

# all header files
set(CAMP_TEST_HDRS
    test.hpp
)

# include files search paths
include_directories(
    ${CAMP_SOURCE_DIR}/include
    ${BOOST_INCLUDE_DIR}
)

# linker search paths
link_directories(
    ${CAMP_BINARY_DIR}
    ${UTF_LIBRARIES}
)

# instruct CMake to build an executable from all of the source files
add_executable(camptest ${CAMP_TEST_SRCS})

# last thing we have to do is to tell CMake what libraries our executable needs,
target_link_libraries(camptest camp utf)

