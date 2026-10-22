# BASIC 2026 Mainframe Business + Trading Framework

This layer turns BASIC 2026 into an application framework for a mainframe-style business computer. It provides C++23 object-oriented services that can later be exposed to BASIC, a REST API, a terminal API, or an external database.

## Architecture

```text
                    BASIC 2026 APPLICATIONS
                              |
                 +------------+------------+
                 |                         |
          Business API              Trading API
                 |                         |
        +--------+--------+        +-------+-------+
        |                 |        |               |
    Customers         Inventory  Quotes        Risk Engine
        |                 |        |               |
      Orders        Products     Orders      Paper Execution
        |                 |        |               |
        +-----------------+--------+---------------+
                              |
                     Mainframe Service Core
                              |
             +----------------+----------------+
             |                |                |
          Terminal         REST/API        Database
          Adapter          Adapter         Adapter
```

The implementation follows the existing BASIC 2026 C++23 object model and keeps business services separate from transport adapters. The existing runtime already provides `Object`, `ClassRegistry`, `BasicModule`, `Value`, and `Environment`. The new services are `BusinessService`, `PaperTradingService`, `RiskEngine`, and `BusinessTradingApi`.

## Business API

Business operations include:

- customers
- products
- inventory quantities
- orders
- order lines
- order totals

Example C++:

```cpp
basic2026::business::BusinessTradingApi api;
auto customer = api.business().addCustomer("ACME", "ops@example.invalid");
auto product = api.business().addProduct("CPU-001", "Mainframe CPU", 100, 499.0);
```

## Trading API

The initial trading service is deliberately **paper trading**. It provides a deterministic application boundary for later broker/exchange adapters without connecting to a live market.

Supported concepts:

- symbols and quotes
- bid/ask prices
- market and limit orders
- buy/sell sides
- order IDs
- paper cash
- order cancellation
- risk limits

Default risk controls include:

- maximum order notional: 100,000
- maximum order quantity: 10,000
- maximum daily loss: 10,000

These are software defaults for the simulator, not investment advice or production regulatory controls.

## BASIC API surface

The `BusinessTradingApi::call()` dispatcher is designed to become the BASIC 2026 foreign-function/API boundary.

Planned BASIC syntax:

```basic
API BUSINESS.ADD_CUSTOMER, "ACME", "ops@example.invalid"
API BUSINESS.ADD_PRODUCT, "CPU-001", "Mainframe CPU", 100, 499
API TRADING.QUOTE, "DEMO", 99.90, 100.10
API TRADING.BUY, "DEMO", 100, 100.00
PRINT API TRADING.CASH
```

The current dispatcher implements the first foundation operations; the structured BASIC frontend can expose the remaining service methods as native functions as it is extended.

## Mainframe deployment model

```text
BASIC Program
     |
     v
BASIC 2026 Runtime
     |
     v
BusinessTradingApi
     |
     +-- BusinessService
     +-- RiskEngine
     +-- PaperTradingService
     |
     +-- REST adapter
     +-- Terminal adapter
     +-- Database repository
     +-- Message/event adapter
```

A production financial system should keep authentication, authorization, audit logging, data validation, order controls, market-data validation, and regulatory/compliance services outside the BASIC language itself. FINRA's API platform uses OAuth 2.0 and documents throttling and security requirements, while its algorithmic-trading guidance emphasizes risk assessment, software testing, trading-system controls, and compliance. See the official FINRA developer documentation before connecting a real regulated service. urlFINRA Developer Centerhttps://developer.finra.org/

## Next implementation layer

1. BASIC 2026 `API` statement and function calls.
2. Typed records and arrays for business datasets.
3. Repository interfaces for SQLite/MySQL.
4. REST/HTTP transport adapter.
5. Authentication and role-based authorization.
6. Audit/event journal.
7. Market-data adapter interface.
8. Broker/exchange adapter interface kept separate from the simulator.
9. Pre-trade risk and account-position services.
10. Mainframe terminal dashboards for accounts, inventory, quotes, and orders.
11. Batch jobs and scheduled business processing.
12. 8-bit/CP/M terminal compatibility layer.
