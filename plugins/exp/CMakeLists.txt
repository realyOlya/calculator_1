add_library(exp SHARED exp.cpp)
target_include_directories(exp PRIVATE ${CMAKE_SOURCE_DIR}/inc)
target_compile_definitions(exp PRIVATE PLUGIN_BUILD)
set_target_properties(exp PROPERTIES
    PREFIX ""
    OUTPUT_NAME "exp"
    WINDOWS_EXPORT_ALL_SYMBOLS OFF
)