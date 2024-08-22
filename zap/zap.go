// package zap
//	@author:	zht
//	@since:		2024/6/5
//	@desc:		//TODO

package ezap

import (
	"go.uber.org/zap"
	"go.uber.org/zap/zapcore"
	"gopkg.in/natefinch/lumberjack.v2"
	"os"
)

var LOGGER *zap.SugaredLogger
var ORIGINLOGGER *zap.Logger

func init() {
	// 配置 lumberjack 日志轮转
	lumberJackLogger := &lumberjack.Logger{
		Filename:   "logs/app.zap",
		MaxSize:    10,   // 每个日志文件的最大大小 (MB)
		MaxBackups: 3,    // 保留的旧日志文件的最大数量
		MaxAge:     28,   // 保留的旧日志文件的最大天数
		Compress:   true, // 是否压缩/归档旧日志文件
	}

	// 日志编码配置
	encoderConfig := zapcore.EncoderConfig{
		TimeKey:        "time",
		LevelKey:       "level",
		NameKey:        "logger",
		CallerKey:      "caller",
		MessageKey:     "msg",
		StacktraceKey:  "stacktrace",
		LineEnding:     zapcore.DefaultLineEnding,
		EncodeLevel:    zapcore.LowercaseLevelEncoder,
		EncodeTime:     zapcore.ISO8601TimeEncoder,
		EncodeDuration: zapcore.StringDurationEncoder,
		EncodeCaller:   zapcore.ShortCallerEncoder,
	}

	stdOutencoderConfig := zapcore.EncoderConfig{
		TimeKey:        "time",
		LevelKey:       "level",
		NameKey:        "logger",
		CallerKey:      "caller",
		MessageKey:     "msg",
		StacktraceKey:  "stacktrace",
		LineEnding:     zapcore.DefaultLineEnding,
		EncodeLevel:    zapcore.LowercaseColorLevelEncoder,
		EncodeTime:     zapcore.ISO8601TimeEncoder,
		EncodeDuration: zapcore.StringDurationEncoder,
		EncodeCaller:   zapcore.ShortCallerEncoder,
	}

	// 设置日志级别
	level := zap.NewAtomicLevel()
	level.SetLevel(zap.DebugLevel)

	// 输出到文件的核心配置
	fileCore := zapcore.NewCore(zapcore.NewJSONEncoder(encoderConfig), zapcore.AddSync(lumberJackLogger), level)
	consoleCore := zapcore.NewCore(zapcore.NewConsoleEncoder(stdOutencoderConfig), zapcore.AddSync(os.Stdout), level)

	core := zapcore.NewTee(consoleCore, fileCore)

	// 初始化日志记录器
	ORIGINLOGGER = zap.New(core, zap.AddCaller(), zap.AddCallerSkip(2))
	LOGGER = ORIGINLOGGER.Sugar()
}
