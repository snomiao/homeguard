# HomeGuard

Home automation system built with Arduino (ESP8266) and a PHP web backend. Guard nodes installed at each door monitor sensors and control electrical devices, syncing state to a central server.

## Architecture

```
Arduino (ESP8266)  →  HTTP GET  →  PHP Server  →  MySQL
     ↓                                               ↓
74HC595 relays                                  Web Dashboard
```

## Hardware (per Guard node)

- Arduino + ESP8266 WiFi module
- 74HC595 shift register for relay control
- IR motion sensor
- Magnetic door sensor
- Temperature sensor
- Relay-controlled outlets / lights / fans

## Guards / Monitored Locations

| Guard | Sensors | Switches |
|-------|---------|----------|
| 卧室·客厅门卫士 (Bedroom–Living Room Door) | IR motion, door magnetic, temperature | Fan, fluorescent light, nightlight, power outlet (公牛 Z4) |
| 卧室·阳台门卫士 (Bedroom–Balcony Door) | Door magnetic, temperature, ground wire | Ceiling light, desk lamp, fan |
| 客厅·正门卫士 (Living Room Main Door) | Door magnetic, temperature | — |

## Stack

- **Firmware**: Arduino C++ with ESP8266 WiFi, EEPROM state persistence
- **Backend**: PHP + MySQL
- **Frontend**: Materialize CSS

## API

Guard nodes sync via HTTP GET:

```
GET /?action=sync&version=2016-08-01&data_states=<relay_bitmask>&data_temperature=<val>&data_infrared=<val>
```

Web clients POST form submissions to `?action=post` (key-authenticated).
