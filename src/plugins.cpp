#include "plugins.h"
#include <stdexcept>

#ifndef NOMINMAX
#  define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#  define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

namespace {
    HMODULE open_lib(const std::filesystem::path& p) noexcept {
        return ::LoadLibraryW(p.wstring().c_str());
    }

    void close_lib(void* h) noexcept {
        if (h) ::FreeLibrary(static_cast<HMODULE>(h));
    }

    void* get_sym(void* h, const char* name) noexcept {
        return reinterpret_cast<void*>(::GetProcAddress(static_cast<HMODULE>(h), name));
    }

    bool is_dll(const std::filesystem::path& p) {
        return p.extension() == L".dll";
    }
}

Plugins::~Plugins() {
    m_map.clear();
    for (void* lib : m_libs) close_lib(lib);
}

bool Plugins::load_one(const std::filesystem::path& path) {
    HMODULE lib = open_lib(path);
    if (!lib) return false;

    const CreateFn make = reinterpret_cast<CreateFn>(get_sym(lib, "make_plugin"));
    const DestroyFn free = reinterpret_cast<DestroyFn>(get_sym(lib, "free_plugin"));

    if (!make || !free) {
        close_lib(lib);
        return false;
    }

    Plugin* raw = nullptr;
    try {
        raw = make();
    }
    catch (...) {
        close_lib(lib);
        return false;
    }
    if (!raw) {
        close_lib(lib);
        return false;
    }

    Entry e;
    e.ptr = Ptr(raw, free);
    e.lib = lib;

    const std::string fname(e.ptr->name());
    if (fname.empty()) {
        close_lib(lib);
        return false;
    }

    m_map.emplace(fname, std::move(e));
    m_libs.push_back(lib);
    return true;
}

std::size_t Plugins::load(const std::filesystem::path& dir) {
    std::error_code ec;
    if (!std::filesystem::exists(dir, ec) || !std::filesystem::is_directory(dir, ec))
        return 0;

    std::size_t n = 0;
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(dir, ec)) {
        if (ec) break;
        if (!entry.is_regular_file(ec)) continue;
        if (!is_dll(entry.path())) continue;

        try {
            if (load_one(entry.path())) ++n;
        }
        catch (...) {}
    }
    return n;
}

bool Plugins::has(std::string_view name) const noexcept {
    return m_map.find(name) != m_map.end();
}

std::optional<double> Plugins::call(std::string_view name, double x) const {
    std::map<std::string, Entry, std::less<>>::const_iterator it = m_map.find(name);
    if (it == m_map.end()) return std::nullopt;

    try {
        return it->second.ptr->calc(x);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("plugin '" + std::string(name) + "': " + e.what());
    }
    catch (...) {
        throw std::runtime_error("plugin '" + std::string(name) + "': unknown error");
    }
}