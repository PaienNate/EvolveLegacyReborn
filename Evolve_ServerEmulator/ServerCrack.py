# !/usr/bin/env Python
# coding=utf-8
"""
    EVOLVE REBORN EMULATOR BY PINENUT
"""
from mitmproxy import http
from mitmproxy import ctx
import json
import time
import os

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
baiduheader = {
    "Bdpagetype": "2",
    "Bdqid": "0x843bff380005b784",
    "Cache-Control": "private",
    "Connection": "keep-alive",
    "Content-Encoding": "gzip",
    "Content-Type": "text/html;charset=utf-8",
    "Date": "Mon, 08 Mar 2021 07:10:30 GMT",
    "Expires": "Mon, 08 Mar 9021 07:10:30 GMT",
    "Server": "BWS/1.1",
    "Set-Cookie": "BDSVRTM=135; path=/",
    "Strict-Transport-Security": "max-age=172800",
    "Traceid": "161518743002161303149528490052616042372",
    "X-Ua-Compatible": "IE=Edge,chrome=1",
    "Transfer-Encoding": "chunked",
}


# For Update Server, there aren't the same, so I write it below, not define here.

def evolvecrack(jsonfile, logstr, optheader, flow):
    ctx.log.info("[Pinenut Log]：" + logstr)
    flow.response = http.HTTPResponse.make(
        200,  # (optional) status code
        jsonfile,  # (optional) content
        optheader)  # (optional)


def readjson(jsonname):
    with open(os.path.abspath('.') + "\\EvolveCrack\\" + jsonname, encoding="utf-8") as f:
        return f.read()


def request(flow: http.HTTPFlow) -> None:
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
            checkapps = checkapp1.replace("\"ownersteamid\": \"0\"",
                                          "\"ownersteamid\": " + "\"76561198587806892\"")  # For this, the steamid seems just not 0 the dlc will unlock.
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
        # For Local Version, Just get the file in local.
        micropak = readjson("micro_patch_252019.252019_31.pak")
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
        evolvecrack(micropak, "Game are Trying to request another build_config from updateserver.", mb_header, flow)
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
        ctx.log.info("[Pinenut Log]：" + "Now Let us redirect it to our official page..")
        # This is another build_config_signed.json.
        mypage = readjson("index.html")
        evolvecrack(mypage, "Game are Trying to request another build_config from updateserver.", baiduheader, flow)