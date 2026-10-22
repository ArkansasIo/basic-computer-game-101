#include "basic2026/business/mainframe.hpp"

#include <algorithm>
#include <sstream>

namespace basic2026::business {

Account& BusinessService::createAccount(std::string id, std::string name, std::int64_t cashCents) {
    auto [it, inserted] = accounts_.emplace(id, Account{std::move(id), std::move(name), cashCents});
    if (!inserted) {
        it->second.name = std::move(name);
        it->second.cashCents = cashCents;
    }
    return it->second;
}

Security& BusinessService::registerSecurity(std::string symbol, std::string name, std::int64_t priceCents) {
    auto [it, inserted] = securities_.emplace(symbol, Security{std::move(symbol), std::move(name), priceCents, 0});
    if (!inserted) {
        it->second.name = std::move(name);
        it->second.lastPriceCents = priceCents;
    }
    return it->second;
}

const Account* BusinessService::account(std::string_view id) const {
    auto it = accounts_.find(std::string(id));
    return it == accounts_.end() ? nullptr : &it->second;
}

const Security* BusinessService::security(std::string_view symbol) const {
    auto it = securities_.find(std::string(symbol));
    return it == securities_.end() ? nullptr : &it->second;
}

const std::vector<Order>& BusinessService::orders() const noexcept { return orders_; }

TradingService::TradingService(BusinessService& business) : business_(business) {}

Order TradingService::submit(Order order) {
    order.id = nextOrderId_++;
    const auto* account = business_.account(order.accountId);
    const auto* security = business_.security(order.symbol);
    if (account == nullptr || security == nullptr || order.quantity <= 0) {
        order.status = OrderStatus::Rejected;
    } else {
        order.status = OrderStatus::Accepted;
    }
    orders_.push_back(order);
    return order;
}

bool TradingService::cancel(std::uint64_t orderId) {
    auto it = std::find_if(orders_.begin(), orders_.end(), [orderId](const Order& order) {
        return order.id == orderId;
    });
    if (it == orders_.end() || it->status == OrderStatus::Filled || it->status == OrderStatus::Cancelled) {
        return false;
    }
    it->status = OrderStatus::Cancelled;
    return true;
}

const std::vector<Order>& TradingService::orders() const noexcept { return orders_; }

MainframeSystem::MainframeSystem() : trading_(business_) {}
BusinessService& MainframeSystem::business() noexcept { return business_; }
TradingService& MainframeSystem::trading() noexcept { return trading_; }
void MainframeSystem::setMode(SystemMode mode) noexcept { mode_ = mode; }
SystemMode MainframeSystem::mode() const noexcept { return mode_; }

std::string MainframeSystem::status() const {
    std::ostringstream out;
    out << "BASIC 2026 MAINFRAME | orders=" << trading_.orders().size();
    return out.str();
}

} // namespace basic2026::business
