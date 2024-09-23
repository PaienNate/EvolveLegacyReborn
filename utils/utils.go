package utils

import (
	ezap "EvolveLegacyReborn/zap"
	"crypto/rand"
	"crypto/rsa"
	"crypto/tls"
	"crypto/x509"
	"crypto/x509/pkix"
	"encoding/pem"
	"fmt"
	"math/big"
	"net"
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
func GetTLSHostNameAndIP(hello *tls.ClientHelloInfo) (name, ip string, err error) {
	domainHostName := hello.ServerName
	addr := hello.Conn.LocalAddr()
	host, _, err := net.SplitHostPort(addr.String())
	// 如果用户根据域名访问
	if domainHostName != "" {
		domainHostName = GenerateWildcardDomainIfNeeded(domainHostName)
		ezap.LOGGER.Infof("发现域名和IP的组合，域名为：%s,IP为:%s\n", domainHostName, addr)
		return domainHostName, host, nil
	} else {
		if err != nil {
			fmt.Println("Failed to split host and port:", err)
			return "", "", err
		}
		ezap.LOGGER.Infof("仅发现IP，目标IP为：%s", host)
		return "", host, nil
	}
}

// generateCertificate
//
//	@描述: 通过传入的CA证书和私钥动态生成对应域名的证书
//	@参数 caCertTLS 使用tls.LoadX509KeyPair("ca.pem", "ca.key")获取的CA证书
//	@参数 hostname 域名
//	@参数 ip IP，如果用户使用IP访问，则签发IP证书
//	@返回值 tls.Certificate
//	@返回值 error
func GenerateCertificate(caCertTLS tls.Certificate, hostname, ip string) (tls.Certificate, error) {
	if hostname == "" {
		hostname = "firehomework.top"
	}
	// 生成缓存的 key，可以根据 hostname 和 ip 的组合来区分不同的证书
	cacheKey := hostname + "|" + ip
	// 检查缓存中是否已有该证书，有则直接返回。由于游玩者不可能一次开100年，所以证书必不会过期。
	if cert, ok := certCache.Load(cacheKey); ok {
		fmt.Println("发现已经签发的证书!")
		return cert.(tls.Certificate), nil
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
		IPAddresses:           []net.IP{net.ParseIP(ip)},
		NotBefore:             time.Now(),
		NotAfter:              time.Now().Add(365 * 24 * time.Hour), // 有效期一百年
		KeyUsage:              x509.KeyUsageKeyEncipherment | x509.KeyUsageDigitalSignature,
		ExtKeyUsage:           []x509.ExtKeyUsage{x509.ExtKeyUsageServerAuth},
		BasicConstraintsValid: true,
	}
	// 读取CA证书私钥用于签发
	privateKey, ok := caCertTLS.PrivateKey.(*rsa.PrivateKey)
	if !ok {
		ezap.LOGGER.Info("Private key is not an RSA private key")
	}
	// 转换TLS证书为X509证书
	caCert, err := x509.ParseCertificate(caCertTLS.Certificate[0])
	if err != nil {
		fmt.Println(err)
	}
	// 生成证书
	certificateDER, err := x509.CreateCertificate(rand.Reader, &template, caCert, &privateKey.PublicKey, privateKey)
	// 将证书用于服务器
	certPEM := pem.EncodeToMemory(&pem.Block{Type: "CERTIFICATE", Bytes: certificateDER})
	keyPEM := pem.EncodeToMemory(&pem.Block{Type: "RSA PRIVATE KEY", Bytes: x509.MarshalPKCS1PrivateKey(privateKey)})
	pair, err := tls.X509KeyPair(certPEM, keyPEM)
	if err != nil {
		return tls.Certificate{}, err
	}
	certCache.Store(cacheKey, pair)
	return pair, err
}

// FindFirstMatch 检查路径是否包含指定的字符串，并返回第一个匹配的字符串
func FindFirstMatch(path string, substrings []string) (string, bool) {
	for _, substring := range substrings {
		if strings.Contains(path, substring) {
			return substring, true
		}
	}
	return "", false
}

// 因为没有那么大的需求，下面的不要了

// isCertificateExpired
//
//	@描述: 检查证书是否过期（100年的证书真的会过期吗）
//	@参数 cert
//	@返回值 bool
//func isCertificateExpired(cert *tls.Certificate) bool {
//	if cert == nil {
//		return true
//	}
//	x509Cert, err := x509.ParseCertificate(cert.Certificate[0])
//	if err != nil {
//		ezap.LOGGER.Errorf("failed to parse certificate: %v", err)
//		return true
//	}
//	return time.Now().After(x509Cert.NotAfter)
//}

//func SaveCertificate(db *bolt.DB, domain string, certData []byte) error {
//	bucket := []byte("cert")
//	err := db.Update(func(tx *bolt.Tx) error {
//		b := tx.Bucket(bucket)
//		if b == nil {
//			return fmt.Errorf("未找到桶")
//		}
//
//		if err := b.Put([]byte(domain), certData); err != nil {
//			return fmt.Errorf("保存证书时出错: %v", err)
//		}
//		return nil
//	})
//	if err != nil {
//		return fmt.Errorf("更新数据库时出错: %v", err)
//	}
//	return nil
//}
//
//// GetCertificate 从数据库中检索证书数据
//func GetCertificate(db *bolt.DB, domain string) ([]byte, error) {
//	bucket := []byte("cert")
//	var certData []byte
//	err := db.View(func(tx *bolt.Tx) error {
//		b := tx.Bucket(bucket)
//		if b == nil {
//			return fmt.Errorf("未找到桶")
//		}
//
//		certData = b.Get([]byte(domain))
//		if certData == nil {
//			return fmt.Errorf("未找到域名 %s 的证书", domain)
//		}
//		return nil
//	})
//	if err != nil {
//		return nil, fmt.Errorf("检索证书时出错: %v", err)
//	}
//	return certData, nil
//}
