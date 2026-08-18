# Ember

Ember is an ESP32-based RPM and speedometer converter with Wi-Fi configuration and firmware updates. It samples the two analog input paths and measures the fundamental frequency of their conditioned pulse signals, providing a small foundation for converting those measurements into vehicle-specific outputs.

## Current scope

This repository is intentionally a minimal product template. The firmware contains only:

- continuous ADC sampling for the RPM and speed signal paths;
- interrupt-driven frequency measurement for conditioned RPM and speed pulses;
- a compact status protocol;
- a Wi-Fi web interface with live input readings;
- over-the-air (OTA) firmware installation.

The dashboard/display, MFA, vehicle telemetry, lighting, buzzer, BLE, sleep, and legacy microcontroller implementations from the upstream project have been removed.

## Repository layout

```text
ESP32/Ember/
├── components/nvs_wifi_connect/  Wi-Fi provisioning component
├── main/                         Ember application sources and web UI
├── partitions.csv                OTA-capable partition table
└── sdkconfig.defaults            ESP32 project defaults
```

See [`ESP32/Ember/README.md`](ESP32/Ember/README.md) for wiring, setup, building, flashing, web API, and OTA instructions.

## Requirements

- Espressif ESP-IDF 5.2 or newer
- A classic ESP32 target (not ESP32-S3)
- `gzip`, used by the build to package the embedded web page

## Quick start

```bash
cd ESP32/Ember
idf.py set-target esp32
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

On first boot, connect to the `Ember_AP` access point and configure Wi-Fi. Change the default provisioning password before deploying a device.

## License

Ember is distributed under the MIT License. See [`LICENSE.txt`](LICENSE.txt).
