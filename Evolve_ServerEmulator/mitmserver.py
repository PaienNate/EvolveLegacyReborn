# Mitmproxy读取部分：
# 作者：陈泽安
# 链接：https://www.zhihu.com/question/422464846/answer/1521657468
# 来源：知乎
# 著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
import ipaddress

from mitmproxy.addons.block import Block
from mitmproxy.options import Options
from mitmproxy.proxy.config import ProxyConfig
from mitmproxy.proxy.server import ProxyServer
from mitmproxy.tools.dump import DumpMaster
from mitmproxy import http, ctx

import threading
import asyncio
import json
import time
# 判断文件是否存在
import os
# 彩蛋
import datetime

# Sorry I don't use english to make this file.


# 搬运他们到这里……
"""
HTTPS HEADER.I DON'T KNOW WHETHER IS THIS FOR USE.
"""
twokheader = {"Server": "mitmcrack",
              "Date": str(time.strftime('%a, %d %b %Y %H:%M:%S %Z', time.localtime(time.time()))),
              "Content-Type": "application/json",
              "Connection": "close",
              "P3P": "CP=\"See http://www.take2games.com/privacy\""
              }

multiheader = {"Content-Type": "application/json,charset=utf-8",
               "Cache-Control": "no-cache, no-store, must-revalidate",
               "Connection": "keep-alive",
               "Date": str(time.strftime('%a, %d %b %Y %H:%M:%S %Z', time.localtime(time.time()))),
               "Expires": "0",
               "Server": "Pinenut-Crack/1.1",
               "Pragma": "no-cache",
               "X-Powered-By": "PHP/5.3.29"
               }


# For Update Server, there aren't the same, so I write it below, not define here.


def evolvecrack(jsonfile, logstr, optheader, flow):
    print("[松子日志]：" + logstr)
    flow.response = http.HTTPResponse.make(
        200,  # (optional) status code
        jsonfile,  # (optional) content
        optheader)  # (optional)


def readjson(jsonname):
    with open(os.path.abspath('.') + "\\EvolveCrack\\" + jsonname, encoding="utf-8") as f:
        return f.read()


class MyBlock:
    def load(self, loader):
        print("尝试加载MyBlock!")
        loader.add_option(
            "block_global", bool, False,
            """
            Block connections from globally reachable networks, as defined in
            the IANA special purpose registries.
            """
        )
        loader.add_option(
            "block_private", bool, False,
            """
            Block connections from private networks, as defined in the IANA
            special purpose registries. This option does not affect loopback
            addresses.
            """
        )

    def clientconnect(self, layer):
        astr = layer.client_conn.address[0]

        parts = astr.rsplit("%", 1)
        address = ipaddress.ip_address(parts[0])
        if isinstance(address, ipaddress.IPv6Address):
            address = address.ipv4_mapped or address

        if address.is_loopback:
            return

        if ctx.options.block_private and address.is_private:
            ctx.log.warn("Client connection from %s killed by block_private" % astr)
            layer.reply.kill()
        if ctx.options.block_global and address.is_global:
            ctx.log.warn("Client connection from %s killed by block_global" % astr)
            layer.reply.kill()


class Addon(object):
    def __init__(self):
        print("EvolveEmulator插件成功挂载！\nEvolveEmulator addon is successfully running!")

    def response(self):
        print("\n")

    def request(self, flow) -> None:
        # print("我觉得这里怎么不得执行一下啊嗯?")
        if "doorman/1" in flow.request.pretty_url:
            evolvecrack(readjson("doorman.json"), "Game are Trying to request doorman in 2K", twokheader, flow)
        elif "telemetry/1" in flow.request.pretty_url:
            if "configs.generate" in flow.request.text:
                telemetryconfiggenerate = str(readjson("telemetryconfiggenerate.json"))
                # Set Time to Local Time.
                teleconfig = telemetryconfiggenerate.replace("\"serverTime\": 0",
                                                             "\"serverTime\": " + str(int(time.time())))
                evolvecrack(teleconfig, "Game are Trying to request teleconfig in 2K", twokheader, flow)
        elif "sessions/1" in flow.request.pretty_url:
            session = str(readjson("sessions.json"))
            # Here,it changed not only serverTime but also appContext.
            sessionto = json.loads(str(flow.request.get_text()))
            appConText = sessionto["header"]["appContext"]
            sessionre = session.replace("\"serverTime\": 0", "\"serverTime\": " + str(int(time.time())))
            sessionre = sessionre.replace("\"appContext\": 0", "\"appContext\": " + str(appConText))
            evolvecrack(sessionre, "Game are Trying to request sessions-heartbeat in 2K", twokheader, flow)
        elif "news/1" in flow.request.pretty_url:
            news = str(readjson("news.json"))
            evolvecrack(news, "Game are Trying to request news in 2K", twokheader, flow)
        elif "storage/1" in flow.request.pretty_url:
            storage = str(readjson("iteminsert.json"))
            evolvecrack(storage, "Game are Trying to request storages - iteminsert in 2K", twokheader, flow)
        elif "sso/1" in flow.request.pretty_url:
            # It change the appContext too,shit!
            ssoto = json.loads(str(flow.request.get_text()))
            sso = str(readjson("applogon.json"))
            appConText = ssoto["header"]["appContext"]
            sso = sso.replace("\"appContext\": 0", "\"appContext\": " + str(appConText))
            evolvecrack(sso, "Game are Trying to request sso-authlogon in 2K", twokheader, flow)
        elif "stats/1" in flow.request.pretty_url:
            # It change the appContext too.
            statsto = json.loads(str(flow.request.get_text()))
            stats = str(readjson("stats.json"))
            appConText = statsto["header"]["appContext"]
            stats = stats.replace("\"appContext\": 0", "\"appContext\": " + str(appConText))
            evolvecrack(stats, "Game are Trying to request stats in 2K", twokheader, flow)
        elif "entitlements/1" in flow.request.pretty_url:
            if "steamApi.user.checkAppOwnership" in flow.request.text:
                # Set All DLC Unlock(No Steam ID will lock all DLC):
                checkapp = str(readjson("checkAppOwnership.json"))
                checkappto = json.loads(str(flow.request.get_text()))
                appConText = checkappto["header"]["appContext"]
                checkapp1 = checkapp.replace("\"appContext\": 0", "\"appContext\": " + str(appConText))
                checkapps = checkapp1.replace("\"ownersteamid\": \"0\"", "\"ownersteamid\": " + "\"76561198587806892\"")
                evolvecrack(checkapps, "Game are Trying to request checkAppOwnership in 2K", twokheader, flow)
            elif "entitlementDefs.getFirstPartyMapping" in flow.request.text:
                # Unknown request.Just send back as its format.
                mapto = json.loads(str(flow.request.get_text()))
                map = str(readjson("getFirstPartyMapping.json"))
                appConText = mapto["header"]["appContext"]
                map = map.replace("\"appContext\": 0", "\"appContext\": " + str(appConText))
                evolvecrack(map, "Game are Trying to request getFirstPartyMapping in 2K", twokheader, flow)
            elif "grants.find" in flow.request.text:
                # Unknown request 2.Just send back as its format.Need 2 replace.
                grants = str(readjson("grantsfind.json"))
                grantsto = json.loads(str(flow.request.get_text()))
                appConText = grantsto["header"]["appContext"]
                grants1 = grants.replace("\"appContext\": 0", "\"appContext\": " + str(appConText))
                grantss = grants1.replace("\"createdOn\": 0", "\"createdOn\": " + str(int(time.time())))
                evolvecrack(grantss, "Game are Trying to request grantsfind in 2K", twokheader, flow)
        elif "content/1" in flow.request.pretty_url:
            if "strings.get" in flow.request.text:
                strsto = json.loads(str(flow.request.get_text()))
                strs = str(readjson("stringsget.json"))
                appConText = strsto["header"]["appContext"]
                strs = strs.replace("\"appContext\": 0", "\"appContext\": " + str(appConText))
                evolvecrack(strs, "Game are Trying to request stringsget in 2K", twokheader, flow)
        elif "auth.php" in flow.request.pretty_url:
            # This is for dsx-1-b.4v1game.net.
            authmulti = str(readjson("SteamLogin.json"))
            evolvecrack(authmulti, "Game are Trying to Go MultiPlayer.", multiheader, flow)
        elif "requestserver.php" in flow.request.pretty_url:
            # This is for dsx-1-b.4v1game.net.
            requestlb = str(readjson("requestserver.json"))
            evolvecrack(requestlb, "Game are Trying to Go MultiPlayer.", multiheader, flow)
        elif "Production/252019.252019.0/build_config_signed.json" in flow.request.pretty_url:
            # mb_exclusions KrakenElder, Don't know what it is.
            # Although it is transparent with "octet-stream", I think it will works using "readjson".
            # There are two build_config_signed.json here.
            mb_exclusions = readjson("build_config_signed0.json")
            mb_header = {
                "Content-Type": "application/octet-stream",
                "Connection": "keep-alive",
                "Last-Modified": "Tue, 19 Jan 2016 17:29:48 GMT",  # I am not sure whether it will check for that.
                "Accept-Ranges": "bytes",
                "Server": "BmazonS3",  # It's Okay I think,lol.
                "Date": str(time.strftime('%a, %d %b %Y %H:%M:%S %Z', time.localtime(time.time()))),  # now time.
                "ETag": "\"302fd31b05d6893d009f81f5babe6de2\"",  # Seems game doesn't use this.
                "X-Cache": "Never from cloudfront",
                "Via": "1.1 646b6f21a2659c68f7a3822d035b97d3.cloudfront.net (CloudFront)",  # Well, no speed up I think.
                "X-Amz-Cf-Pop": "NRT57-C2",
                "X-Amz-Cf-Id": "Bg5xfYkwkZXBisBgsbBLjoR_u3V_zJfPK9W7T1Jy57bmsktcUqTE5A=="
            }
            evolvecrack(mb_exclusions, "Game are Trying to request mbclusions from updateserver.", mb_header, flow)
        elif "Production/252019.252019/micro_patch_version_signed.json" in flow.request.pretty_url:
            # Micro patch json. If game are not have it in GameDLC08, it will send request to server to get.
            microjson = readjson("micro_patch_version_signed.json")
            micro_header = {"Content-Type": "application/octet-stream",
                            "Connection": "keep-alive",
                            "Date": str(time.strftime('%a, %d %b %Y %H:%M:%S %Z', time.localtime(time.time()))),
                            # now time.
                            "Last-Modified": "Thu, 28 Apr 2016 19:21:44 GMT",
                            "ETag": "\"0b8122e356ca818bf97c80e71379bef4\"",
                            "Cache-Control": "max-age=259200",
                            "Accept-Ranges": "bytes",
                            "Server": "BmazonS3",
                            "X-Cache": "Never from cloudfront",
                            "Via": "1.1 646b6f21a2659c68f7a3822d035b97d3.cloudfront.net (CloudFront)",
                            "X-Amz-Cf-Pop": "NRT57-C2",
                            "X-Amz-Cf-Id": "pKEfUlyGx4Sn1J-uWqjQ6U3OskCD-9iXUYffyW-U99MhYGQOdJIE2Q==",
                            "Age": "172185"
                            }
            evolvecrack(microjson, "Game are Trying to request micro version from updateserver.", micro_header, flow)
        elif "Production/252019.252019/micro_patch_252019.252019_31.pak" in flow.request.pretty_url:
            # This will redirect it to Gitee, which are the same like github, Or you can return the file on your server.
            # flow.request.url = "https://gitee.com/firehomework/evolve-legacy-files" \
            #                   "/raw/master/micro_patch_252019.252019_31.pak"
            print("Here")
            mb_header = {
                "Content-Type": "text/plain",
                "Connection": "keep-alive",
                "Transfer - Encoding": " chunked",
                "Last-Modified": "Tue, 19 Jan 2016 17:29:48 GMT",  # I am not sure whether it will check for that.
                "Accept-Ranges": "bytes",
                "Server": "BmazonS3",  # It's Okay I think,lol.
                "Date": str(time.strftime('%a, %d %b %Y %H:%M:%S %Z', time.localtime(time.time()))),  # now time.
                "ETag": "\"302fd31b05d6893d009f81f5babe6de2\"",  # Seems game doesn't use this.
                "X-Cache": "Never from cloudfront",
                "Via": "1.1 646b6f21a2659c68f7a3822d035b97d3.cloudfront.net (CloudFront)",  # Well, no speed up I think.
                "X-Amz-Cf-Pop": "NRT57-C2",
                "X-Amz-Cf-Id": "Bg5xfYkwkZXBisBgsbBLjoR_u3V_zJfPK9W7T1Jy57bmsktcUqTE5A=="
            }
            with open(os.path.abspath('.') + "\\EvolveCrack\\" + "micro_patch_252019.252019_31.pak", "rb") as f:
                evolvecrack(f.read(), "Game are Trying to request micro pak from updateserver.", mb_header, flow)
            # Actually I don't surely know whether it will work.
            # And the game won't request it when using fake server. Why?!
        elif "Production/252019.252019.31/build_config_signed.json" in flow.request.pretty_url:
            # This is another build_config_signed.json.
            anocof = readjson("build_config_signed.json")
            mb_header = {
                "Content-Type": "application/octet-stream",
                "Connection": "keep-alive",
                "Last-Modified": "Tue, 19 Jan 2016 17:29:48 GMT",  # I am not sure whether it will check for that.
                "Accept-Ranges": "bytes",
                "Server": "BmazonS3",  # It's Okay I think,lol.
                "Date": str(time.strftime('%a, %d %b %Y %H:%M:%S %Z', time.localtime(time.time()))),  # now time.
                "ETag": "\"302fd31b05d6893d009f81f5babe6de2\"",  # Seems game doesn't use this.
                "X-Cache": "Never from cloudfront",
                "Via": "1.1 646b6f21a2659c68f7a3822d035b97d3.cloudfront.net (CloudFront)",  # Well, no speed up I think.
                "X-Amz-Cf-Pop": "NRT57-C2",
                "X-Amz-Cf-Id": "Bg5xfYkwkZXBisBgsbBLjoR_u3V_zJfPK9W7T1Jy57bmsktcUqTE5A=="
            }
            evolvecrack(anocof, "Game are Trying to request another build_config from updateserver.", mb_header, flow)
        else:
            print("[松子日志]：" + "Maybe there are something I forgot to emulate.")
            print("[松子日志]:It's okay as seems there are lots of them just need a response...")
            print(flow.request.pretty_url)
            requestlb = str(readjson("requestserver.json"))
            evolvecrack(requestlb, "Something I forgot to emulate but seems okay", multiheader, flow)


# see source mitmproxy/master.py for details
def loop_in_thread(loop, m):
    asyncio.set_event_loop(loop)  # This is the key.
    m.run_loop(loop.run_forever)


# 用于文件读取的函数
def read_file(filepath):
    with open(filepath) as fp:
        content = fp.read()
    return content


if __name__ == "__main__":
    print("Welcome to use EvolveReborn Software By Pinenut. 严禁倒卖!正版/盗版联机群：366237012")
    print("Lots of thanks:Nemirtingas,schmogmog,nemerod,kiagam")
    # This is an egg,and it doesn't have any useful function, Like a joke, so just ignore it,please?
    d1 = datetime.datetime.today()
    d2 = datetime.datetime(2019, 10, 16)
    if (d1 - d2).days % 365 == 0:
        print("小(夹)彩(私)蛋(活)！今天是对于作者来讲很重要的周年纪念日哦！")
        print("Hey!Today is a very important day for author! Anniversary!")
    else:
        print("A number:" + str((d1 - d2).days % 365))
        print("今天是：" + str(d1))
        print("Today is " + str(d1))
    # This is an egg,and it doesn't have any useful function, Like a joke, so just ignore it,please?
    serverport = 2000
    print("================================================================")
    # 添加读取文件部分
    if not os.path.exists("EvolveCrack"):
        print("严重错误……您的EvolveCrack文件夹呢……考虑重新拷贝一下文件吧……")
        print("Fatal Error...Cannot found EvolveCrack Folder...Copy the crack and try again...")
        os.system("pause")
        exit(1)
    if os.path.exists("motd.txt"):
        print("下面的是motd的公告/包含的信息不由我做主。")
        print("The thing below will read from motd.txt, I don't know what in it.")
        print("===========================MOTD START===================================")
        with open("motd.txt", "r", encoding="utf-8") as fr:
            for line in fr:
                print(line)
        print("===========================MOTD OVER===================================")
    if os.path.exists("serverport.txt"):
        with open("serverport.txt", "r") as fr:
            # 以r形式打开文件
            for line in fr:  # 一行一行读取
                # 读取到了端口号信息
                print("读取到修改后的端口信息，信息为\n" + str(line))
                print("Read successfully!The port now is :\n" + str(line))
                print("正在修改端口号。\nChanging the server port...")
                serverport = int(line)
    # 关闭昵称修改部分
    # if os.path.exists("local_save.txt"):
    #     localsave = read_file("local_save.txt")
    #     if os.path.exists(localsave + "\\" + "settings" + "\\" + "account_name.txt"):
    #         account_name = read_file(localsave + "\\" + "settings" + "\\" + "account_name.txt")
    #         if account_name == "Goldberg":
    #             print("您要考虑修改一个名字吗？这个名字是默认名字，辨识度很差！\n"
    #                   "Do you wanna to change the name? This name is the default name, "
    #                   "there will lots of people named that.\n")
    #             choose = input("输入Y来修改名字，输入其他不修改名字并启动游戏\nInput Y then press Enter to change the name, Others to run "
    #                            "the game without changing the name\n")
    #             if choose == "Y" or choose == "y":
    #                 newname = input("请输入新的名字！（不支持中文！）\nPlease Input your new name!")
    #                 with open(localsave + "\\" + "settings" + "\\" + "account_name.txt", "w+", encoding="utf-8") as f:
    #                     f.write(newname)
    #                 print("修改新名字成功，新名字为：" + newname + "\n Set new name successfully!New name is" + newname)
    #     else:
    #         print("看起来这是您第一次启动游戏。请按照向导设置您的游戏属性！\nSeems this is the first time you run the game,please follow the "
    #               "guide to set your game!")
    #         newname = input("请输入您游戏内的名字！（不支持中文！）\nPlease Input your new name!")
    #         #先得创建文件夹
    #         os.mkdir(localsave + "\\" + "settings")
    #         with open(localsave + "\\" + "settings" + "\\" + "account_name.txt", "w+", encoding="utf-8") as f:
    #             f.write(newname)
    #         print("设置名字成功，您游戏内的名字为：" + newname + "\n Set the name successfully, your new name is" + newname)
    options = Options(listen_host='0.0.0.0', listen_port=serverport, http2=True, confdir="./certs",
                      mode="reverse:http://127.0.0.1:5555")
    m = DumpMaster(options, with_termlog=False, with_dumper=False)
    config = ProxyConfig(options)
    m.server = ProxyServer(config)
    m.addons.add(Addon())
    block_addon = m.addons.get("block")
    m.addons.remove(block_addon)
    m.addons.add(MyBlock())
    print("服务器已经运行！请配置您的serverip为127.0.0.1。您也可以在局域网内做自建服务器。")
    print("Server is running, Port = " + str(serverport) + ", Please configure your serverip to 127.0.0.1（A default "
                                                           "config)")
    print("================================================================")
    # print("正在启动游戏……如果游戏没有成功启动，请手动启动RebornEvolve.exe!\nRunning game...If the game failed to run,please run "
    #       "RebornEvolve.exe by yourself")
    # if(os.path.exists("RebornEvolve.exe")):
    #     os.startfile("RebornEvolve.exe")
    # else:
    #     print("启动失败，可能这个文件被误杀了。")
    #     print("Failed to run the game, maybe the RebornEvolve.exe was deleted by anti-virus software.")
    # run mitmproxy in background, especially integrated with other server
    loop = asyncio.get_event_loop()
    t = threading.Thread(target=loop_in_thread, args=(loop, m))
    t.start()

    # Other servers might be started too.
    # time.sleep(20)
    # print('going to shutdown mitmproxy')
    # m.shutdown()
