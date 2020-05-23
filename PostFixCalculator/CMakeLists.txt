add_executable(post_fix_calc
        main.cpp
        PostFixCalculator.cpp)

target_include_directories(post_fix_calc PUBLIC ${Boost_INCLUDE_DIRS})

package_add_test(post_fix_calc_test
        PostFixCalculator.cpp
        PostFixCalculator.t.cpp)
