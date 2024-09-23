package service

import (
	"EvolveLegacyReborn/database"
	"EvolveLegacyReborn/model"
	"EvolveLegacyReborn/response"
	"EvolveLegacyReborn/strstr"
	ezap "EvolveLegacyReborn/zap"
	"net/http"
)

// HandleHotFixService 原理完全一致 同 multiService
func HandleHotFixService(pattern string, w http.ResponseWriter, r *http.Request) {
	pattern = strstr.HotFixMap[pattern]
	var data [][]byte
	err := database.SQLITE.Model(&model.JsonFile{}).Select("binary_data").Where("name = ? and tag = ?", pattern, "hotfix").Scan(&data).Error
	if err != nil || len(data) == 0 {
		ezap.LOGGER.Error("ERROR WHEN GETTING RESPONSE WHEN READING SQL")
		response.GetDefaultResponse(w)
		return
	}
	// 否则直接写对应的byte即可
	response.SetCustomHeaders(w, strstr.HotfixHeader)
	response.SendFakeResponse(w, data[0])
}
