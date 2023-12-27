cmake_minimum_required(VERSION 3.20)
project(test_cstructures VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED 23)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic -pedantic-errors -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -fanalyzer")

add_executable(${PROJECT_NAME} ${PROJECT_NAME}.c)
add_subdirectory(src)
target_include_directories(${PROJECT_NAME} PUBLIC
                          "${PROJECT_SOURCE_DIR}/include"
                          "${PROJECT_SOURCE_DIR}/include/screen"
                          "${PROJECT_SOURCE_DIR}/include/test"
                          )

target_link_libraries(${PROJECT_NAME}
                        PUBLIC structuresCommon
                        PUBLIC cstructures
                        PUBLIC screen
                        PUBLIC datasetTest)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
