add_library(ln SHARED ln.cpp)
target_include_directories(ln PRIVATE ${CMAKE_SOURCE_DIR}/inc)
target_compile_definitions(ln PRIVATE PLUGIN_BUILD)
set_target_properties(ln PROPERTIES
    PREFIX ""
    OUTPUT_NAME "ln"
    WINDOWS_EXPORT_ALL_SYMBOLS OFF
)