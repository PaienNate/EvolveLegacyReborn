# EvolveHooker(Bintrust)

## 更新日志 (Changelog)

- 修改了 Hook 逻辑。  
  Updated the hook logic.  
- 使用 `KNSoft.SlimDetours` 包替代原有的 `Detours` 包。  
  Replaced the `Detours` package with `KNSoft.SlimDetours`.  
- 添加了 INI 配置库和日志库。  
  Added an INI configuration library and a logging library.  

---

## 编译说明 (Build Instructions)

1. 请使用 **Visual Studio 2022** 编译此项目。  
   Use **Visual Studio 2022** to compile this project.  
2. 确保安装了 `KNSoft.SlimDetours` 的 NuGet 包：  
   Ensure that the NuGet package `KNSoft.SlimDetours` is installed:  
   ```bash
   Install-Package KNSoft.SlimDetours

## 原理说明 (How It Works)

### 游戏验证流程 (Game Validation Process)

1. **DLC 验证 (DLC Validation)**

   - 游戏通过 **`ca-bundle.crt` 文件** 和 **`ws2_32.dll`** 向 **2K 验证服务**发起 HTTPS 请求，用于解锁 DLC 内容。
     The game sends HTTPS requests to the **2K validation service** using the **`ca-bundle.crt` file** and **`ws2_32.dll`** to unlock DLC content.

2. **多人游戏授权与热更新 (Multiplayer Authorization and Hotfix Updates)**

   - 游戏通过 **`WINHTTP.DLL`** 向服务器请求多人游戏授权和热更新补丁文件。
     The game uses **`WINHTTP.DLL`** to request multiplayer authorization and download hotfix updates from the server.

3. **Steam 环境验证 (Steam Environment Validation)**

   - 游戏通过以下方法验证 Steam 环境：

     The game validates the Steam environment through the following methods:

     - 环境变量验证 (Environment Variable Check)

       - 检查环境变量 `SteamAppId` 和 `SteamGameId` 是否为正确的 APPID。
         It checks the environment variables `SteamAppId` and `SteamGameId` to ensure they match the correct APPID.

     - 注册表检查 (Registry Check)

       - 游戏直接读取 **`steamclient64.dll`** 的注册表位置，并尝试与其通信，以验证 Steam 服务状态。
         The game reads the **registry location of `steamclient64.dll`** and attempts to communicate with it to verify the status of Steam services.

       > **注意 (Note):** 与大多数 Steam 游戏不同，该游戏不通过 **`steam_api64.dll`** 与 Steam 通信。
       > **Unlike most Steam games, this game does not use `steam_api64.dll` for Steam communication.**

4. **文件签名验证 (File Signature Validation)**

   - 游戏通过 

     `WINTRUST.DLL`

      对以下文件进行证书签名验证：

     The game uses 

     `WINTRUST.DLL`

      to verify the certificate signatures of the following files:

     - **`steam_api64.dll`**

     - `steamclient64.dll`

       > **注意 (Note):** 尽管游戏不直接依赖 `steam_api64.dll`，但删除该文件会触发错误。
       > **Even though the game does not directly depend on `steam_api64.dll`, deleting the file will cause errors.**

------

### 本补丁的工作原理 (How This Patch Works)

1. **拦截 DLC 验证请求 (Intercepting DLC Validation Requests)**

   - 本补丁钩住 **`ws2_32.dll` 的 `getaddrinfo` 函数**，当检测到请求的地址中包含 `2k.com` 字符串时，将其解析为破解服务器的 IP 地址，以解锁 2K 验证 DLC。
     This patch hooks the **`getaddrinfo` function in `ws2_32.dll`**. When it detects that the requested address contains the string `2k.com`, it redirects the IP resolution to the crack server to unlock 2K-validated DLCs.
   - 同时，本补丁钩住 **`WinHTTPConnect` 函数**，将其请求同样重定向到破解服务器。
     Additionally, it hooks the **`WinHTTPConnect` function**, redirecting these requests to the crack server.

2. **处理 HTTPS 请求 (Handling HTTPS Requests)**

   - 对于使用 **`ws2_32.dll`** 的 HTTPS 连接：

     - 游戏通过读取 **`ca-bundle.crt` 文件**来获取根证书。本补丁将该文件替换为破解服务器的根证书，以通过验证。
       For HTTPS connections using **`ws2_32.dll`**, the game retrieves root certificates via the **`ca-bundle.crt` file**. This patch replaces the file with the crack server's root certificate to ensure trust.

   - 对于 **`WINHTTP.DLL`** 的 HTTPS 请求：

     - 由于其使用系统根证书进行验证，本补丁通过钩住 

       `WinHTTPOpenRequest` 函数

       ，使其忽略证书错误，从而避免用户安装破解证书引发安全问题。

       For 

       `WINHTTP.DLL`

        connections, which use the system root certificate store, this patch hooks the 

       `WinHTTPOpenRequest` function

        to ignore certificate errors. This avoids requiring the user to install the crack certificate, which could pose security risks.

       > **注意 (Note):** 切勿将 **`ca-bundle.crt`** 的根证书安装到你的系统中，否则你可能面临严重的中间人攻击风险！
       > **Do not install the root certificate from the `ca-bundle.crt` file into your system! This could expose you to severe man-in-the-middle attacks.**

3. **绕过文件签名验证 (Bypassing File Signature Validation)**

   - 本补丁通过十六进制修改 **`Evolve.exe`**，将导入的 `WINTRUST.DLL` 名称替换为 `BINTRUST.DLL`。
     This patch modifies **`Evolve.exe`** via hexadecimal editing to replace the imported `WINTRUST.DLL` name with `BINTRUST.DLL`.
   - 本补丁在自定义的 `BINTRUST.DLL` 中实现了唯一的导入函数 **`WinVerifyTrust`**，使其始终返回 `0`，从而跳过签名验证。
     In the custom `BINTRUST.DLL`, this patch implements the sole imported function **`WinVerifyTrust`**, which always returns `0`, bypassing the signature verification.

4. **拦截 Steam 验证逻辑 (Intercepting Steam Validation Logic)**

   - 本补丁钩住 **`Real_RegQueryValueExA` 函数**，将其重定向到 **Goldberg Emu (EMU)** 的虚拟注册表路径，以模拟 Steam 环境。
     This patch hooks the **`Real_RegQueryValueExA` function** and redirects it to the virtual registry path of **Goldberg Emu (EMU)** to emulate the Steam environment.
   - 游戏检查环境变量 `SteamAppId` 和 `SteamGameId`。如果检测失败，游戏会尝试通过 Steam 重新启动。本补丁通过钩住 **`GetEnvironmentVariableA` 函数** 修复此问题。
     The game verifies the environment variables `SteamAppId` and `SteamGameId`. If the check fails, the game attempts to restart via Steam. This patch fixes the issue by hooking the **`GetEnvironmentVariableA` function**.

5. **修复中文输入法导致的闪退问题 (Fixing Crashes Caused by Chinese IMEs)**

   - 对于中国玩家，使用中文输入法会导致游戏闪退。本补丁钩住游戏处理输入法上下文**ImmAssociateContext**的函数，强制禁用输入法。
     For Chinese players, using a Chinese Input Method Editor (IME) causes the game to crash. This patch hooks the game's IME context functions to forcibly disable IME usage.

## THANKS:
用了下面这些MIT的库：
This project uses third-party libraries or code licensed under the MIT License. Specifically:
- [KNSoft.SlimDetours](https://github.com/KNSoft/KNSoft.SlimDetours) - Copyright (c) KNSoft.org
- [SimpleIni](https://github.com/brofield/simpleini) - Copyright (c) 2006-2022 Brodie Thiesfield
- [EasyLoggingCpp](https://github.com/abumq/easyloggingpp) - Copyright (c) 2012-present @abumq (Majid Q.)

以及代码灵感来源于这个LGPL库：
This project uses some code from libraries licensed under the GNU Lesser General Public License (LGPL). The specific libraries used are listed below:

- [Goldberg Steam Emulator](https://gitlab.com/Mr_Goldberg/goldberg_emulator) - Licensed under LGPL v2.1 or later.