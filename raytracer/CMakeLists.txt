cmake_minimum_required(VERSION 4.0)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

project(raytracer)
add_executable(raytracer MACOSX_BUNDLE main.cpp
        vector/Vector3.h
        camera/Camera.h
        vector/Vector2.h
        sphere/Sphere.h
        ray/Ray.h
        scene/Scene.h
        Plane.h
        Entity.h
        Color.h)
target_link_libraries(raytracer PRIVATE SDL3::SDL3)

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
        sphere/Sphere.test.cpp
        scene/Scene.test.cpp
        camera/Camera.h
        vector/Vector2.h
        sphere/Sphere.h
        ray/Ray.h
        scene/Scene.h
        Plane.h
        Entity.h
        Color.h)
target_link_libraries(
        all_test
        GTest::gtest_main
)
gtest_discover_tests(all_test)