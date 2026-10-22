#pragma once

#include "basic2026/business/mainframe.hpp"

#include <string>
#include <string_view>

namespace basic2026::business {

class ApiFacade {
public:
    explicit ApiFacade(MainframeSystem& system) : system_(system) {}

    std::string health() const { return system_.status(); }
    bool hasAccount(std::string_view id) const { return system_.business().account(id) != nullptr; }
    bool hasSecurity(std::string_view symbol) const { return system_.business().security(symbol) != nullptr; }

private:
    MainframeSystem& system_;
};

} // namespace basic2026::business
