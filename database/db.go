package database

import (
	"awesomeProject5/model"
	"gorm.io/gorm/logger"
	"log"

	"awesomeProject5/zap"
	"gorm.io/driver/sqlite"
	"gorm.io/gorm"
)

var SQLITE *gorm.DB

// 初始化 SQLite 数据库
func _SQLiteDBInit(path string) (*gorm.DB, error) {
	// 创建自定义的 Logger 实现
	customLogger := logger.New(
		log.New(log.Writer(), "\r\n", log.LstdFlags),
		logger.Config{
			LogLevel: logger.Info, // 设置日志级别：Silent、Error、Warn、Info
		},
	)

	// 连接到 SQLite 数据库并配置 Logger
	db, err := gorm.Open(sqlite.Open(path), &gorm.Config{
		Logger: customLogger,
	})
	if err != nil {
		return nil, err
	}
	return db, nil
}

func init() {
	lite, err := _SQLiteDBInit("files.db")
	if err != nil {
		ezap.LOGGER.Error("Failed to init database!")
		log.Fatal("Database error")
	}
	SQLITE = lite
	// 自动迁移表结构
	err = SQLITE.AutoMigrate(&model.JsonFile{})
	if err != nil {
		ezap.LOGGER.Error("Failed to migrate database!")
		log.Fatal("migration error")
	}
}
