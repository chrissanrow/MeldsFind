cmake_minimum_required(VERSION 3.10)
project(MeldsFind)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Source files
file(GLOB SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Executable
add_executable(MeldsFind ${SOURCES})

# Link OpenCV
target_include_directories(MeldsFind PRIVATE src ${OpenCV_INCLUDE_DIRS})
target_link_libraries(MeldsFind PRIVATE ${OpenCV_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)