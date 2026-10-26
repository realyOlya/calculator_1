#pragma once
#include <string_view>

#ifdef PLUGIN_BUILD
#  define PLUGIN_API __declspec(dllexport)
#else
#  define PLUGIN_API __declspec(dllimport)
#endif

#define PLUGIN_CALL __cdecl

class Plugin {
public:
    virtual ~Plugin() = default;
    virtual std::string_view name() const noexcept = 0;
    virtual double calc(double x) const = 0;
};

using CreateFn = Plugin * (PLUGIN_CALL*)();
using DestroyFn = void (PLUGIN_CALL*)(Plugin*) noexcept;

extern "C" {
    PLUGIN_API Plugin* PLUGIN_CALL make_plugin();
    PLUGIN_API void    PLUGIN_CALL free_plugin(Plugin*) noexcept;
}