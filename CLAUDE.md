# MIDIPedal Control

Desktop configuration tool for MIDIPedal hardware interfaces. Allows users to discover, configure, and test their pedal controllers.

## What This App Does

This is an Electron desktop app for Jake's custom Arduino-based MIDI pedal interface boxes. Users plug in a pedal box via USB, the app discovers it, and they can configure each pedal's settings. **Settings are stored on the device itself** — the app is just a configuration tool and can be closed after setup.

**Important:** Never expose serial ports, paths, baud rates, or other technical/serial details in the UI. The user-facing language should refer to "devices" and "pedal interfaces", not serial ports.

## Tech Stack

- **Electron 25** + **Nuxt 2** (Vue 2) — single page app, no SSR
- **SerialPort 10.4** — communication with Arduino hardware at 115200 baud
- **Webpack 4** — requires `NODE_OPTIONS=--openssl-legacy-provider` to run on Node 17+
- **No CSS framework** — all styling is scoped CSS in Vue components (dark theme, Inter font, indigo accent)
- **No Vuex** — state is managed via two Vue.observable plugins injected as `$global` and `$serial`

## Project Structure

```
src/
  main/                     # Electron main process
    index.js                # App entry, native menu, IPC, quit handling
    mainWindow.js           # BrowserWindow creation (1000x600)
    BrowserWinHandler.js    # Window lifecycle management
    boot/
      index.dev.js          # Dev: DevTools, Vue DevTools, relaunch menu
      index.prod.js         # Prod: custom app:// protocol
  renderer/                 # Nuxt frontend
    layouts/default.vue     # Root layout with header
    components/header.vue   # App header bar with logo
    pages/
      index.vue             # Device discovery & selection
      control.vue           # Pedal settings & testing
    plugins/
      global.js             # Global state ($global.device, $global.debug)
      serial.js             # Serial communication ($serial.connect, etc.)
    store/                  # Empty (using plugins instead)
```

## Serial Protocol

All commands are newline-terminated strings. Values are zero-padded to 3 digits.

### Commands (app → device)
| Command | Format | Description |
|---------|--------|-------------|
| Get settings | `003` | Request all pedal settings |
| Update CC | `001001{pedalId}001{cc}` | Set CC number (0-127) |
| Update mode | `001001{pedalId}002{mode}` | 1=Switched, 2=Variable |
| Update type | `001001{pedalId}003{type}` | 1=Yamaha, 2=Roland |
| Update polarity | `001001{pedalId}004{polarity}` | 0 or 1 |
| Update enabled | `001001{pedalId}005{enabled}` | 0 or 1 |
| Test pedal | `002001{pedalId}{value}` | Send test value (0-127) |
| Factory reset | `006` | Reset device to defaults |

### Status (device → app)
Format: `-{mode},{type},{connected},{enabled},{polarity},{cc},{value}-...`

Each pedal section is delimited by `-`. First character is always `-`.

## Device Discovery

Devices are filtered by USB vendor/product IDs:
- Vendor IDs: `2341` (Arduino)
- Product IDs: `8036`, `8037`

## Running

```bash
NODE_OPTIONS=--openssl-legacy-provider yarn dev
```

## Building

```bash
NODE_OPTIONS=--openssl-legacy-provider yarn build
```

Targets: macOS (DMG), Linux (DEB), Windows (NSIS). Config in `builder.config.js`.

## Key Behaviors

- The serial port is released on window close, reload, navigation away, and app quit (via `beforeunload` + IPC `app-before-quit`)
- If a port can't be opened (locked by another app), a connection error modal appears with Retry/Back options
- Factory reset is on the control page with a confirmation modal
- Debug mode (toggled via View menu) logs raw serial data to the console
- The native Electron menu includes standard Edit/View/Window menus plus Debug Mode toggle and About dialog
