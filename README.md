# Hotspot GUI (prototype)

This repository contains an initial skeleton for a Linux-first Hotspot GUI written in C++ and Qt (QML frontend).

Goals:
- Simple setup UI with advanced options available
- Linux-first implementation (NetworkManager + hostapd fallback)
- Privileged backend daemon for system tasks
- Modular platform adapters for portability

Build (example):

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

Next steps:
- Implement DBus IPC and privileged backend
- Implement NetworkManagerAdapter DBus calls
- Add systemd service and PolicyKit rules
- Add unit and integration tests

This skeleton now provides two build targets:

- `hotspot-gui` - the QML frontend (exposes a `HotspotDaemon` object to QML)
- `hotspot-daemon` - a privileged backend stub (intended to run as a systemd service)

Service and polkit templates are available under `packaging/`.
