cmake_minimum_required(VERSION 3.16.0)
project(gscunit VERSION 1.0.0 LANGUAGES CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(CMakeRC.cmake)

# CPack
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR 7Z)
include(CPack)

# Conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# Tests
include(CTest)
add_executable(gscunit.Tests)
target_link_libraries(gscunit.Tests PUBLIC ${CONAN_LIBS})
add_test(NAME Tests COMMAND gscunit.Tests)

# Build
add_executable(gscunit)
add_subdirectory(runtime)
add_subdirectory(src)
target_include_directories(gscunit PUBLIC ..)
target_link_directories(gscunit PUBLIC ..)
target_link_libraries(gscunit PUBLIC Resources.Runtime)

# Pack
install(TARGETS gscunit LIBRARY DESTINATION lib)
install(DIRECTORY src/ DESTINATION lib/include/gscunit
    FILES_MATCHING PATTERN "*.hpp" PATTERN "__test__" EXCLUDE)
