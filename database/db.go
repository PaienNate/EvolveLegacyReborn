//go:build cgo
// +build cgo

package database

import (
	ezap "awesomeProject5/zap"
	"github.com/jmoiron/sqlx"
	_ "github.com/mattn/go-sqlite3" // sqlite3 driver
)

var SQLITE *sqlx.DB

// 启动Init
func _SQLiteDBInit(path string) (*sqlx.DB, error) {
	db, err := sqlx.Open("sqlite3", path)
	if err != nil {
		panic(err)
	}
	return db, err
}

func init() {
	lite, err := _SQLiteDBInit("data.db")
	if err != nil {
		ezap.LOGGER.Error("Failed to init database!")
		panic("database error")
	}
	SQLITE = lite
}
