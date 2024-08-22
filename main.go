package main

import "C"
import (
	"awesomeProject5/config"
	"awesomeProject5/service"
	"awesomeProject5/strstr"
	"awesomeProject5/utils"
	ezap "awesomeProject5/zap"
	"crypto/tls"
	"fmt"
	"github.com/go-chi/chi/v5"
	"github.com/go-chi/chi/v5/middleware"
	_ "github.com/jmoiron/sqlx"
	_ "github.com/mattn/go-sqlite3"
	"moul.io/chizap"
	"net/http"
)

//export StartServer
func StartServer() {
	// 创建新的Chi路由器
	r := chi.NewRouter()
	r.Use(chizap.New(ezap.ORIGINLOGGER, &chizap.Opts{
		WithReferer:   true,
		WithUserAgent: true,
	}))
	// 添加一些常见的中间件（类似于Gin的默认中间件）
	r.Use(middleware.Recoverer) // 从panic中恢复
	r.Use(middleware.RequestID) // 给每个请求添加唯一的ID

	// 简单测试部分
	r.Get("/ping", func(w http.ResponseWriter, r *http.Request) {
		w.Header().Set("Content-Type", "application/json")
		w.WriteHeader(http.StatusOK)
		w.Write([]byte(`{"message": "pong"}`))
	})
	// 其他部分绑定，全部处理方案
	r.Handle("/*", http.HandlerFunc(handleRequest))

	// 启动服务器
	newTLSMitmServer(r)
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
	fmt.Println(result)
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
		fmt.Println(err)
		return
	}
}

func main() {
	StartServer()
}
