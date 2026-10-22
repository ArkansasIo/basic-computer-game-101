#include "basic2026/business_trading.hpp"

#include <stdexcept>
#include <utility>

namespace basic2026::business {

BusinessService::BusinessService() = default;
std::string BusinessService::className() const { return "BUSINESS.SERVICE"; }

Id BusinessService::addCustomer(std::string name, std::string email) {
    const Id id = nextId_++;
    customers_.emplace(id, Customer{id, std::move(name), std::move(email)});
    return id;
}

Id BusinessService::addProduct(std::string sku, std::string name, Integer quantity, double unitPrice) {
    if (quantity < 0 || unitPrice < 0.0) throw std::invalid_argument("invalid product values");
    const Id id = nextId_++;
    products_.emplace(id, Product{id, std::move(sku), std::move(name), quantity, unitPrice});
    return id;
}

Id BusinessService::createOrder(Id customerId, std::vector<OrderLine> lines) {
    if (!customers_.contains(customerId)) throw std::runtime_error("unknown customer");
    double total = 0.0;
    for (const auto& line : lines) {
        if (line.quantity <= 0) throw std::invalid_argument("order quantity must be positive");
        auto it = products_.find(line.productId);
        if (it == products_.end()) throw std::runtime_error("unknown product");
        if (it->second.quantity < line.quantity) throw std::runtime_error("insufficient inventory");
        total += static_cast<double>(line.quantity) * line.price;
    }
    for (const auto& line : lines) products_.at(line.productId).quantity -= line.quantity;
    const Id id = nextId_++;
    orders_.emplace(id, Order{id, customerId, std::move(lines), total});
    return id;
}

const Customer& BusinessService::customer(Id id) const { return customers_.at(id); }
const Product& BusinessService::product(Id id) const { return products_.at(id); }
const Order& BusinessService::order(Id id) const { return orders_.at(id); }

std::vector<Product> BusinessService::inventory() const {
    std::vector<Product> result;
    result.reserve(products_.size());
    for (const auto& [id, product] : products_) result.push_back(product);
    return result;
}

void RiskEngine::setLimits(RiskLimits limits) { limits_ = limits; }

bool RiskEngine::approve(const TradeOrder& order, const Quote& quote, double currentDailyPnL) const {
    if (order.quantity <= 0 || order.quantity > limits_.maxOrderQuantity) return false;
    const double referencePrice = order.type == OrderType::Limit
        ? order.limitPrice
        : (order.side == Side::Buy ? quote.ask : quote.bid);
    if (referencePrice <= 0.0) return false;
    if (static_cast<double>(order.quantity) * referencePrice > limits_.maxOrderValue) return false;
    if (currentDailyPnL <= -limits_.maxDailyLoss) return false;
    return true;
}

const RiskLimits& RiskEngine::limits() const noexcept { return limits_; }

PaperTradingService::PaperTradingService() = default;
std::string PaperTradingService::className() const { return "TRADING.PAPER"; }

void PaperTradingService::setRiskLimits(RiskLimits limits) { risk_.setLimits(limits); }

void PaperTradingService::setQuote(Quote quote) { quotes_[quote.symbol] = std::move(quote); }

Id PaperTradingService::submit(TradeOrder order) {
    const auto quoteIt = quotes_.find(order.symbol);
    if (quoteIt == quotes_.end()) throw std::runtime_error("no quote for symbol");
    if (!risk_.approve(order, quoteIt->second, 0.0)) throw std::runtime_error("order rejected by risk engine");
    order.id = nextOrderId_++;
    const double executionPrice = order.type == OrderType::Limit
        ? order.limitPrice
        : (order.side == Side::Buy ? quoteIt->second.ask : quoteIt->second.bid);
    const double notional = static_cast<double>(order.quantity) * executionPrice;
    if (order.side == Side::Buy && notional > cash_) throw std::runtime_error("insufficient paper cash");
    if (order.side == Side::Buy) cash_ -= notional;
    else cash_ += notional;
    orders_.push_back(order);
    return order.id;
}

void PaperTradingService::cancel(Id orderId) {
    for (auto& order : orders_) {
        if (order.id == orderId) {
            order.quantity = 0;
            return;
        }
    }
}

const std::vector<TradeOrder>& PaperTradingService::orders() const noexcept { return orders_; }
double PaperTradingService::cash() const noexcept { return cash_; }
TradingMode PaperTradingService::mode() const noexcept { return TradingMode::Paper; }

BusinessTradingApi::BusinessTradingApi() = default;
std::string BusinessTradingApi::className() const { return "BUSINESS.TRADING.API"; }
BusinessService& BusinessTradingApi::business() noexcept { return business_; }
PaperTradingService& BusinessTradingApi::trading() noexcept { return trading_; }

Value BusinessTradingApi::call(std::string_view operation, const std::vector<Value>& args) {
    if (operation == "BUSINESS.ADD_CUSTOMER") {
        if (args.size() != 2) throw std::invalid_argument("ADD_CUSTOMER expects name,email");
        return Value(static_cast<Integer>(business_.addCustomer(args[0].asString(), args[1].asString())));
    }
    if (operation == "BUSINESS.ADD_PRODUCT") {
        if (args.size() != 4) throw std::invalid_argument("ADD_PRODUCT expects sku,name,quantity,price");
        return Value(static_cast<Integer>(business_.addProduct(args[0].asString(), args[1].asString(), args[2].asInteger(), args[3].asReal())));
    }
    if (operation == "TRADING.CASH") return Value(trading_.cash());
    throw std::runtime_error("unknown API operation: " + std::string(operation));
}

} // namespace basic2026::business
