#include "plugin.h"
#include <cmath>
#include <stdexcept>

class Ln : public Plugin {
public:
    std::string_view name() const noexcept override { return "ln"; }
    double calc(double x) const override {
        if (x <= 0.0) throw std::runtime_error("ln: x > 0");
        return std::log(x);
    }
};

extern "C" {
    Plugin* make_plugin() { return new Ln(); }
    void free_plugin(Plugin* p) noexcept { delete p; }
}