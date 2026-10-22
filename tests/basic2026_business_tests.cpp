#include "basic2026/business/mainframe.hpp"

#include <cassert>

int main() {
    using namespace basic2026::business;

    MainframeSystem system;
    system.business().createAccount("A1", "Test", 500000);
    system.business().registerSecurity("ABC", "Example Corp", 10000);

    Order order;
    order.accountId = "A1";
    order.symbol = "ABC";
    order.quantity = 10;
    order.type = OrderType::Limit;
    order.limitPriceCents = 10000;

    const auto result = system.trading().submit(order);
    assert(result.id == 1);
    assert(result.status == OrderStatus::Accepted);
    assert(system.trading().cancel(result.id));
    assert(system.trading().orders().front().status == OrderStatus::Cancelled);

    return 0;
}
