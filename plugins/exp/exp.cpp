#include "plugin.h"
#include <cmath>

class Exp : public Plugin {
public:
    std::string_view name() const noexcept override { return "exp"; }
    double calc(double x) const override {
        return std::exp(x);
    }
};

extern "C" {
    Plugin* make_plugin() { return new Exp(); }
    void free_plugin(Plugin* p) noexcept { delete p; }
}