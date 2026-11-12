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
    Plugin* make_plugin() { return new Sin(); }
    void free_plugin(Plugin* p) noexcept { delete p; }
}