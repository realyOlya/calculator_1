#include "plugin.h"
#include <cmath>

constexpr double PI = 3.14159265358979323846;

class Sin : public Plugin {
public:
    std::string_view name() const noexcept override { return "sin"; }
    double calc(double x) const override {
        return std::sin(x * PI / 180.0);
    }
};

extern "C" {
    PLUGIN_API Plugin* PLUGIN_CALL make_plugin() { return new Sin(); }
    PLUGIN_API void    PLUGIN_CALL free_plugin(Plugin* p) noexcept { delete p; }
}