package constant

import (
	"net/http"
	"time"
)

var Server = "Pinenut-Crack-EvolveLegacyReborn/V1.2"

// 2K Header
var TwokHeader = map[string]string{
	"Server":     Server,
	"Connection": "close",
	"P3P":        "CP=\"CAO PSA OUR\"",
}

// Multiplayer Header
var MultiHeader = map[string]string{
	"Content-Type":  "application/json,charset=utf-8",
	"Cache-Control": "no-cache, no-store, must-revalidate",
	"Connection":    "keep-alive",
	"Expires":       "0",
	"Server":        Server,
	"Pragma":        "no-cache",
}

var HotfixHeader = map[string]string{
	"Content-Type": "application/octet-stream",
	"Connection":   "keep-alive",
	"Date":         time.Now().UTC().Format(http.TimeFormat),
}
