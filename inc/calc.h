#pragma once
#include "parser.h"
#include "plugins.h"
#include <string>

class Calc {
public:
    explicit Calc(Plugins& p) noexcept : plugins(p) {}

    double eval(const std::string& s) const {
        Parser parser(s);
        NodePtr tree = parser.parse();
        return tree->eval(plugins);
    }

private:
    Plugins& plugins;
};