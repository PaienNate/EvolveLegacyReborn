package config

import (
	"fmt"
	"github.com/fsnotify/fsnotify"
	"github.com/spf13/viper"
)

var CONFIG *Config

type Config struct {
	Server  ServerConfig  `json:"server" yaml:"server"`
	General GeneralConfig `json:"general" yaml:"general"`
}

type ServerConfig struct {
	Mode   string `json:"mode" yaml:"mode"`
	Port   int    `json:"port" yaml:"port"`
	CACert string `json:"cacert" yaml:"cacert"`
	CAKey  string `json:"cakey" yaml:"cakey"`
}

type GeneralConfig struct {
	HookMode   string `json:"hookmode" yaml:"hookmode"`
	RandomPort bool   `json:"randomport" yaml:"randomport"`
	// Unused
	RecordMode string `json:"recordmode" yaml:"recordmode"`
	SteamID    string `json:"steamid" yaml:"steamid"`
	FromSteam  string `json:"fromsteam" yaml:"fromsteam"`
}

func init() {
	v := viper.New()
	v.SetConfigFile("config/config.yaml")
	v.SetConfigType("yaml")
	err := v.ReadInConfig()
	if err != nil {
		panic(fmt.Errorf("Fatal error config file: %s \n", err))
	}
	v.WatchConfig()
	v.OnConfigChange(func(e fsnotify.Event) {
		fmt.Println("config file changed:", e.Name)
		if err = v.Unmarshal(&CONFIG); err != nil {
		}
	})
	if err = v.Unmarshal(&CONFIG); err != nil {
	}
}
