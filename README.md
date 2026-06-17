# DUCO-Miner

**Antigravity DUCO Miner** is a compact, PlatformIO-based firmware that turns ESP8266 boards into a high-performance Duino‑Coin (DUCO) mining rig with a local web dashboard and easy configuration.

## Overview

- Provides a responsive embedded miner for hobbyist and experimental use on NodeMCU/ESP8266 hardware.
- Focuses on stability, responsiveness, and optimized SHA-1 mining performance.

## Key Features

- **High Performance:** CPU overclocking and a hyper-optimized C++ SHA-1 hashing loop for maximum hashrate.
- **Web Dashboard:** Glassmorphic responsive local UI with real-time stats, logs, and remote controls (restart, save settings).
- **Automatic Pool Selection:** Queries Duino-Coin pool selector with robust fallback behavior.
- **Config Persistence:** EEPROM-backed configuration for Wi-Fi, username, miner key, rig ID, LED modes, and difficulty.
- **Robustness:** Watchdog-friendly multitasking keeps the Web server responsive during mining and handles reconnect logic.
- **Telemetry:** Tracks accepted/rejected shares, ping, hashrate, pool status, and displays real-time stats over serial and HTTP.

## Platform

- Written in C++ for the Arduino/ESP8266 ecosystem.
- Built with PlatformIO for NodeMCU/ESP8266 boards.
