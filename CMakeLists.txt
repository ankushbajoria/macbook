cmake_minimum_required(VERSION 3.16)

project(code)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.66.0 REQUIRED COMPONENTS program_options)

enable_testing()
include(GoogleTest)

add_subdirectory(gtest-1.7.0)

macro(package_add_test TESTNAME)
    # create an exectuable in which the tests will be stored
    add_executable(${TESTNAME} ${ARGN})
    # link the Google test infrastructure, mocking library, and a default main fuction to
    # the test executable.  Remove g_test_main if writing your own main function.
    target_include_directories(${TESTNAME} PUBLIC ${gtest_SOURCE_DIR}/include)
    target_link_libraries(${TESTNAME} gtest gtest_main)
    # gtest_discover_tests replaces gtest_add_tests,
    # see https://cmake.org/cmake/help/v3.10/module/GoogleTest.html for more options to pass to it
    gtest_discover_tests(${TESTNAME}
            # set a working directory so your project root so that you can find test data via paths relative to the project root
            WORKING_DIRECTORY ${PROJECT_DIR}
            PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
            )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/PostFixCalc)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/OrderBook)
