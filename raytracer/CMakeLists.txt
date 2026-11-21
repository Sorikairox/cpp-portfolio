cmake_minimum_required(VERSION 4.0)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(raytracer)
add_executable(raytracer MACOSX_BUNDLE main.cpp
        vector/Vector3.h
        camera/Camera.h
        vector/Vector2.h)

project(vector3_test)
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)
add_executable(all_test vector/Vector3.test.cpp
        camera/Camera.test.cpp
        camera/Camera.h
        vector/Vector2.h)
target_link_libraries(
        all_test
        GTest::gtest_main
)
gtest_discover_tests(all_test)