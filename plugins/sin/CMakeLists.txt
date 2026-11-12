add_library(sin SHARED sin.cpp)
target_include_directories(sin PRIVATE ${CMAKE_SOURCE_DIR}/inc)
target_compile_definitions(sin PRIVATE PLUGIN_BUILD)
set_target_properties(sin PROPERTIES
    PREFIX ""
    OUTPUT_NAME "sin"
    WINDOWS_EXPORT_ALL_SYMBOLS OFF
)