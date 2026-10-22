# BASIC 2026 Mainframe Business Computer

This layer defines a C++23 object-oriented framework for a business computer and a stock-market integration boundary.

## Architecture

```text
BASIC 2026 Application
        |
        v
MainframeSystem
   +----+----------------+
   |                     |
BusinessService     TradingService
   |                     |
Accounts              Orders
Securities            Order status
   |                     |
   +----------+----------+
              |
        External API Adapter
              |
       Market / Broker API
```

The repository implementation intentionally stops at the domain/API boundary. It does not connect to a live brokerage or place real financial orders.

## Domain objects

- `Account` — business/trading account identity and cash balance.
- `Security` — symbol, name, reference price, and quantity metadata.
- `Position` — account security holdings.
- `Order` — side, type, quantity, limit price, and lifecycle status.
- `BusinessService` — account and security registry.
- `TradingService` — order submission and cancellation boundary.
- `MainframeSystem` — top-level system container and operating mode.

## System modes

- `Mainframe`
- `Business`
- `MarketData`
- `Trading`
- `Reporting`

## BASIC 2026 application template

```basic
10 SYSTEM "MAINFRAME"
20 MODE "BUSINESS"
30 ACCOUNT.CREATE "ACCT001", "DEMO", 1000000
40 SECURITY.REGISTER "BASIC", "BASIC SYSTEMS", 12500
50 ORDER.BUY "ACCT001", "BASIC", 100, 12500
60 REPORT.ORDER
70 END
```

These BASIC statements are the target language surface for a future BASIC 2026 adapter. The C++23 classes provide the host runtime underneath them.

## API boundary

A production adapter should map these domain operations to an authenticated external service:

```text
BASIC 2026
    |
    +-- Mainframe API
          |
          +-- Authentication
          +-- Account API
          +-- Security / market-data API
          +-- Order API
          +-- Portfolio API
          +-- Reporting API
          +-- Audit/event stream
```

Use paper/sandbox environments for integration testing. Credentials, signing keys, and live-order endpoints must remain outside the emulator source tree.
