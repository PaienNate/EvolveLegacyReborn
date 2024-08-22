package strstr

import "time"

var TwoKList = []string{"doorman/1", "telemetry/1", "sessions/1", "news/1", "storage/1", "sso/1", "stats/1", "entitlements/1", "content/1"}

var MultiPlayerList = []string{"auth.php", "requestserver.php"}

var HotFixList = []string{"Production/252019.252019.0/build_config_signed.json", "Production/252019.252019/micro_patch_version_signed.json", "Production/252019.252019/micro_patch_252019.252019_31.pak", "Production/252019.252019.31/build_config_signed.json"}

// SMALL CHANGE
var HotFixMap = map[string]string{
	"Production/252019.252019.0/build_config_signed.json":       "build_config_signed_0.json",
	"Production/252019.252019/micro_patch_version_signed.json":  "micro_patch_version_signed.json",
	"Production/252019.252019/micro_patch_252019.252019_31.pak": "micro_patch_252019.252019_31.pak",
	"Production/252019.252019.31/build_config_signed.json":      "build_config_signed_31.json",
}

var TwoKHeader = map[string]string{
	"Server":       "Pinenut-Crack-Go/1.2",
	"Date":         time.Now().UTC().Format("Mon, 02 Jan 2006 15:04:05 GMT"),
	"Content-Type": "application/json",
	"Connection":   "close",
}

var MultiHeader = map[string]string{
	"Content-Type":  "application/json,charset=utf-8",
	"Cache-Control": "no-cache, no-store, must-revalidate",
	"Connection":    "keep-alive",
	"Date":          time.Now().UTC().Format("Mon, 02 Jan 2006 15:04:05 GMT"),
	"Expires":       "0",
	"Server":        "Pinenut-Crack-Go/1.2",
	"Pragma":        "no-cache",
	"X-Powered-By":  "EvolveLegacyReborn/5.3.29",
}

var HotfixHeader = map[string]string{
	"Content-Type":  "application/octet-stream",
	"Connection":    "keep-alive",
	"Last-Modified": "Tue, 19 Jan 2016 17:29:48 GMT",
	"Accept-Ranges": "bytes",
	"Server":        "Pinenut-Crack-Go/1.2",
	"Date":          time.Now().Format("Mon, 02 Jan 2006 15:04:05 MST"),
	"ETag":          "\"justKidding123\"",
	"X-Cache":       "Memory from EvolveLegacyReborn",
	"Via":           "1.1 giggle.cloudfront.net (CloudFront)",
	"X-Amz-Cf-Pop":  "MAGIC-POP",
	"X-Amz-Cf-Id":   "abracadabra987654321",
}
