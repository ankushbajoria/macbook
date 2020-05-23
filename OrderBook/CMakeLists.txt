add_executable(feed_handler
        main.cpp
        FeedHandler.cpp
#        Book.cpp
        RawMsg.cpp QuoteHandler.h)

message(STATUS ${Boost_PROGRAM_OPTIONS})
target_include_directories(feed_handler PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(feed_handler LINK_PUBLIC ${Boost_LIBRARIES} )

package_add_test(feed_handler_tests
        RawMsg.cpp
        RawMsg.t.cpp)