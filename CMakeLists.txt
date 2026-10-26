cmake_minimum_required(VERSION 3.20)
project(calculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/W4 /permissive- /EHsc /utf-8)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS NOMINMAX WIN32_LEAN_AND_MEAN)
endif()

add_library(core STATIC
    src/plugins.cpp
    src/parser.cpp
)
target_include_directories(core PUBLIC inc)

add_executable(calc src/main.cpp)
target_link_libraries(calc PRIVATE core)
set_target_properties(calc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>
)

set(PLUGIN_OUT ${CMAKE_BINARY_DIR}/$<CONFIG>/plugins)
add_subdirectory(plugins/sin)
add_subdirectory(plugins/ln)
add_subdirectory(plugins/exp)

add_custom_target(copy_plugins ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PLUGIN_OUT}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sin> $<TARGET_FILE:ln> $<TARGET_FILE:exp>
        ${PLUGIN_OUT}
    DEPENDS sin ln exp
)
add_dependencies(calc copy_plugins)