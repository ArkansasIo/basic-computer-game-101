#include "basic2026/business/mainframe.hpp"

#include <iostream>

int main() {
    using namespace basic2026::business;

    MainframeSystem system;
    system.setMode(SystemMode::Business);

    system.business().createAccount("ACCT001", "Demo Trading Account", 1'000'000);
    system.business().registerSecurity("BASIC", "BASIC Systems Inc.", 12500);

    Order order;
    order.accountId = "ACCT001";
    order.symbol = "BASIC";
    order.side = OrderSide::Buy;
    order.type = OrderType::Limit;
    order.quantity = 100;
    order.limitPriceCents = 12500;

    const Order accepted = system.trading().submit(order);

    std::cout << system.status() << '\n';
    std::cout << "ORDER " << accepted.id << " STATUS="
              << (accepted.status == OrderStatus::Accepted ? "ACCEPTED" : "REJECTED")
              << '\n';

    return accepted.status == OrderStatus::Accepted ? 0 : 1;
}
