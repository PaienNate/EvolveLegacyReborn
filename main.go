package main

import "C" // 必须导入 C 伪包?

import (
	"crypto/tls"
	"embed"
	"encoding/json"
	"fmt"
	"image"
	"image/png"
	"io"
	"junk/config"
	"junk/constant"
	"junk/utils"
	"net/http"
	"strconv"
	"strings"
	"time"

	"github.com/gin-gonic/gin"
)

//go:embed EvolveCrack/*
var crackFiles embed.FS

// 读取文本文件
func readTextFile(fileName string) (string, error) {
	data, err := crackFiles.ReadFile("EvolveCrack/" + fileName)
	if err != nil {
		utils.SUGAR.Errorw("读取文件失败", "fileName", fileName, "error", err)
		return "", err
	}
	return string(data), nil
}

// 读取二进制文件
func readBlobFile(fileName string) ([]byte, error) {
	data, err := crackFiles.ReadFile("EvolveCrack/" + fileName)
	if err != nil {
		utils.SUGAR.Errorf("读取二进制文件失败: %s, 错误: %v", fileName, err)
		return nil, err
	}
	return data, nil
}

// 处理请求体的JSON，修改时间戳和appContext
func processRequestBody(c *gin.Context, jsonFileName string) (string, error) {
	var requestBody map[string]interface{}
	body, err := io.ReadAll(c.Request.Body)
	if err != nil {
		return "", err
	}

	if err := json.Unmarshal(body, &requestBody); err != nil {
		// 打印日志
		utils.SUGAR.Errorf("解析请求体失败: %v", err)
	}

	jsonData, err := readTextFile(jsonFileName)
	if err != nil {
		return "", err
	}

	// 获取appContext
	header, exists := requestBody["header"].(map[string]interface{})
	if exists {
		appContext, hasAppContext := header["appContext"]
		if hasAppContext {
			jsonData = strings.Replace(jsonData, "\"appContext\": 0", fmt.Sprintf("\"appContext\": %v", appContext), 1)
		}
	}

	// 替换时间戳
	currentTime := time.Now().Unix()
	jsonData = strings.Replace(jsonData, "\"serverTime\": 0", fmt.Sprintf("\"serverTime\": %d", currentTime), 1)
	jsonData = strings.Replace(jsonData, "\"createdOn\": 0", fmt.Sprintf("\"createdOn\": %d", currentTime), 1)

	return jsonData, nil
}

// 设置响应头
func setHeaders(c *gin.Context, headers map[string]string) {
	for key, value := range headers {
		c.Header(key, value)
	}
}

// 发送响应
func sendResponse(c *gin.Context, data interface{}, headers map[string]string, logMessage string) {
	utils.SUGAR.Infow("[松子日志]", "message", logMessage)
	setHeaders(c, headers)

	// 根据类型不同发送不同类型的响应
	switch v := data.(type) {
	case string:
		// c.JSON(http.StatusOK, data)
		c.Data(http.StatusOK, headers["Content-Type"], []byte(v))
	case []byte:
		// 否则使用实际长度
		c.Header("Content-Length", strconv.Itoa(len(v)))
		c.Data(http.StatusOK, headers["Content-Type"], v)
	default:
		c.JSON(http.StatusOK, data)
	}
}

// 处理所有请求的函数
func handleRequest(c *gin.Context) {
	url := c.Request.URL.String()

	// 2K服务器请求处理
	if strings.Contains(url, "doorman/1") {
		doormanJSON, err := processRequestBody(c, "doorman.json")
		if err != nil {
			c.String(http.StatusInternalServerError, "无法读取doorman.json")
			return
		}
		sendResponse(c, doormanJSON, constant.TwokHeader, "Game is Trying to request doorman in 2K")
		return
	}
	if strings.Contains(url, "telemetry/1") && strings.Contains(c.Request.URL.RawQuery, "configs.generate") {
		telemetryConfig, err := processRequestBody(c, "telemetryconfiggenerate.json")
		if err != nil {
			c.String(http.StatusInternalServerError, "无法读取telemetryconfiggenerate.json")
			return
		}
		// 替换时间
		currentTime := time.Now().Unix()
		telemetryConfig = strings.Replace(telemetryConfig, "\"serverTime\": 0", fmt.Sprintf("\"serverTime\": %d", currentTime), 1)
		sendResponse(c, telemetryConfig, constant.TwokHeader, "Game is Trying to request teleconfig in 2K")
		return
	}
	if strings.Contains(url, "sessions/1") {
		session, err := processRequestBody(c, "sessions.json")
		if err != nil {
			c.String(http.StatusInternalServerError, "处理sessions请求出错")
			return
		}
		sendResponse(c, session, constant.TwokHeader, "Game is Trying to request sessions-heartbeat in 2K")
		return
	}
	if strings.Contains(url, "news/1") {
		news, err := processRequestBody(c, "news.json")
		if err != nil {
			c.String(http.StatusInternalServerError, "无法读取news.json")
			return
		}
		sendResponse(c, news, constant.TwokHeader, "Game is Trying to request news in 2K")
		return
	}
	if strings.Contains(url, "storage/1") {
		storage, err := processRequestBody(c, "iteminsert.json")
		if err != nil {
			c.String(http.StatusInternalServerError, "无法读取iteminsert.json")
			return
		}
		sendResponse(c, storage, constant.TwokHeader, "Game is Trying to request storages - iteminsert in 2K")
		return
	}
	if strings.Contains(url, "sso/1") {
		sso, err := processRequestBody(c, "applogon.json")
		if err != nil {
			c.String(http.StatusInternalServerError, "处理sso请求出错")
			return
		}
		sendResponse(c, sso, constant.TwokHeader, "Game is Trying to request sso-authlogon in 2K")
		return
	}
	if strings.Contains(url, "stats/1") {
		stats, err := processRequestBody(c, "stats.json")
		if err != nil {
			c.String(http.StatusInternalServerError, "处理stats请求出错")
			return
		}
		sendResponse(c, stats, constant.TwokHeader, "Game is Trying to request stats in 2K")
		return
	}
	if strings.Contains(url, "entitlements/1") {
		body, err := io.ReadAll(c.Request.Body)
		if err != nil {
			c.String(http.StatusInternalServerError, "读取请求体失败")
			return
		}
		c.Request.Body = io.NopCloser(strings.NewReader(string(body))) // 重置请求体，以便后续读取

		bodyStr := string(body)

		if strings.Contains(bodyStr, "steamApi.user.checkAppOwnership") {
			checkApp, err := processRequestBody(c, "checkAppOwnership.json")
			if err != nil {
				c.String(http.StatusInternalServerError, "处理checkAppOwnership请求出错")
				return
			}
			checkApp = strings.Replace(checkApp, "\"ownersteamid\": \"0\"", "\"ownersteamid\": \"76561198587806892\"", 1)
			sendResponse(c, checkApp, constant.TwokHeader, "Game is Trying to request checkAppOwnership in 2K")
			return
		} else if strings.Contains(bodyStr, "entitlementDefs.getFirstPartyMapping") {
			mapping, err := processRequestBody(c, "getFirstPartyMapping.json")
			if err != nil {
				c.String(http.StatusInternalServerError, "处理getFirstPartyMapping请求出错")
				return
			}
			sendResponse(c, mapping, constant.TwokHeader, "Game is Trying to request getFirstPartyMapping in 2K")
			return
		} else if strings.Contains(bodyStr, "grants.find") {
			grants, err := processRequestBody(c, "grantsfind.json")
			if err != nil {
				c.String(http.StatusInternalServerError, "处理grantsfind请求出错")
				return
			}
			sendResponse(c, grants, constant.TwokHeader, "Game is Trying to request grantsfind in 2K")
			return
		}
		// 502 BAD GATEWAY
		c.String(http.StatusBadGateway, "502 Bad Gateway")
		return
	}
	if strings.Contains(url, "content/1") && strings.Contains(c.Request.URL.RawQuery, "strings.get") {
		strings, err := processRequestBody(c, "stringsget.json")
		if err != nil {
			c.String(http.StatusInternalServerError, "处理stringsget请求出错")
			return
		}
		sendResponse(c, strings, constant.TwokHeader, "Game is Trying to request stringsget in 2K")
		return
	}
	// 验证服务器请求处理
	if strings.Contains(url, "auth.php") {
		authMulti, err := readTextFile("SteamLogin.json")
		if err != nil {
			c.String(http.StatusInternalServerError, "无法读取SteamLogin.json")
			return
		}
		sendResponse(c, authMulti, constant.MultiHeader, "Game is Trying to Go MultiPlayer.")
		return

	}
	if strings.Contains(url, "requestserver.php") {
		requestLB, err := readTextFile("requestserver.json")
		if err != nil {
			c.String(http.StatusInternalServerError, "无法读取requestserver.json")
			return
		}
		sendResponse(c, requestLB, constant.MultiHeader, "Game is Trying to Go MultiPlayer.")
		return
	}
	// 更新服务器请求处理
	if strings.Contains(url, "Production/252019.252019.0/build_config_signed.json") {
		buildConfig, err := readBlobFile("build_config_signed0.json")
		if err != nil {
			c.String(http.StatusInternalServerError, "无法读取build_config_signed0.json")
			return
		}
		sendResponse(c, buildConfig, constant.HotfixHeader, "Game is Trying to request mbclusions from updateserver.")
		return
	}
	if strings.Contains(url, "Production/252019.252019/micro_patch_version_signed.json") {
		microPatch, err := readBlobFile("micro_patch_version_signed.json")
		if err != nil {
			c.String(http.StatusInternalServerError, "无法读取micro_patch_version_signed.json")
			return
		}
		sendResponse(c, microPatch, constant.HotfixHeader, "Game is Trying to request micro version from updateserver.")
		return
	}
	if strings.Contains(url, "Production/252019.252019/micro_patch_252019.252019_31.pak") {
		fix34Patch, err := readBlobFile("micro_patch_252019.252019_31.pak")
		if err != nil {
			c.String(http.StatusInternalServerError, "无法读取micro_patch_252019.252019_31.pak")
			return
		}
		sendResponse(c, fix34Patch, constant.HotfixHeader, "Game is Trying to request the micro pak from updateserver.")
		return
	}
	if strings.Contains(url, "Production/252019.252019.31/build_config_signed.json") {
		buildConfig31, err := readBlobFile("build_config_signed_31.json")
		if err != nil {
			c.String(http.StatusInternalServerError, "无法读取build_config_signed_31.json")
			return
		}
		sendResponse(c, buildConfig31, constant.HotfixHeader, "Game is Trying to request another build_config from updateserver.")
		return
	} else {
		// 未识别的请求，返回通用响应
		utils.SUGAR.Warnw("[松子日志]", "message", "Maybe there are something I forgot to emulate.\nIt's okay as seems there are lots of them just need a response...")
		requestLB, err := readTextFile("requestserver.json")
		if err != nil {
			c.String(http.StatusInternalServerError, "无法读取requestserver.json")
			return
		}
		sendResponse(c, requestLB, constant.MultiHeader, url+" Something I forgot to emulate but seems okay")
		return
	}
}

//export StartServer
func StartServer() int {
	defer utils.LOGGER.Sync()
	utils.SUGAR.Info("Welcome to use EvolveReborn Software By Pinenut. 严禁倒卖!正版/盗版联机群：366237012")
	utils.SUGAR.Info("Lots of thanks:Nemirtingas,schmogmog,nemerod,kiagam")

	// 读取端口
	serverPort := config.Cfg.Server.Port

	// 设置Gin为release模式，减少日志输出
	gin.SetMode(gin.ReleaseMode)

	// 创建Gin路由器
	router := gin.New()

	// 使用中间件
	router.Use(gin.Recovery())

	router.GET("/favicon.ico", func(c *gin.Context) {
		// 创建16x16图像
		img := image.NewRGBA(image.Rect(0, 0, 16, 16))
		// 直接以PNG格式返回
		c.Header("Content-Type", "image/png")
		c.Status(http.StatusOK)
		err := png.Encode(c.Writer, img)
		if err != nil {
			c.AbortWithStatus(http.StatusInternalServerError)
		}
	})
	// 设置路由，处理所有请求
	router.NoRoute(handleRequest)

	utils.SUGAR.Infof("Server is running, Port = %d YOU CAN TEST WITH HTTPS.", serverPort)
	// 创建自签名HTTPS服务
	// caCertTLS, err := tls.LoadX509KeyPair(path.Join("EvolveCrack", "ca.pem"), path.Join("EvolveCrack", "ca.key"))
	certPEMBlock, _ := crackFiles.ReadFile("EvolveCrack/" + "ca.pem")
	keyPEMBlock, _ := crackFiles.ReadFile("EvolveCrack/" + "ca.key")
	caCertTLS, err := tls.X509KeyPair(certPEMBlock, keyPEMBlock)
	if err != nil {
		utils.SUGAR.Warnf("Failed to load CA certificate and key: %v", err)
		return 0
	}
	// 创建HTTPS服务器配置
	tlsConfig := &tls.Config{
		GetCertificate: func(hello *tls.ClientHelloInfo) (*tls.Certificate, error) {
			// 记录
			hostname, ip, err := utils.GetTLSHostNameAndIP(hello)
			if err != nil {
				return nil, err
			}
			serverCert, err := utils.GenerateCertificate(caCertTLS, hostname, ip)
			return &serverCert, nil
		},
	}
	// 以配置创建HTTP服务器，并使用Gin框架
	server := &http.Server{
		Addr:      fmt.Sprintf(":%d", serverPort),
		Handler:   router.Handler(),
		TLSConfig: tlsConfig,
	}
	//// 监听端口
	//if err := server.ListenAndServeTLS("", ""); err != nil {
	//	utils.SUGAR.Warnf("Server run failed as: %v, YOUR CRACK WILL WONT WORK", err)
	//}
	// 使用 goroutine 启动 HTTPS 服务器（非阻塞）
	errChan := make(chan error)
	go func() {
		errChan <- server.ListenAndServeTLS("", "")
	}()
	// 只要 ListenAndServe 没立即报错，就认为启动成功
	select {
	case err = <-errChan:
		if err != nil {
			utils.SUGAR.Warnf("Server run failed: %v", err)
			return 0 // 启动失败
		}
		return 1 // 理论上不会走到这里（ListenAndServe 阻塞直到出错）
	default:
		return 1 // 没有立即报错，认为启动成功
	}
}

func main() {
	StartServer()
	select {}
}
