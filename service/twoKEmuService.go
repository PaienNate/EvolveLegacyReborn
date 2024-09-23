package service

import (
	"EvolveLegacyReborn/config"
	"EvolveLegacyReborn/database"
	"EvolveLegacyReborn/model"
	"EvolveLegacyReborn/response"
	"EvolveLegacyReborn/strstr"
	ezap "EvolveLegacyReborn/zap"
	"encoding/json"
	"errors"
	"fmt"
	"github.com/tidwall/gjson"
	"io"
	"net/http"
	"strings"
	"time"
)

// 读取请求头，区分是否是EVOLVE2
func HandleTwoKService(pattern string, w http.ResponseWriter, r *http.Request) {
	ResponseEvolveLegacy(pattern, w, r)
}

func ResponseEvolveLegacy(pattern string, w http.ResponseWriter, r *http.Request) {
	switch pattern {
	case "news/1":
		fallthrough
	case "doorman/1":
		fallthrough
	case "sessions/1":
		fallthrough
	case "stats/1":
		ExecJustTimeOrContext(pattern, w, r)
	case "entitlements/1":
		ExecEntitlements(pattern, w, r)
	case "telemetry/1":
		pattern = "telemetryconfiggenerate"
		ExecJustTimeOrContext(pattern, w, r)
	case "content/1":
		pattern = "stringsget"
		ExecJustTimeOrContext(pattern, w, r)
	case "sso/1":
		pattern = "applogon"
		ExecJustTimeOrContext(pattern, w, r)
	case "storage/1":
		pattern = "iteminsert"
		ExecJustTimeOrContext(pattern, w, r)
	}
}

func ExecEntitlements(pattern string, w http.ResponseWriter, r *http.Request) {
	// 此种情况下，应该解析r的body,并读取它的参数，将参数作为分析。
	requestBody, err := parseRequestBodyToGJson(r)

	if err != nil {
		ezap.LOGGER.Error("ERROR WHEN READING REQUEST")
		response.GetDefaultResponse(w)
		return
	}
	// 获取它的执行信息
	if !requestBody.Get("header.action").Exists() {
		ezap.LOGGER.Error("NO ACTION,ILLEGAL")
		response.GetDefaultResponse(w)
		return
	}
	action := requestBody.Get("header.action").String()
	switch action {
	case "steamApi.user.checkAppOwnership":
		pattern = "checkAppOwnership"
		ExecEngineWithBody(pattern, w, requestBody, SteamIDChangeStr)
	case "entitlementDefs.getFirstPartyMapping":
		pattern = "getFirstPartyMapping"
	case "grants.find":
		pattern = "grantsfind"
	default:
		// 随便返回点什么就是了，反正是豹错
		ezap.LOGGER.Error("DEFAULT")
		response.GetDefaultResponse(w)
		return
	}

	ExecEngineWithBody(pattern, w, requestBody, GenerateGeneralStr)
}

// 针对不需要处理的进行简单的封装
func ExecJustTimeOrContext(pattern string, w http.ResponseWriter, r *http.Request) {
	ExecEngine(pattern, w, r, GenerateGeneralStr)
}

// 定义一个套壳的处理
type HandleReturnStr func(responseStr string) string

func SteamIDChangeStr(responseStr string) string {
	steamid := "76561101839859666"
	if config.CONFIG.General.SteamID != "0" && config.CONFIG.General.SteamID != "" {
		steamid = config.CONFIG.General.SteamID
	}
	responseStr = strings.Replace(responseStr, "\"ownersteamid\": \"0\"", fmt.Sprintf("\"ownersteamid\": \"%s\"", steamid), -1)
	return GenerateGeneralStr(responseStr)
}

// 非通用的部分
func GenerateGeneralStr(responseStr string) string {
	var nowTime = time.Now().Unix()
	responseStr = strings.Replace(responseStr, "\"serverTime\": 0", fmt.Sprintf("\"serverTime\": %d", nowTime), -1)
	responseStr = strings.Replace(responseStr, "\"createdOn\": 0", fmt.Sprintf("\"createdOn\": %d", nowTime), -1)
	return responseStr
}

// 不经过转换的引擎，可以直接使用
func ExecEngineWithBody(pattern string, w http.ResponseWriter, r gjson.Result, handleReturnStr HandleReturnStr) {
	var appContext int64
	// SQLITE 取 响应体
	pattern = strings.Replace(pattern, "/1", "", 1) + ".json"
	var data [][]byte
	err := database.SQLITE.
		Model(&model.JsonFile{}).
		Select("binary_data").
		Where("name = ? and tag = ?", pattern, "twok").
		Scan(&data).Error
	if err != nil || len(data) == 0 {
		ezap.LOGGER.Error("ERROR WHEN GETTING RESPONSE WHEN READING SQL")
		response.GetDefaultResponse(w)
		return
	}
	// SQLITE 取 响应体 END

	// 转换为gjson对象
	if !gjson.ValidBytes(data[0]) {
		ezap.LOGGER.Error("ERROR PARSE THE REQUEST DATA")
		response.GetDefaultResponse(w)
		return
	}
	responseRaw := gjson.ParseBytes(data[0])
	// 转换为gjson对象 END
	// 读取请求体，获取appContext参数
	appContext = r.Get("header.appContext").Int()
	// 读取请求体，获取appContext参数 END

	responseStr := responseRaw.String()
	responseStr = handleReturnStr(responseStr)
	// 强制时间替换 END
	// 转换为Go结构体，并进行对应的赋值
	trulyResponse := gjson.Parse(responseStr).Value().(map[string]interface{})
	header, ok := trulyResponse["header"].(map[string]interface{})
	if !ok {
		ezap.LOGGER.Error("Header is not a valid map")
		response.GetDefaultResponse(w)
		return
	}
	header["appContext"] = appContext
	// 转换为Go结构体，并进行对应的赋值 END
	// 序列化并发送
	jsonData, err := json.Marshal(trulyResponse)
	if err != nil {
		ezap.LOGGER.Error("Error marshalling JSON:", err)
		response.GetDefaultResponse(w)
		return
	}

	response.SetCustomHeaders(w, strstr.TwoKHeader)
	response.SendFakeResponse(w, jsonData)
	// 序列化并发送 END
}

// 不经过转换的引擎，可以直接使用
func ExecEngine(pattern string, w http.ResponseWriter, r *http.Request, handleReturnStr HandleReturnStr) {
	requestBody, err := parseRequestBodyToGJson(r)
	if err != nil {
		fmt.Println("失败!")
	}
	ExecEngineWithBody(pattern, w, requestBody, handleReturnStr)
}

// utils
// 转换为gjson对象
func parseByteDataToGJson(data []byte) (gjson.Result, error) {
	if !gjson.ValidBytes(data) {
		return gjson.Result{}, errors.New("invalid JSON data")
	}
	return gjson.ParseBytes(data), nil
}

func parseRequestBodyToGJson(r *http.Request) (gjson.Result, error) {
	defer r.Body.Close()
	all, err := io.ReadAll(r.Body)
	if err != nil {
		return gjson.Result{}, errors.New("failed to get response body")
	}
	return parseByteDataToGJson(all)
}
