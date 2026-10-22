#pragma once

#include "basic2026/core.hpp"
#include "basic2026/oop.hpp"

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace basic2026::business {

using Id = std::uint64_t;

struct Customer {
    Id id{};
    std::string name;
    std::string email;
};

struct Product {
    Id id{};
    std::string sku;
    std::string name;
    Integer quantity{};
    double unitPrice{};
};

struct OrderLine {
    Id productId{};
    Integer quantity{};
    double price{};
};

struct Order {
    Id id{};
    Id customerId{};
    std::vector<OrderLine> lines;
    double total{};
};

class BusinessService : public Object {
public:
    BusinessService();
    std::string className() const override;

    Id addCustomer(std::string name, std::string email);
    Id addProduct(std::string sku, std::string name, Integer quantity, double unitPrice);
    Id createOrder(Id customerId, std::vector<OrderLine> lines);
    const Customer& customer(Id id) const;
    const Product& product(Id id) const;
    const Order& order(Id id) const;
    std::vector<Product> inventory() const;

private:
    Id nextId_ = 1;
    std::unordered_map<Id, Customer> customers_;
    std::unordered_map<Id, Product> products_;
    std::unordered_map<Id, Order> orders_;
};

enum class Side { Buy, Sell };
enum class OrderType { Market, Limit };

enum class TradingMode { Paper, Live };

struct Quote {
    std::string symbol;
    double bid{};
    double ask{};
    std::uint64_t timestamp{};
};

struct TradeOrder {
    Id id{};
    std::string symbol;
    Side side{Side::Buy};
    OrderType type{OrderType::Market};
    Integer quantity{};
    double limitPrice{};
};

struct RiskLimits {
    double maxOrderValue = 100000.0;
    Integer maxOrderQuantity = 10000;
    double maxDailyLoss = 10000.0;
};

class RiskEngine {
public:
    void setLimits(RiskLimits limits);
    bool approve(const TradeOrder& order, const Quote& quote, double currentDailyPnL) const;
    const RiskLimits& limits() const noexcept;

private:
    RiskLimits limits_{};
};

class PaperTradingService : public Object {
public:
    PaperTradingService();
    std::string className() const override;
    void setRiskLimits(RiskLimits limits);
    void setQuote(Quote quote);
    Id submit(TradeOrder order);
    void cancel(Id orderId);
    const std::vector<TradeOrder>& orders() const noexcept;
    double cash() const noexcept;
    TradingMode mode() const noexcept;

private:
    Id nextOrderId_ = 1;
    double cash_ = 1000000.0;
    RiskEngine risk_;
    std::unordered_map<std::string, Quote> quotes_;
    std::vector<TradeOrder> orders_;
};

class BusinessTradingApi : public Object {
public:
    BusinessTradingApi();
    std::string className() const override;

    BusinessService& business() noexcept;
    PaperTradingService& trading() noexcept;

    Value call(std::string_view operation, const std::vector<Value>& args);

private:
    BusinessService business_;
    PaperTradingService trading_;
};

} // namespace basic2026::business
