package config

import (
	"fmt"
	"github.com/fsnotify/fsnotify"
	"github.com/spf13/viper"
)

var CONFIG *Config

type Config struct {
	Server ServerConfig `json:"server" yaml:"server"`
}

type ServerConfig struct {
	Mode   string `json:"mode" yaml:"mode"`
	Port   int    `json:"port" yaml:"port"`
	CACert string `json:"cacert" yaml:"cacert"`
	CAKey  string `json:"cakey" yaml:"cakey"`
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
