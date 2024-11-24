# Evolve Legacy Reborn  

**国内联机群号**：366237012  

## 介绍 / Instructions  

### **项目简介 / Project Overview**  
该项目旨在对《遗产进化》进行修改，使其能够在局域网环境下运行，无需通过 Steam 进行联机。  
This project modifies *Evolve Legacy* to enable co-op gameplay over LAN/Virtual LAN without requiring Steam.  

### **注意事项 / Important Notices**  

- **严禁倒卖，以及用于引流广告等活动**  
  **Reselling or using this project for promotional advertisements is strictly prohibited.**  
- **若用于直播联机，必须明确注明本项目的使用！**  
  **If used for live streaming, you must clearly credit this project.**  

游戏链接：[Evolve (已停服)](https://store.steampowered.com/app/273350/)  
了解更多关于停服详情与“遗产进化”的公告：  
[中文公告链接](https://support.2k.com/hc/zh-tw/articles/360004104913--Evolve-的重要公告)  
Learn more about the server closure and "Evolve Legacy" here: [Official Announcement](https://support.2k.com/hc/zh-tw/articles/360004104913--Evolve-的重要公告)  

---

## 软件架构 / Software Architecture  

### **架构说明 / Architecture Overview**  
本项目基于 Detours 重定向 和 mitmproxy 实现功能。  
This project is based on **Detours redirection** and **mitmproxy** for functionality.  

---

## 安装教程 / Installation Guide  

1. 解压补丁包，将内容直接放入游戏目录，替换原文件。  
   Unpack the patch files and place them directly in the game directory, replacing the original files.  
2. 使用 **Bin64_SteamRetail** 文件夹内的 `Evolve.exe` 启动游戏即可。  
   Launch the game using `Evolve.exe` from the **Bin64_SteamRetail** folder.  

---

## 编译说明 / Compilation Notes  

1. 服务器模拟部分需要安装 **Python 及 mitmproxy==6.0.1**。  
   Install **Python** and `mitmproxy==6.0.1` for server emulation.  
2. EvolveHooker(Bintrust)模块可根据README的教程自行编译。  
   The **Bintrust module** can be compiled directly by its instruction.  

> **注 / Note:**  
> 由于代码注释包含大量中文，可能对非中文用户带来一定困扰。  
> Code comments are in Chinese, which may cause difficulty for non-Chinese users.  

---

## 项目说明 / Project Details  

1. 当前使用 bintrust（替代 wintrust）实现重定向，并结合 mitmproxy 进行模拟。  
   Currently, **bintrust** (as a replacement for wintrust) is used for redirection, along with **mitmproxy** for simulation.  
2. 您可以查看 EvolveHooker 来查看 BINTRUST 模块的代码。  
   You can check **EvolveHooker** for the source code of the BINTRUST module.  

---

## 贡献人员 / Contributors  

- **@Nemirtingas** (GitLab): 协助解决黑屏验证检查问题。  
  Assisted in solving the screen freeze verification issue.  
- **@schmogmog**: 进行测试协作。  
  Supported with testing.  
- **@nemerod**: 初期分析游戏代码，提供服务器列表。  
  Conducted early-stage game code analysis and provided the server list.  
- **@kiagam**: 使用 Python 工具抓包，为 mitmproxy 提供思路。  
  Used Python tools for packet capture, inspiring the use of mitmproxy.  
- **@1101839859 (Pinenut)**: 整合脚本，模拟服务器验证流程。  
  Integrated scripts and simulated server verification processes.  
- **@pikapika**: 提供测试支持及成品打包。  
  Provided testing support and final build packaging.  
- **@DeinAlbtraum:** 协助测试  
  Assisted with testing.  

---

## 需要帮助？ / How You Can Help  

### **当前问题 / Current Issues**  

1. **性能优化 / Performance Optimization**  
   mitmproxy 作为反代验证服务器的性能存在潜在问题。（正在着手开发，请查看 develop 分支）  
   The performance of mitmproxy as a reverse proxy validation server remains a concern. (Under development; see the `develop` branch.)  

2. **HTTPS 验证 / HTTPS Validation**  
   ~~ws2_32.dll 中使用 `ca-bundle.crt` 进行验证，尚未找到方法移除 HTTPS 头或绕过验证。~~  
   ~~HTTPS headers or validation removal for ws2_32.dll remains unresolved.~~  
   当前全部使用 HTTPS。  
   All communications now use HTTPS.  

---

## 支持作者 / Support  

若愿意支持作者的工作或帮助维持服务器运行，可使用以下 XMR 钱包捐赠：  
If you'd like to support the developer's work or help maintain the servers, you can donate via the following XMR wallet:  

**钱包地址 / Wallet Address**：  
`46QCd5fqNQTG3xgpmnwbsS8X1DBvbd23EaLtYgYTrdRtaKY3knX5gFCKCZQDcirMzTPexqTZRyt14JgsBbkZL79GGSghPsS`  

> **特别说明 / Special Note:**  
> 由于项目的特殊性，目前作者仅接受上述方式捐赠。  
> Due to the nature of the project, the developer only accepts donations through the above method.