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
from logger import logger

import threading
import asyncio
import json
import time
# 判断文件是否存在
import os
# 彩蛋
import datetime

# Sorry I don't use english to make this file.
# PaienNate: Add lots of comments.
mylogger = logger('BYPINENUT.log')
# 搬运他们到这里……
"""
HTTPS HEADER.I DON'T KNOW WHETHER IS THIS USEFUL, BUT JUST HAVE IT WILL GOOD.
"""
twokheader = {"Server": "mitmcrack",
              "Date": str(time.strftime('%a, %d %b %Y %H:%M:%S GMT', time.gmtime(time.time()))),
              "Content-Type": "application/json",
              "Connection": "close",
              "P3P": "CP=\"See http://www.take2games.com/privacy\""
              }

multiheader = {"Content-Type": "application/json,charset=utf-8",
               "Cache-Control": "no-cache, no-store, must-revalidate",
               "Connection": "keep-alive",
               "Date": str(time.strftime('%a, %d %b %Y %H:%M:%S GMT', time.gmtime(time.time()))),
               "Expires": "0",
               "Server": "Pinenut-Crack/1.1",
               "Pragma": "no-cache",
               "X-Powered-By": "PHP/5.3.29"
               }
PC_Production_252019_252019_micro_patch_252019_252019_31 = {
    "Content-Type": "application/octet-stream",
    "Content-Length": "603948",
    "Connection": "keep-alive",
    "Date": str(time.strftime('%a, %d %b %Y %H:%M:%S GMT', time.gmtime(time.time()))),
    "Last-Modified": "Thu, 28 Apr 2016 19:21:44 GMT",
    "ETag": "\"06c6f43c9c1bf077929f0e9b5cda98de\"",
    "Cache-Control": "max-age=259200",
    "Accept-Ranges": "bytes",
    "Server": "AmazonS3",
    "X-Cache": "Miss from cloudfront",
    "Via": "1.1 45fb7111a175c6978b1378f5881fcaee.cloudfront.net (CloudFront)",
    "X-Amz-Cf-Pop": "SFO5-C3",
    "X-Amz-Cf-Id": "XRgbCNfwcxYPt_LVeV-PfKCMKkC08VVTJLzCvR20lk0mN4HMuQ9zbw==",

}
# PaienNate:
# Game has three server. I named them:
# Verify server, 2k server, update server.
# Verify server will be used when you click "Multiplayer"
# 2k server need ca-bundle.crt, and will unlock your monster and characters.
# update server will make your game to version 31 (If don't have, version will be 0, Like @Bluem describe below:
'''

hmm personally entering the hunting mode with a character like Tec Hack and using his shield, the difference is very visible, 
otherwise I would not know how to find out, the perk also affects
It looks like an old version of Evolve, the monsters do a grab and throw animation like the first versions used to do,
and I personally want to thank you for making this a reality. Thanks a lot

'''
# So I will leave comment sorted with these server.

# PaienNate: Other server is okay.For Update Server, there aren't the same, so I write it below, not define here.
def evolvecrack(jsonfile, logstr, optheader, flow):
    # print(flow.request.host)
    mylogger.info("[松子日志]：" + logstr)
    # print("[松子日志]：" + logstr)
    flow.response = http.HTTPResponse.make(
        200,  # (optional) status code
        jsonfile,  # (optional) content
        optheader)  # (optional)

# update server's file seems has some check for blob, so must read as blob.(Although some of them can open as text)
def readblob(jsonname):
    with open(os.path.abspath('.') + "/EvolveCrack/" + jsonname, mode='rb') as f:
        return f.read()

# for server without update server.
def readjson(jsonname):
    with open(os.path.abspath('.') + "/EvolveCrack/" + jsonname, encoding="utf-8") as f:
        return f.read()

# PaienNate: this is the mitmproxy's addon, to unlock global access.
# And I make a crack version without launcher, they can just write my server ip in serverip.txt
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

# PaienNate: This is the key to crack.
class Addon(object):
    def __init__(self):
        print("EvolveEmulator插件成功挂载！\nEvolveEmulator addon is successfully running!")

    def response(self):
        print("\n")

    # Use mitmproxy to replay all the request.
    def request(self, flow) -> None:
        # 2K server.
        if "doorman/1" in flow.request.pretty_url:
            evolvecrack(readjson("doorman.json"), "Game is Trying to request doorman in 2K", twokheader, flow)
        # 2K server.
        elif "telemetry/1" in flow.request.pretty_url:
            if "configs.generate" in flow.request.text:
                telemetryconfiggenerate = str(readjson("telemetryconfiggenerate.json"))
                # Set Time to Local Time.
                teleconfig = telemetryconfiggenerate.replace("\"serverTime\": 0",
                                                             "\"serverTime\": " + str(int(time.time())))
                evolvecrack(teleconfig, "Game is Trying to request teleconfig in 2K", twokheader, flow)
        # 2K server.
        elif "sessions/1" in flow.request.pretty_url:
            session = str(readjson("sessions.json"))
            # Here,it changed not only serverTime but also appContext.
            sessionto = json.loads(str(flow.request.get_text()))
            appConText = sessionto["header"]["appContext"]
            sessionre = session.replace("\"serverTime\": 0", "\"serverTime\": " + str(int(time.time())))
            sessionre = sessionre.replace("\"appContext\": 0", "\"appContext\": " + str(appConText))
            evolvecrack(sessionre, "Game is Trying to request sessions-heartbeat in 2K", twokheader, flow)
        # 2K server.
        elif "news/1" in flow.request.pretty_url:
            news = str(readjson("news.json"))
            evolvecrack(news, "Game is Trying to request news in 2K", twokheader, flow)
        # 2K server.
        elif "storage/1" in flow.request.pretty_url:
            storage = str(readjson("iteminsert.json"))
            evolvecrack(storage, "Game is Trying to request storages - iteminsert in 2K", twokheader, flow)
        # 2K server.
        elif "sso/1" in flow.request.pretty_url:
            # It change the appContext too,shit!
            ssoto = json.loads(str(flow.request.get_text()))
            sso = str(readjson("applogon.json"))
            appConText = ssoto["header"]["appContext"]
            sso = sso.replace("\"appContext\": 0", "\"appContext\": " + str(appConText))
            evolvecrack(sso, "Game is Trying to request sso-authlogon in 2K", twokheader, flow)
        # 2K server.
        elif "stats/1" in flow.request.pretty_url:
            # It change the appContext too.
            statsto = json.loads(str(flow.request.get_text()))
            stats = str(readjson("stats.json"))
            appConText = statsto["header"]["appContext"]
            stats = stats.replace("\"appContext\": 0", "\"appContext\": " + str(appConText))
            evolvecrack(stats, "Game is Trying to request stats in 2K", twokheader, flow)
        # 2K server.
        elif "entitlements/1" in flow.request.pretty_url:
            if "steamApi.user.checkAppOwnership" in flow.request.text:
                # PaienNate: Set All DLC Unlock(No Steam ID will lock all DLC too):
                checkapp = str(readjson("checkAppOwnership.json"))
                checkappto = json.loads(str(flow.request.get_text()))
                appConText = checkappto["header"]["appContext"]
                checkapp1 = checkapp.replace("\"appContext\": 0", "\"appContext\": " + str(appConText))
                checkapps = checkapp1.replace("\"ownersteamid\": \"0\"", "\"ownersteamid\": " + "\"76561198587806892\"")
                evolvecrack(checkapps, "Game is Trying to request checkAppOwnership in 2K", twokheader, flow)
            elif "entitlementDefs.getFirstPartyMapping" in flow.request.text:
                # Unknown request.Just send back as its format.
                mapto = json.loads(str(flow.request.get_text()))
                map = str(readjson("getFirstPartyMapping.json"))
                appConText = mapto["header"]["appContext"]
                map = map.replace("\"appContext\": 0", "\"appContext\": " + str(appConText))
                evolvecrack(map, "Game is Trying to request getFirstPartyMapping in 2K", twokheader, flow)
            elif "grants.find" in flow.request.text:
                # Unknown request 2.Just send back as its format.Need 2 replace.
                grants = str(readjson("grantsfind.json"))
                grantsto = json.loads(str(flow.request.get_text()))
                appConText = grantsto["header"]["appContext"]
                grants1 = grants.replace("\"appContext\": 0", "\"appContext\": " + str(appConText))
                grantss = grants1.replace("\"createdOn\": 0", "\"createdOn\": " + str(int(time.time())))
                evolvecrack(grantss, "Game is Trying to request grantsfind in 2K", twokheader, flow)
        elif "content/1" in flow.request.pretty_url:
            if "strings.get" in flow.request.text:
                strsto = json.loads(str(flow.request.get_text()))
                strs = str(readjson("stringsget.json"))
                appConText = strsto["header"]["appContext"]
                strs = strs.replace("\"appContext\": 0", "\"appContext\": " + str(appConText))
                evolvecrack(strs, "Game is Trying to request stringsget in 2K", twokheader, flow)
        # PaienNate: These are verify server.
        elif "auth.php" in flow.request.pretty_url:
            # This is for dsx-1-b.4v1game.net.
            authmulti = str(readjson("SteamLogin.json"))
            evolvecrack(authmulti, "Game is Trying to Go MultiPlayer.", multiheader, flow)
        # PaienNate: These are verify server.
        elif "requestserver.php" in flow.request.pretty_url:
            # This is for dsx-1-b.4v1game.net.
            requestlb = str(readjson("requestserver.json"))
            evolvecrack(requestlb, "Game is Trying to Go MultiPlayer.", multiheader, flow)
        # PaienNate: These are update server.
        # fix with read blob. And if you use a normal browser header, the original server will response a fake one
        # it has something different in HEX, but if just open with notepad, it can't find any different.
        elif "Production/252019.252019.0/build_config_signed.json" in flow.request.pretty_url:
            mb_exclusions = readblob("build_config_signed0.json")
            mb_header = {
                "Content-Type": "application/octet-stream",
                "Connection": "keep-alive",
                "Last-Modified": "Tue, 19 Jan 2016 17:29:48 GMT",  # I am not sure whether it will check for that.
                "Accept-Ranges": "bytes",
                "Server": "AmazonS3",  # It's Okay I think,lol.
                "Date": str(time.strftime('%a, %d %b %Y %H:%M:%S %Z', time.localtime(time.time()))),  # now time.
                "ETag": "\"302fd31b05d6893d009f81f5babe6de2\"",  # Seems game doesn't use this.
                "X-Cache": "Hit from cloudfront",
                "Via": "1.1 646b6f21a2659c68f7a3822d035b97d3.cloudfront.net (CloudFront)",  # Well, no speed up I think.
                "X-Amz-Cf-Pop": "NRT57-C2",
                "X-Amz-Cf-Id": "Bg5xfYkwkZXBisBgsbBLjoR_u3V_zJfPK9W7T1Jy57bmsktcUqTE5A=="
            }
            evolvecrack(mb_exclusions, "Game is Trying to request mbclusions from updateserver.", mb_header, flow)
        # update server.
        elif "Production/252019.252019/micro_patch_version_signed.json" in flow.request.pretty_url:
            # Micro patch json. If game is not have it in GameDLC08, it will send request to server to get.
            # Yeah fix it!
            microjson = readblob("micro_patch_version_signed.json")
            micro_header = {"Content-Type": "application/octet-stream",
                            "Connection": "keep-alive",
                            "Date": str(time.strftime('%a, %d %b %Y %H:%M:%S GMT', time.gmtime(time.time()))),
                            # now time.
                            "Last-Modified": "Thu, 28 Apr 2016 19:21:44 GMT",
                            "ETag": "\"0b8122e356ca818bf97c80e71379bef4\"",
                            "Cache-Control": "max-age=259200",
                            "Accept-Ranges": "bytes",
                            "Server": "AmazonS3",
                            "X-Cache": "Hit from cloudfront",
                            "Via": "1.1 646b6f21a2659c68f7a3822d035b97d3.cloudfront.net (CloudFront)",
                            "X-Amz-Cf-Pop": "NRT57-C2",
                            "X-Amz-Cf-Id": "pKEfUlyGx4Sn1J-uWqjQ6U3OskCD-9iXUYffyW-U99MhYGQOdJIE2Q==",
                            "Age": "172185"
                            }
            evolvecrack(microjson, "Game is Trying to request micro version from updateserver.", micro_header, flow)
        # update server.
        elif "Production/252019.252019/micro_patch_252019.252019_31.pak" in flow.request.pretty_url:
            fix34patch = readblob("micro_patch_252019.252019_31.pak")
            evolvecrack(fix34patch, "Game is Trying to request the micro pak from updateserver.", PC_Production_252019_252019_micro_patch_252019_252019_31, flow)
            # Actually I don't surely know whether it will work.
            # And the game won't request it when using fake server. Why?!
            # Fix it, yeah!
        # update server.
        elif "Production/252019.252019.31/build_config_signed.json" in flow.request.pretty_url:
            # This is 31 version build_config.
            anocof = readblob("build_config_signed_31.json")
            mb_header = {
                "Content-Type": "application/octet-stream",
                "Connection": "keep-alive",
                "Last-Modified": "Tue, 19 Jan 2016 17:29:48 GMT",  # I am not sure whether it will check for that.
                "Accept-Ranges": "bytes",
                "Server": "AmazonS3",  # It's Okay I think,lol.
                "Date": str(time.strftime('%a, %d %b %Y %H:%M:%S GMT', time.gmtime(time.time()))),  # now time.
                "ETag": "\"302fd31b05d6893d009f81f5babe6de2\"",  # Seems game doesn't use this.
                "X-Cache": "Hit from cloudfront",
                "Via": "1.1 646b6f21a2659c68f7a3822d035b97d3.cloudfront.net (CloudFront)",  # Well, no speed up I think.
                "X-Amz-Cf-Pop": "NRT57-C2",
                "X-Amz-Cf-Id": "Bg5xfYkwkZXBisBgsbBLjoR_u3V_zJfPK9W7T1Jy57bmsktcUqTE5A=="
            }
            evolvecrack(anocof, "Game is Trying to request another build_config from updateserver.", mb_header, flow)
        # If have something useless for crack, just send back a normal data...
        else:
            mylogger.war("[松子日志]：" + "Maybe there are something I forgot to emulate.")
            mylogger.war("[松子日志]:It's okay as seems there are lots of them just need a response...")
            requestlb = str(readjson("requestserver.json"))
            evolvecrack(requestlb, flow.request.pretty_url + " Something I forgot to emulate but seems okay", multiheader, flow)

# PaienNate: These are source for running it without mitmproxy installed.
# Seems just work with mitmproxy v6, You can find your way to run it.
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
    # just delete my joke code. Hope you can see it clearly.
    # I also delete some useless print as I put it in my server.
    serverport = 2000
    print("================================================================")
    # 添加读取文件部分
    if not os.path.exists("EvolveCrack"):
        print("严重错误……您的EvolveCrack文件夹呢……考虑重新拷贝一下文件吧……")
        print("Fatal Error...Cannot found EvolveCrack Folder...Copy the crack and try again...")
        os.system("pause")
        exit(1)
    if os.path.exists("serverport.txt"):
        with open("serverport.txt", "r") as fr:
            # 以r形式打开文件
            for line in fr:  # 一行一行读取
                # 读取到了端口号信息
                print("读取到修改后的端口信息，信息为\n" + str(line))
                print("Read successfully!The port now is :\n" + str(line))
                print("正在修改端口号。\nChanging the server port...")
                serverport = int(line)
    # Use reverse mode, and use certs.
    options = Options(listen_host='0.0.0.0', listen_port=serverport, http2=True, confdir="./certs",
                      mode="reverse:http://127.0.0.1:12345")
    m = DumpMaster(options, with_termlog=False, with_dumper=False)
    config = ProxyConfig(options)
    m.server = ProxyServer(config)
    m.addons.add(Addon())
    block_addon = m.addons.get("block")
    m.addons.remove(block_addon)
    m.addons.add(MyBlock())
    print("服务器已经运行！请配置您的serverip为127.0.0.1(默认即配置）")
    print("Server is running, Port = " + str(serverport) + ", Please configure your serverip to 127.0.0.1（A default "
                                                           "config)")
    print("================================================================")
    print("正在启动游戏……如果游戏没有成功启动，请手动启动RebornEvolve.exe!\nRunning game...If the game failed to run,please run "
          "RebornEvolve.exe by yourself")
    loop = asyncio.get_event_loop()
    t = threading.Thread(target=loop_in_thread, args=(loop, m))
    t.start()