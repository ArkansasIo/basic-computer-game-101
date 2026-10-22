#pragma once

#include "basic2026/core.hpp"

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace basic2026::business {

enum class SystemMode { Mainframe, Business, MarketData, Trading, Reporting };

enum class OrderSide { Buy, Sell };
enum class OrderType { Market, Limit };
enum class OrderStatus { New, Accepted, Rejected, Filled, Cancelled };

struct Account {
    std::string id;
    std::string name;
    std::int64_t cashCents = 0;
};

struct Security {
    std::string symbol;
    std::string name;
    std::int64_t lastPriceCents = 0;
    std::int64_t quantity = 0;
};

struct Position {
    std::string accountId;
    std::string symbol;
    std::int64_t quantity = 0;
    std::int64_t averagePriceCents = 0;
};

struct Order {
    std::uint64_t id = 0;
    std::string accountId;
    std::string symbol;
    OrderSide side = OrderSide::Buy;
    OrderType type = OrderType::Market;
    std::int64_t quantity = 0;
    std::int64_t limitPriceCents = 0;
    OrderStatus status = OrderStatus::New;
};

class BusinessService {
public:
    Account& createAccount(std::string id, std::string name, std::int64_t cashCents);
    Security& registerSecurity(std::string symbol, std::string name, std::int64_t priceCents);
    const Account* account(std::string_view id) const;
    const Security* security(std::string_view symbol) const;
    const std::vector<Order>& orders() const noexcept;

private:
    std::unordered_map<std::string, Account> accounts_;
    std::unordered_map<std::string, Security> securities_;
    std::vector<Order> orders_;
};

class TradingService {
public:
    explicit TradingService(BusinessService& business);
    Order submit(Order order);
    bool cancel(std::uint64_t orderId);
    const std::vector<Order>& orders() const noexcept;

private:
    BusinessService& business_;
    std::uint64_t nextOrderId_ = 1;
    std::vector<Order> orders_;
};

class MainframeSystem {
public:
    MainframeSystem();

    BusinessService& business() noexcept;
    TradingService& trading() noexcept;
    void setMode(SystemMode mode) noexcept;
    SystemMode mode() const noexcept;
    std::string status() const;

private:
    BusinessService business_;
    TradingService trading_;
    SystemMode mode_ = SystemMode::Mainframe;
};

} // namespace basic2026::business
