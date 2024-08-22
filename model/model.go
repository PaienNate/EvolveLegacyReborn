package model

type JsonFile struct {
	ID         uint   `gorm:"primaryKey;autoIncrement"`
	Name       string `gorm:"column:name"`
	Tag        string `gorm:"column:tag;default:'default'"`
	BinaryData []byte `gorm:"column:binary_data"`
}

func (JsonFile) TableName() string {
	return "jsonfile"
}
