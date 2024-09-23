package main

/*
#include <stdlib.h>

typedef struct {
    char* mode;
    int port;
    char* cacert;
    char* cakey;
} ServerConfig;

typedef struct {
    char* hookmode;
    int randomport;  // 使用int来表示bool值
    char* recordmode;
    char* steamid;
    char* fromsteam;
} GeneralConfig;

typedef struct {
    ServerConfig server;
    GeneralConfig general;
} Config;
*/
import "C"
import (
	"EvolveLegacyReborn/config"
	"EvolveLegacyReborn/service"
	"EvolveLegacyReborn/strstr"
	"EvolveLegacyReborn/utils"
	ezap "EvolveLegacyReborn/zap"
	"crypto/tls"
	"fmt"
	"github.com/go-chi/chi/v5"
	"github.com/go-chi/chi/v5/middleware"
	_ "github.com/jmoiron/sqlx"
	_ "github.com/mattn/go-sqlite3"
	"moul.io/chizap"
	"net/http"
	"unsafe"
)

//export StartServer
func StartServer() {
	// 创建新的Chi路由器
	ezap.LOGGER.Info("启动模拟服务器中")
	r := chi.NewRouter()
	r.Use(chizap.New(ezap.ORIGINLOGGER, &chizap.Opts{
		WithReferer:   true,
		WithUserAgent: true,
	}))
	// 添加一些常见的中间件（类似于Gin的默认中间件）
	r.Use(middleware.Recoverer) // 从panic中恢复
	r.Use(middleware.RequestID) // 给每个请求添加唯一的ID

	// 方便用户的测试部分
	r.Get("/ping", func(w http.ResponseWriter, r *http.Request) {
		w.Header().Set("Content-Type", "application/json")
		w.WriteHeader(http.StatusOK)
		_, err := w.Write([]byte(`{"message": "pong"}`))
		if err != nil {
			return
		}
	})
	// TODO：绑定一个页面用来启动N2N
	// 其他部分绑定，全部处理方案
	r.Handle("/*", http.HandlerFunc(handleRequest))
	newTLSMitmServer(r)
}

// boolToInt converts a Go bool to C int (0 or 1)
func boolToInt(b bool) C.int {
	if b {
		return 1
	}
	return 0
}

//export PrintDebug
func PrintDebug(format *C.char) {
	formatStr := C.GoString(format)
	ezap.LOGGER.Infof(formatStr)
}

//export GetConfig
func GetConfig() *C.Config {
	cConfig := (*C.Config)(C.malloc(C.size_t(unsafe.Sizeof(C.Config{}))))
	// 填充ServerConfig
	cConfig.server.mode = C.CString(config.CONFIG.Server.Mode)
	cConfig.server.port = C.int(config.CONFIG.Server.Port)
	cConfig.server.cacert = C.CString(config.CONFIG.Server.CACert)
	cConfig.server.cakey = C.CString(config.CONFIG.Server.CAKey)
	// 填充GeneralConfig
	cConfig.general.hookmode = C.CString(config.CONFIG.General.HookMode)
	cConfig.general.randomport = C.int(boolToInt(config.CONFIG.General.RandomPort))
	cConfig.general.recordmode = C.CString(config.CONFIG.General.RecordMode)
	cConfig.general.steamid = C.CString(config.CONFIG.General.SteamID)
	cConfig.general.fromsteam = C.CString(config.CONFIG.General.FromSteam)
	return cConfig
}

//export FreeConfig
func FreeConfig(cConfig *C.Config) {
	// 释放ServerConfig中的字符串
	C.free(unsafe.Pointer(cConfig.server.mode))
	C.free(unsafe.Pointer(cConfig.server.cacert))
	C.free(unsafe.Pointer(cConfig.server.cakey))

	// 释放GeneralConfig中的字符串
	C.free(unsafe.Pointer(cConfig.general.hookmode))
	C.free(unsafe.Pointer(cConfig.general.recordmode))
	C.free(unsafe.Pointer(cConfig.general.steamid))
	C.free(unsafe.Pointer(cConfig.general.fromsteam))

	// 释放整个结构体
	C.free(unsafe.Pointer(cConfig))
}

// 处理请求
func handleRequest(w http.ResponseWriter, r *http.Request) {
	// 获取请求URL，尝试匹配2K处理机制
	path := r.URL.Path
	result, ok := utils.FindFirstMatch(path, strstr.TwoKList)
	if ok {
		// 使用2K处理器进行处理
		service.HandleTwoKService(result, w, r)
		return
	}
	result, ok = utils.FindFirstMatch(path, strstr.MultiPlayerList)
	if ok {
		// 使用多人游戏处理器进行处理
		service.HandleMultiService(result, w, r)
		return
	}
	result, ok = utils.FindFirstMatch(path, strstr.HotFixList)
	if ok {
		// 使用热补丁处理进行处理
		service.HandleHotFixService(result, w, r)
		return
	}
	ezap.LOGGER.Info("REQUEST NOT SUPPORTED! USE DEFAULT REQUEST...")
	// 否则，写默认的处理
	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(http.StatusOK)
	w.Write([]byte(`{"message": "OK"}`))
}

func newTLSMitmServer(g *chi.Mux) {
	// 读取HTTPS-CA证书
	testConfig := config.CONFIG
	caCertTLS, _ := tls.LoadX509KeyPair(testConfig.Server.CACert, testConfig.Server.CAKey)
	// 创建HTTPS服务器配置
	tlsConfig := &tls.Config{
		GetCertificate: func(hello *tls.ClientHelloInfo) (*tls.Certificate, error) {
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
		Addr:      fmt.Sprintf(":%d", testConfig.Server.Port),
		Handler:   g,
		TLSConfig: tlsConfig,
	}
	err := server.ListenAndServeTLS("", "")
	if err != nil {
		ezap.LOGGER.Errorf("服务器启动出现问题，问题为:%s", err)
		return
	}
}

func main() {
}
