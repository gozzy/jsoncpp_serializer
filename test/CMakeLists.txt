project(SerializingTest)

find_package (Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(serializing_test test.cpp)
include_directories(/usr/include/jsoncpp ../inc)
target_link_libraries(serializing_test ${Boost_LIBRARIES} -ljsoncpp)
add_definitions(-std=c++11 -DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN)

add_test(NAME SerializingTest COMMAND serializing_test)