#include "basic2026/business_trading.hpp"
#include <iostream>

int main() {
    using namespace basic2026::business;
    BusinessTradingApi api;

    const auto customer = api.business().addCustomer("ACME CORP", "ops@example.invalid");
    const auto product = api.business().addProduct("CPU-001", "Mainframe CPU", 100, 499.0);
    std::cout << "BASIC 2026 BUSINESS/MAINFRAME API\n";
    std::cout << "CUSTOMER=" << customer << " PRODUCT=" << product << "\n";

    api.trading().setQuote({"DEMO", 99.90, 100.10, 0});
    TradeOrder order{};
    order.symbol = "DEMO";
    order.side = Side::Buy;
    order.type = OrderType::Limit;
    order.quantity = 100;
    order.limitPrice = 100.00;

    const auto orderId = api.trading().submit(order);
    std::cout << "PAPER_ORDER=" << orderId << " CASH=" << api.trading().cash() << "\n";
    std::cout << "Mode=Paper; live broker adapters intentionally not connected.\n";
    return 0;
}
