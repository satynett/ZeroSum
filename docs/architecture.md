# Architecture and contracts

## Module boundaries
- `Game` receives parsed `Message` values and returns `Outbox` values. It never owns sockets or network threads.
- `Protocol` maps one newline-delimited wire message to/from `Message`.
- The server is responsible for client IDs, delivery, lobby state, and calling `Game::update()`.
- Local runners call the same game interface directly, so game rules can be tested without TCP.

## Wire format
Messages are `TYPE|field1|field2\n`. Lines over 1 KiB and unknown message types are rejected. Fields must not contain a raw newline.

## Suggested team split
1. Math module and generator tests.
2. Poker evaluator and game rules tests.
3. TCP/lobby, invite-code helpers, and cross-platform testing.

## Build checks
Run configure, build, and CTest on macOS Clang and Windows MSVC or MinGW. Test networking on one shared Wi-Fi/hotspot; permit TCP port 5555 through Windows Firewall on private networks only.