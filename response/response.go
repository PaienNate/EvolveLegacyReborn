package response

import (
	"net/http"
)

// GetDefaultResponse 保底专用
func GetDefaultResponse(w http.ResponseWriter) {
	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(http.StatusOK)
	_, err := w.Write([]byte(`{"message": "OK"}`))
	if err != nil {
		return
	}
}

func SendFakeResponse(w http.ResponseWriter, byteList []byte) {
	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(http.StatusOK)
	_, err := w.Write(byteList)
	if err != nil {
		return
	}
}

func SetCustomHeaders(w http.ResponseWriter, headers map[string]string) {
	for key, value := range headers {
		w.Header().Set(key, value)
	}
}
