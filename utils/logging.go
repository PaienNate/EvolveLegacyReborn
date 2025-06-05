package utils

import (
	"github.com/natefinch/lumberjack"
	"go.uber.org/zap"
	"go.uber.org/zap/zapcore"
	"os"
)

// 全局变量
var (
	LOGGER *zap.Logger
	SUGAR  *zap.SugaredLogger
)

// 初始化日志
func init() {
	// 日志文件的路径
	path := "BYPINENUT.log"

	// 使用lumberjack进行日志文件轮转配置
	lumlog := &lumberjack.Logger{
		Filename:   path, // 日志文件的名称和路径
		MaxSize:    10,   // 每个日志文件最大10MB
		MaxBackups: 3,    // 最多保留3个旧日志文件
		MaxAge:     7,    // 日志文件保存7天
	}
	encoderConfig := zap.NewProductionEncoderConfig()
	encoderConfig.EncodeTime = zapcore.ISO8601TimeEncoder
	encoderConfig.EncodeLevel = zapcore.CapitalLevelEncoder
	encoder := zapcore.NewConsoleEncoder(encoderConfig)
	// 文件的
	fileLogCore := zapcore.NewCore(encoder, zapcore.AddSync(lumlog), zapcore.DebugLevel)
	// 创建控制台的日志编码器
	consoleCoreRaw := zapcore.NewCore(encoder, zapcore.AddSync(os.Stdout), zapcore.DebugLevel)
	LOGGER = zap.New(zapcore.NewTee(fileLogCore, consoleCoreRaw))
	SUGAR = LOGGER.Sugar()
}
