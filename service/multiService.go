package service

import (
	"awesomeProject5/database"
	"awesomeProject5/model"
	"awesomeProject5/response"
	"awesomeProject5/strstr"
	ezap "awesomeProject5/zap"
	"net/http"
)

// 这个不需要处理，只需要直接读，读出来就行了
func HandleMultiService(pattern string, w http.ResponseWriter, r *http.Request) {
	var data [][]byte
	err := database.SQLITE.Model(&model.JsonFile{}).Select("binary_data").Where("name = ? and tag = ?", pattern, "multiplayer").Scan(&data).Error
	if err != nil || len(data) == 0 {
		ezap.LOGGER.Error("ERROR WHEN GETTING RESPONSE WHEN READING SQL")
		response.GetDefaultResponse(w)
		return
	}
	response.SetCustomHeaders(w, strstr.HotfixHeader)
	response.SendFakeResponse(w, data[0])
}