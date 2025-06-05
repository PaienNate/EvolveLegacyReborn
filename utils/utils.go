package utils

import (
	"crypto/rand"
	"crypto/rsa"
	"crypto/tls"
	"crypto/x509"
	"crypto/x509/pkix"
	"encoding/pem"
	"fmt"
	"junk/config"
	"math/big"
	"net"
	"sort"
	"strings"
	"sync"
	"time"
)

var certCache sync.Map

// GenerateWildcardDomainIfNeeded
//
//	@描述: 根据域名的级别返回对应的域名或泛域名
//	@参数 domain
//	@返回值 string
func GenerateWildcardDomainIfNeeded(domain string) string {
	parts := strings.Split(domain, ".")
	if len(parts) >= 3 {
		return "*." + strings.Join(parts[1:], ".")
	}
	return domain
}

// GetTLSHostNameAndIP
//
//	@描述: 通过ClientHello信息获取访问域名或IP
//	@参数 hello
//	@返回值 name
//	@返回值 ip
//	@返回值 err
func GetTLSHostNameAndIP(hello *tls.ClientHelloInfo) (name string, ipList []string, err error) {
	domainHostName := hello.ServerName
	addr := hello.Conn.LocalAddr()
	// 切分出IP地址
	host, _, err := net.SplitHostPort(addr.String())
	configIPs := strings.Split(config.Cfg.Server.ExternalIPList, ",")
	// 补充配置文件配置的IP，往往该IP是服务器的真实IP
	ipList = append(configIPs, host)
	// 4. 去重（避免重复 IP）
	ipList = removeDuplicateIPs(ipList)
	// 如果用户根据域名访问
	if domainHostName != "" {
		domainHostName = GenerateWildcardDomainIfNeeded(domainHostName)
		SUGAR.Infof("发现域名和IP的组合，域名为：%s,IP为:%s\n", domainHostName, addr)
		return domainHostName, ipList, nil
	} else {
		if err != nil {
			fmt.Println("Failed to split host and port:", err)
			return "", ipList, err
		}
		SUGAR.Infof("仅发现IP，目标IP为：%s", host)
		return "", ipList, nil
	}
}

// removeDuplicateIPs 去除重复的 IP
func removeDuplicateIPs(ips []string) []string {
	seen := make(map[string]struct{})
	result := []string{}
	for _, ip := range ips {
		if _, ok := seen[ip]; !ok {
			seen[ip] = struct{}{}
			result = append(result, ip)
		}
	}
	return result
}

// GenerateCertificate
//
//	@描述: 通过传入的CA证书和私钥动态生成对应域名的证书
//	@参数 caCertTLS 使用tls.LoadX509KeyPair("ca.pem", "ca.key")获取的CA证书
//	@参数 hostname 域名
//	@参数 ip IP，如果用户使用IP访问，则签发IP证书
//	@返回值 tls.Certificate
//	@返回值 error
func GenerateCertificate(caCertTLS tls.Certificate, hostname string, ips []string) (tls.Certificate, error) {
	if hostname == "" {
		hostname = "firehomework.top"
	}

	// 1. 对 IP 列表排序，确保相同的 IP 集合生成相同的 cacheKey
	sortedIPs := make([]string, len(ips))
	copy(sortedIPs, ips)
	sort.Strings(sortedIPs)

	// 解析 IP 地址
	var ipAddresses []net.IP
	for _, ip := range ips {
		if parsedIP := net.ParseIP(ip); parsedIP != nil {
			ipAddresses = append(ipAddresses, parsedIP)
		}
	}

	// 生成缓存的 key，可以根据 hostname 和 ip 的组合来区分不同的证书
	cacheKey := hostname + "|" + strings.Join(sortedIPs, ",")

	// 检查缓存中是否已有该证书，有则直接返回
	if cert, ok := certCache.Load(cacheKey); ok {
		fmt.Println("发现已经签发的证书!")
		return cert.(tls.Certificate), nil
	}

	// 转换TLS证书为X509证书
	caCert, err := x509.ParseCertificate(caCertTLS.Certificate[0])
	if err != nil {
		return tls.Certificate{}, fmt.Errorf("解析CA证书失败: %v", err)
	}

	// 读取CA证书私钥用于签发
	privateKey, ok := caCertTLS.PrivateKey.(*rsa.PrivateKey)
	if !ok {
		return tls.Certificate{}, fmt.Errorf("CA私钥不是RSA私钥")
	}

	// 计算证书有效期，确保：
	// 1. 从当前时间开始
	// 2. 不超过浏览器接受的 398 天（约 13 个月）
	// 3. 不超过 CA 证书的有效期
	notBefore := time.Now()
	if notBefore.Before(caCert.NotBefore) {
		notBefore = caCert.NotBefore
	}

	maxBrowserValidity := notBefore.Add(398 * 24 * time.Hour) // 浏览器最大接受 398 天
	notAfter := maxBrowserValidity

	// 如果 CA 证书的有效期更短，则使用 CA 的有效期
	if caCert.NotAfter.Before(notAfter) {
		notAfter = caCert.NotAfter
	}

	// 创建证书模板
	template := x509.Certificate{
		SerialNumber: big.NewInt(1),
		Subject: pkix.Name{
			Country:      []string{"CN"},
			Organization: []string{"Pinenutn Fixed"},
			CommonName:   hostname,
		},
		DNSNames:              []string{hostname},
		IPAddresses:           ipAddresses,
		NotBefore:             notBefore,
		NotAfter:              notAfter,
		KeyUsage:              x509.KeyUsageKeyEncipherment | x509.KeyUsageDigitalSignature,
		ExtKeyUsage:           []x509.ExtKeyUsage{x509.ExtKeyUsageServerAuth},
		BasicConstraintsValid: true,
	}

	// 生成证书
	certificateDER, err := x509.CreateCertificate(rand.Reader, &template, caCert, &privateKey.PublicKey, privateKey)
	if err != nil {
		return tls.Certificate{}, fmt.Errorf("创建证书失败: %v", err)
	}

	// 将证书用于服务器
	certPEM := pem.EncodeToMemory(&pem.Block{Type: "CERTIFICATE", Bytes: certificateDER})
	keyPEM := pem.EncodeToMemory(&pem.Block{Type: "RSA PRIVATE KEY", Bytes: x509.MarshalPKCS1PrivateKey(privateKey)})
	pair, err := tls.X509KeyPair(certPEM, keyPEM)
	if err != nil {
		return tls.Certificate{}, fmt.Errorf("创建X509密钥对失败: %v", err)
	}

	certCache.Store(cacheKey, pair)
	return pair, nil
}
