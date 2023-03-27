include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  unittests
  unittests.cpp
)

target_link_libraries(
  unittests
  GTest::gtest_main
  App
)

include(GoogleTest)
gtest_discover_tests(unittests)