#pragma once
#include "plugin.h"
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

class Plugins {
public:
    Plugins() = default;
    ~Plugins();

    Plugins(const Plugins&) = delete;
    Plugins& operator=(const Plugins&) = delete;

    std::size_t load(const std::filesystem::path& dir);
    std::optional<double> call(std::string_view name, double x) const;
    bool has(std::string_view name) const noexcept;

private:
    using Ptr = std::unique_ptr<Plugin, DestroyFn>;

    struct Entry {
        Ptr   ptr{ nullptr, [](Plugin*) noexcept {} };
        void* lib = nullptr;
    };

    std::map<std::string, Entry, std::less<>> m_map;
    std::vector<void*>                        m_libs;

    bool load_one(const std::filesystem::path& path);
};