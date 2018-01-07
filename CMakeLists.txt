project(LibBlinkStickCpp)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Eigen3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread)

include(GNUInstallDirs)

add_subdirectory(src)