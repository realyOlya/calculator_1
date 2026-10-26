#include "calc.h"
#include "plugins.h"
#include <filesystem>
#include <iostream>
#include <string>

int main() {
    const std::filesystem::path dir = std::filesystem::current_path() / "plugins";

    Plugins plugins;
    const std::size_t n = plugins.load(dir);

    if (n == 0)
        std::cerr << "[warning] no plugins in " << dir << '\n';
    else
        std::cout << "[info] loaded " << n << " plugin(s)\n";

    Calc calc(plugins);
    std::string line;

    std::cout << "enter expression (or 'exit'):\n";
    while (std::getline(std::cin, line)) {
        if (line == "exit" || line == "quit") break;
        if (line.empty()) continue;
        try {
            std::cout << "= " << calc.eval(line) << '\n';
        }
        catch (const std::exception& e) {
            std::cerr << "[error] " << e.what() << '\n';
        }
    }
    return 0;
}