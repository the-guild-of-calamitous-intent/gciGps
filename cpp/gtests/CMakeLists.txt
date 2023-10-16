include(FetchContent)

# GTest -----------------
FetchContent_Declare(gtest
  GIT_REPOSITORY "https://github.com/google/googletest"
  GIT_TAG "origin/main"
  SOURCE_DIR "${CMAKE_BINARY_DIR}/_deps/gtest"
)
# set(EXAMPLES OFF CACHE INTERNAL "Dont build examples")
FetchContent_MakeAvailable(gtest)
if(gtest_POPULATED)
    message(STATUS "=> Found gtest")
else()
    message(STATUS "*** Didn't find gtest")
endif()

#----------------------------
enable_testing()

list(APPEND gtests
    main.cpp
)

# run all tests
add_executable(test-all ${gtests})
target_link_libraries(test-all GTest::gtest_main ${PROJECT_NAME})
include(GoogleTest)
gtest_discover_tests(test-all)