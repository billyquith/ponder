cmake_minimum_required(VERSION 2.6)

# set project's name
project(CAMP_TEST)

# all source files
set(CAMP_TEST_SRCS
    main.cpp
)

# all header files
set(CAMP_TEST_HDRS
    test.hpp
)

# find Boost
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.38.0 COMPONENTS unit_test_framework REQUIRED)

# include files search paths
include_directories(
    ${CAMP_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
)

# linker search paths
link_directories(
    ${CAMP_BINARY_DIR}
)

# instruct CMake to build an executable from all of the source files
add_executable(camptest ${CAMP_TEST_SRCS})

# last thing we have to do is to tell CMake what libraries our executable needs,
target_link_libraries(camptest camp ${Boost_LIBRARIES})

