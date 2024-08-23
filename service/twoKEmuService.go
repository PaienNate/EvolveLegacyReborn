package service

import (
	"awesomeProject5/database"
	"awesomeProject5/model"
	"awesomeProject5/response"
	"awesomeProject5/strstr"
	ezap "awesomeProject5/zap"
	"encoding/json"
	"errors"
	"fmt"
	"github.com/tidwall/gjson"
	"io"
	"net/http"
	"strings"
	"time"
)

// 这个不需要处理，只需要直接读，读出来就行了
func HandleTwoKService(pattern string, w http.ResponseWriter, r *http.Request) {
	// 2K的处理逻辑，需要分情况讨论
	// fallthrough用来保证相同处理逻辑的应该到一起
	// 这三个是相同的处理逻辑：读取信息，直接返回，不需要处理任何参数
	switch pattern {
	case "news/1":
		fallthrough
	case "storage/1":
		fallthrough
	case "doorman/1":
		fallthrough
	case "sessions/1":
		fallthrough
	case "sso/1":
		fallthrough
	case "stats/1":
		ExecJustTimeOrContext(pattern, w, r)
	case "entitlements/1":

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
		// EVOLVE1
		case ""
	}

}

// 针对不需要处理的进行简单的封装
func ExecJustTimeOrContext(pattern string, w http.ResponseWriter, r *http.Request) {
	pattern = strings.Replace(pattern, "/1", "", 1) + ".json"
	ExecEngine(pattern, w, r)
}

// 不经过转换的引擎，可以直接使用
func ExecEngine(pattern string, w http.ResponseWriter, r *http.Request) {
	var nowTime = time.Now().Unix()
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
	all, err := io.ReadAll(r.Body)
	if err != nil {
		ezap.LOGGER.Error("ERROR READ THE REQUEST DATA")
		response.GetDefaultResponse(w)
		return
	}
	if !gjson.ValidBytes(all) {
		ezap.LOGGER.Error("ERROR PARSE THE REQUEST DATA")
		response.GetDefaultResponse(w)
		return
	}
	appContext = gjson.ParseBytes(all).Get("header.appContext").Int()
	// 读取请求体，获取appContext参数 END

	// TODO：是否有更好的解决策略？强制时间替换 serverTime createdOn
	responseStr := responseRaw.String()
	responseStr = strings.Replace(responseStr, "\"serverTime\": 0", fmt.Sprintf("\"serverTime\": %d", nowTime), -1)
	responseStr = strings.Replace(responseStr, "\"createdOn\": 0", fmt.Sprintf("\"createdOn\": %d", nowTime), -1)
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

// utils
// 转换为gjson对象
func parseByteDataToGJson(data []byte) (gjson.Result, error) {
	if !gjson.ValidBytes(data) {
		return gjson.Result{}, errors.New("invalid JSON data")
	}
	return gjson.ParseBytes(data), nil
}

func parseRequestBodyToGJson(r *http.Request) (gjson.Result, error) {
	all, err := io.ReadAll(r.Body)
	if err != nil {
		return gjson.Result{}, errors.New("failed to get response body")
	}
	return parseByteDataToGJson(all)
}
