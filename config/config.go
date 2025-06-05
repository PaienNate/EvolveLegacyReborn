package config

import (
	"fmt"
	"log"
	"os"
	"path/filepath"

	"github.com/go-ini/ini"
)

var (
	// Cfg 全局配置（自动加载）
	Cfg *Config
)

type ServerConfig struct {
	Domain            string `ini:"server_domain"`
	Port              int    `ini:"server_port"`
	UseInternalServer bool   `ini:"use_internal_server"`
	InternalServerDll string `ini:"internal_server_dll"`
	ExternalIPList    string `ini:"external_ip_list"`
}

type SteamConfig struct {
	EmuSteam bool   `ini:"emu_steam"`
	DllPath  string `ini:"dll_path"`
}

type Config struct {
	Server ServerConfig `ini:"server"`
	Steam  SteamConfig  `ini:"steam"`
}

// init 自动加载当前目录下的 config.ini
func init() {
	var err error
	Cfg, err = loadConfig("EvolveLogging.ini")
	if err != nil {
		log.Fatalf("Failed to load config: %v", err)
	}
}

// loadConfig 加载并解析 INI 文件
func loadConfig(filename string) (*Config, error) {
	// 获取当前工作目录下的 config.ini
	path := filepath.Join(".", filename)
	if _, err := os.Stat(path); os.IsNotExist(err) {
		return nil, fmt.Errorf("config file not found: %s", path)
	}

	iniCfg, err := ini.Load(path)
	if err != nil {
		return nil, fmt.Errorf("failed to load config: %v", err)
	}

	var config Config
	if err := iniCfg.MapTo(&config); err != nil {
		return nil, fmt.Errorf("failed to map config: %v", err)
	}

	return &config, nil
}
