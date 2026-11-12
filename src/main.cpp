#include "calc.h"
#include "plugins.h"
#include <filesystem>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    try {
        std::filesystem::path exe_path = std::filesystem::absolute(argv[0]);
        std::filesystem::path dir = exe_path.parent_path() / "plugins";

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
    }
    catch (const std::exception& e) {
        std::cerr << "[fatal] " << e.what() << '\n';
        return 1;
    }
    return 0;
}