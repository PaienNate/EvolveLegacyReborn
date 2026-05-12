# EvolveHooker (BINTRUST)

Game patch DLL that hooks Windows API functions to redirect Evolve's server communication and bypass validation checks.

## Changelog

- Replaced `KNSoft.SlimDetours` with **MinHook** for API hooking.
- Moved initialization out of `DllMain` to a worker thread to fix MessageBox freeze issues.
- Added **embedded CivetWeb HTTPS server** as an alternative to the external Python mitmproxy server.
- Replaced **OpenSSL** with **MbedTLS** for TLS and certificate generation.
- Build system migrated from Visual Studio `.vcxproj` to **CMake** with cross-compilation support.

## Build Instructions

### Prerequisites

- **CMake** 3.21 or later
- C++17 compiler

### Windows (MSVC)

```bash
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 \
  -DEVOLVE_BUILD_BINTRUST=ON \
  -DEVOLVE_BINTRUST_INTERNAL_SERVER=ON
cmake --build build --config Release
```

Output: `build/EvolveHooker/Release/BINTRUST.dll`

### Windows (MinGW cross-compilation from Linux)

```bash
sudo apt install g++-mingw-w64-x86-64 cmake ninja-build

cmake -S . -B build-mingw -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=cmake/mingw-w64-x86_64.cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DEVOLVE_BUILD_BINTRUST=ON
cmake --build build-mingw
```

Output: `build-mingw/EvolveHooker/BINTRUST.dll`

> **Note:** MinGW builds produce a basic BINTRUST.dll without the embedded internal server.
> Use the external Python `mitmserver.py` for server emulation with MinGW builds.

### Build Options

| Option | Default | Description |
|--------|---------|-------------|
| `EVOLVE_BUILD_BINTRUST` | `OFF` | Build BINTRUST.dll (Windows only) |
| `EVOLVE_BINTRUST_INTERNAL_SERVER` | `OFF` | Enable embedded CivetWeb HTTPS server |
| `EVOLVE_BUILD_HARNESS` | `OFF` | Build standalone server harness for testing |
| `EVOLVE_BUILD_TESTING` | `ON` | Build and run server logic tests |

## How It Works

### Game Validation Process

1. **DLC Validation** - The game sends HTTPS requests to 2K servers via `ca-bundle.crt` and `ws2_32.dll`.
2. **Multiplayer Authorization** - Uses `WINHTTP.DLL` for multiplayer auth and hotfix downloads.
3. **Steam Environment Validation** - Checks environment variables (`SteamAppId`, `SteamGameId`) and registry for Steam client.
4. **File Signature Validation** - Verifies `steam_api64.dll` and `steamclient64.dll` signatures via `WINTRUST.DLL`.

### Patch Mechanism

1. **DNS Redirection** - Hooks `getaddrinfo` to redirect `2k.com` lookups to the crack server.
2. **HTTP Redirection** - Hooks `WinHttpConnect` and `WinHttpOpenRequest` to redirect traffic and ignore certificate errors.
3. **Signature Bypass** - Replaces `WINTRUST.DLL` with `BINTRUST.DLL`; `WinVerifyTrust` always returns success.
4. **Steam Emulation** - Hooks `GetEnvironmentVariableA` and `RegQueryValueExA` to simulate Steam environment.
5. **IME Fix** - Disables IME via `ImmDisableIME` to prevent crashes with Chinese input methods.

### Hook Summary

| Target | DLL | Hook Function | Purpose |
|--------|-----|---------------|---------|
| `getaddrinfo` | ws2_32 | `Evolve_getaddrinfo` | Redirect 2k.com DNS |
| `WinHttpConnect` | winhttp | `Evolve_WinHttpConnect` | Redirect HTTP connections |
| `WinHttpOpenRequest` | winhttp | `Evolve_WinHttpOpenRequest` | Force HTTPS, ignore cert errors |
| `GetEnvironmentVariableA` | kernel32 | `Evolve_GetEnvironmentVariableA` | Fake Steam app ID |
| `RegQueryValueExA` | advapi32 | `Evolve_RegQueryValueExA` | Fake Steam registry values |

## Configuration

Edit `EvolveLogging.ini` in the game's `Bin64_SteamRetail` directory:

```ini
[server]
server_domain = 117.72.125.6
server_port = 2000
use_internal_server = false
internal_bind_address = 127.0.0.1

[steam]
emu_steam = false
dll_path = steamclient64.dll
```

When `use_internal_server = true`, the DLL runs its own HTTPS server using the assets from the `EvolveCrack/` directory and CA certificates from `certs/`.

## Third-Party Libraries

- [MinHook](https://github.com/TsudaKageyu/minhook) - BSD-2-Clause
- [CivetWeb](https://github.com/civetweb/civetweb) - MIT
- [MbedTLS](https://github.com/Mbed-TLS/mbedtls) - Apache-2.0
- [SimpleIni](https://github.com/brofield/simpleini) - MIT
- [EasyLogging++](https://github.com/abumq/easyloggingpp) - MIT

Inspired by [Goldberg Steam Emulator](https://gitlab.com/Mr_Goldberg/goldberg_emulator) (LGPL v2.1+).
