package kicad

import "fmt"

type AlternatePin struct {
	Name           string
	ElectricalType ElectricalType
	GraphicStyle   GraphicStyle
}

func NewAlternatePin(name string, electrical ElectricalType, graphic GraphicStyle) AlternatePin {
	return AlternatePin{
		Name:           name,
		ElectricalType: electrical,
		GraphicStyle:   graphic,
	}
}

func (pin AlternatePin) String() string {
	return fmt.Sprintf("(alternate \"%s\" %s %s)", pin.Name, pin.ElectricalType, pin.GraphicStyle)
}
