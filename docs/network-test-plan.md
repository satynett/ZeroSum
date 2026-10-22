# LAN test plan (Windows + Mac + phone hotspot)

1. Connect the Windows laptop and Mac to the same phone hotspot. Confirm both receive private IPv4 addresses in the same subnet.
2. On Windows, allow the server executable or TCP port 5555 through **Windows Defender Firewall** for **Private networks** only. Do not disable the firewall globally.
3. Start the host process on the Windows laptop and display its invite code (8 hex digits) and port 5555.
4. On the Mac, enter the invite code; verify it decodes to the host's IPv4 address and that TCP connection succeeds.
5. Repeat with the Mac hosting and Windows joining.
6. Send fragmented messages and multiple newline-delimited messages; verify each parses as a complete line.
7. Send a line longer than 1024 bytes; verify the client is disconnected/rejected without crashing the host.
8. Disconnect a client during a game; verify the host remains running and game disconnect handling is called.
9. Run `ctest --test-dir build --output-on-failure` on both systems.

Note: some phone hotspots isolate connected devices. If both laptops can access the internet but cannot connect to each other, check hotspot client isolation or try a router/hotspot that permits local device-to-device traffic.