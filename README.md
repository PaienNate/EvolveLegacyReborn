# Evolve Legacy Reborn
国内联机群号：366237012

#### 介绍/Instruction

该项目旨在对《遗产进化》进行修改，使其能够在局域网环境下，免Steam进行游玩。 

This project will make Evolve legacy coop in LAN/Virtual LAN.  

**该项目禁止用于商业活动！严禁倒卖！如果用于直播联机，必须注明这点！**

进化(已停服） 游戏地址：https://store.steampowered.com/app/273350/   

进化停服说明，及遗产进化是什么（L）公告地址（中文）：   

https://support.2k.com/hc/zh-tw/articles/360004104913--Evolve-%E7%9A%84%E9%87%8D%E8%A6%81%E5%85%AC%E5%91%8A   

#### 软件架构

~~软件架构说明:~~   

~~该项目目前基于Goldberg_emulator和mitmproxy发挥作用；~~  

该项目目前使用Goldberg部分代码和mitmproxy发挥作用

Now it use some goldberg's code and mitmproxy to work.

#### 安装教程

1. 将补丁解压后直接放入游戏目录替换原文件即可。之后使用Bin64_SteamRetail内的Evolve.exe启动游戏。

#### 编译说明

~~goldberg部分请自行参照goldberg补丁（goldberg在gitlab上）编译；~~

Please see Evolve_ServerEmulator and then see bintrust's readme to know how to compile.

Server模拟部分，需要python mitmproxy==6.0.1

bintrust直接编译即可。

很抱歉我写的注释全部是中文，可能会给国外的用户带来很大的困扰。

#### 说明

 1. 内部代码经过多次迭代之后，现在使用bintrust（替代wintrust)实现重定向；使用mitmproxy来进行模拟。
 2. 目前打包的release内使用的是群友的服务器。
  3. 项目若无必要将不会有后续更新。该项目仅做引流用。

#### 参与贡献

参与贡献的人员：   
@Nemirtingas(Gitlab) 协助解决游戏黑屏的验证检查（十分感谢！没有这位大佬的帮助，真的就卡住了）   
@schmogmog 协助进行测试   
@nemerod 初期对游戏代码进行了分析，并给出了游戏连接的服务器列表   
@kiagam 初期使用Python等工具抓取，为后来使用mitmproxy提供思路。   
@1101839859（Pinenut） 按照思路整合脚本，模拟服务器验证。   
@pikapika 提供测试及某些成品打包

#### 帮助我们？

当前有的两个问题：

1. mitmproxy作为反代验证服务器，对其性能的担忧
2. ws2_32.dll(也就是使用ca-bundle.crt的验证)目前还不知道如何脱掉https头，或者去掉这个验证。

#### 支持？

如果乐意请作者一杯咖啡，或者希望帮助作者维持服务器费用，可以使用XMR钱包：

46QCd5fqNQTG3xgpmnwbsS8X1DBvbd23EaLtYgYTrdRtaKY3knX5gFCKCZQDcirMzTPexqTZRyt14JgsBbkZL79GGSghPsS

**由于项目的特殊性，本人目前不接受其他任何捐赠方式。**
