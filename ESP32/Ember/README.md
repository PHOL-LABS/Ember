# Ember ESP32 firmware

Ember converts vehicle RPM and speedometer signals. The firmware retains a deliberately small set of services: two analog samples, two frequency measurements, Wi-Fi, a status protocol, a browser UI, and OTA updates.

## Hardware target and input map

The target is the original **ESP32** (`idf.py set-target esp32`), not ESP32-S3.

| Signal | ESP32 input | Purpose |
|---|---:|---|
| RPM analog | GPIO34 / ADC1 channel 6 | Observe the conditioned analog signal level |
| Speed analog | GPIO35 / ADC1 channel 7 | Observe the conditioned analog signal level |
| RPM pulse | GPIO32 | Rising-edge frequency measurement |
| Speed pulse | GPIO33 | Rising-edge frequency measurement |

GPIO32 and GPIO33 must receive logic-level signals produced by suitable input conditioning. Vehicle signals must **never** be connected directly to ESP32 pins. Add protection, level shifting, filtering, and a Schmitt-trigger/comparator appropriate to the vehicle and board design. ESP32 inputs are not 5 V or automotive-voltage tolerant.

The frequency counter rejects pulses closer than 100 μs and reports zero when no edge arrives for one second. Conversion from hertz to RPM or road speed is intentionally left to the product-specific layer.

## Prerequisites

1. Install and activate ESP-IDF 5.2 or newer.
2. Install `gzip` (the web page is compressed and embedded during the build).
3. Connect a supported classic ESP32 board.

## Configure and build

```bash
cd ESP32/Ember
idf.py set-target esp32
idf.py menuconfig
idf.py build
```

Important settings are under **NVS WiFi Connect configuration**. The defaults create an access point named `Ember_AP` with password `configureme`. Replace that password for any real deployment.

Flash and open the serial monitor:

```bash
idf.py -p /dev/ttyUSB0 flash monitor
```

## Wi-Fi provisioning

At startup Ember reads saved Wi-Fi configuration from NVS. If none is usable, it starts the provisioning server in access-point mode. Connect to `Ember_AP`, visit `http://192.168.4.1/`, and enter the network credentials. Once provisioned, open the IP address assigned by the local network.

## Web interface and protocol

The root page shows live raw ADC readings and extracted frequencies. The underlying HTTP protocol is intentionally small:

| Method | Path | Description |
|---|---|---|
| `GET` | `/` | Embedded Ember web interface |
| `GET` | `/api/status` | Current readings as JSON |
| `POST` | `/api/ota` | Raw ESP-IDF application `.bin` image |

Status response example:

```json
{
  "product": "Ember",
  "rpm_hz": 42.50,
  "speed_hz": 12.25,
  "rpm_adc": 2010,
  "speed_adc": 1984
}
```

The internal protocol module also recognizes `status` and `restart` commands, ready for a future transport without coupling acquisition code to the web server.

## OTA update

Build a new image, open the web interface, select `build/Ember.bin`, and choose **Install**. Ember writes it to the inactive OTA partition and selects it for the next boot. Restart the device after the upload succeeds.

For production, add authentication and transport security before exposing OTA beyond a trusted network.

## Architecture

- `adc.c`: 100 Hz ADC acquisition of both analog inputs.
- `frequency_count.c`: reusable interrupt-driven rising-edge period measurement.
- `tacho.c` and `speedometer.c`: named input adapters.
- `protocol.c`: transport-independent status serialization and commands.
- `ember_web.c`: HTTP UI, status endpoint, Wi-Fi startup, and OTA upload.
- `main.c`: NVS and service initialization plus the sampling task.

## Clean rebuild

```bash
idf.py fullclean
idf.py set-target esp32
idf.py build
```
