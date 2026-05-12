# Evolve Legacy Reborn

**国内联机群号**：366237012

## 介绍 / Instructions

### 项目简介 / Project Overview
该项目旨在对《遗产进化》进行修改，使其能够在局域网环境下运行，无需通过 Steam 进行联机。
This project modifies *Evolve Legacy* to enable co-op gameplay over LAN/Virtual LAN without requiring Steam.

### 注意事项 / Important Notices

- **严禁倒卖，以及用于引流广告等活动**
  **Reselling or using this project for promotional advertisements is strictly prohibited.**
- **若用于直播联机，必须明确注明本项目的使用！**
  **If used for live streaming, you must clearly credit this project.**

游戏链接：[Evolve (已停服)](https://store.steampowered.com/app/273350/)
了解更多关于停服详情与"遗产进化"的公告：
[中文公告链接](https://support.2k.com/hc/zh-tw/articles/360004104913--Evolve-的重要公告)
Learn more about the server closure and "Evolve Legacy" here: [Official Announcement](https://support.2k.com/hc/zh-tw/articles/360004104913--Evolve-的重要公告)

---

## 软件架构 / Software Architecture

### 架构说明 / Architecture Overview
本项目使用 **MinHook** 实现 API 钩子重定向，**CivetWeb + MbedTLS** 提供嵌入式 HTTPS 服务器。
This project uses **MinHook** for API hook redirection and **CivetWeb + MbedTLS** for an embedded HTTPS server.

---

## 安装教程 / Installation Guide

1. 解压补丁包，将内容直接放入游戏目录的 `Bin64_SteamRetail/` 文件夹。
   Unpack the patch files and place them directly in the game's `Bin64_SteamRetail/` folder.
2. 编辑 `EvolveLogging.ini`：设置 `use_internal_server = true` 使用内置服务器，或配置外部服务器地址。
   Edit `EvolveLogging.ini`: set `use_internal_server = true` for the built-in server, or configure an external server address.
3. 使用 `Evolve.exe` 启动游戏即可。
   Launch the game using `Evolve.exe`.

---

## 编译说明 / Compilation Notes

### 依赖 / Dependencies
- **CMake** 3.21+
- C++17 编译器 (MSVC 2022 或 GCC/MinGW 11+)
- 无需外部包管理器（依赖通过 CMake FetchContent 自动下载）

### Windows (MSVC)
```bash
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 \
  -DEVOLVE_BUILD_BINTRUST=ON
cmake --build build --config Release
```

### Linux 交叉编译 (MinGW)
```bash
sudo apt install g++-mingw-w64-x86-64 cmake ninja-build
cmake -S . -B build-mingw -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=cmake/mingw-w64-x86_64.cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DEVOLVE_BUILD_BINTRUST=ON
cmake --build build-mingw
```

产物与 MSVC 构建功能完全等价，均包含嵌入式 CivetWeb+MbedTLS HTTPS 服务器。
Output BINTRUST.dll is functionally identical to the MSVC build, including the embedded HTTPS server.

---

## 项目说明 / Project Details

1. BINTRUST.dll 通过 MinHook 钩住 `getaddrinfo`、`WinHttpConnect` 等函数，将游戏通信重定向到本地服务器。
   BINTRUST.dll uses MinHook to intercept `getaddrinfo`, `WinHttpConnect` and other functions, redirecting game traffic to the local server.
2. 内置 CivetWeb 服务器使用 MbedTLS 进行 TLS 加密，运行时自动生成服务器证书。
   The built-in CivetWeb server uses MbedTLS for TLS encryption and generates server certificates at runtime.

---

## 贡献人员 / Contributors

- **@Nemirtingas** (GitLab): 协助解决黑屏验证检查问题。
  Assisted in solving the screen freeze verification issue.
- **@schmogmog**: 进行测试协作。
  Supported with testing.
- **@nemerod**: 初期分析游戏代码，提供服务器列表。
  Conducted early-stage game code analysis and provided the server list.
- **@kiagam**: 使用 Python 工具抓包，为服务器模拟提供思路。
  Used Python tools for packet capture, inspiring server emulation.
- **@1101839859 (Pinenut)**: 整合脚本，模拟服务器验证流程。
  Integrated scripts and simulated server verification processes.
- **@pikapika**: 提供测试支持及成品打包。
  Provided testing support and final build packaging.
- **@DeinAlbtraum**: 协助测试
  Assisted with testing.

---

## 支持作者 / Support

若愿意支持作者的工作或帮助维持服务器运行，可使用以下 XMR 钱包捐赠：
If you'd like to support the developer's work or help maintain the servers, you can donate via the following XMR wallet:

**钱包地址 / Wallet Address**：
`46QCd5fqNQTG3xgpmnwbsS8X1DBvbd23EaLtYgYTrdRtaKY3knX5gFCKCZQDcirMzTPexqTZRyt14JgsBbkZL79GGSghPsS`

> **特别说明 / Special Note:**
> 由于项目的特殊性，目前作者仅接受上述方式捐赠。
> Due to the nature of the project, the developer only accepts donations through the above method.
