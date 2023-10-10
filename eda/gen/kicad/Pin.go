package kicad

import "fmt"

type Pin struct {
	ElectricalType ElectricalType
	GraphicStyle   GraphicStyle
	Position       Position
	Length         float32
	Name           string
	NameFont       Font
	Number         string
	NumberFont     Font
	Alternates     []AlternatePin
}

func NewPin(name string, number string) Pin {
	return Pin{
		ElectricalType: ElectricalTypeBidirectional,
		GraphicStyle:   GraphicStyleLine,
		Position:       NewPosition(),
		Length:         2.54,
		Name:           name,
		NameFont:       NewFont(),
		Number:         number,
		NumberFont:     NewFont(),
		Alternates:     nil,
	}
}

func (pin *Pin) AddAlternate(name string) *AlternatePin {
	alt := NewAlternatePin(name, pin.ElectricalType, pin.GraphicStyle)
	if pin.Alternates == nil {
		pin.Alternates = []AlternatePin{alt}
	} else {
		pin.Alternates = append(pin.Alternates, alt)
	}
	return &pin.Alternates[len(pin.Alternates)-1]
}

func (pin Pin) String() string {
	str := fmt.Sprintf(
		""+
			"      (pin %s %s %s (length %g)\n"+
			"        (name \"%s\" %s)\n"+
			"        (number \"%s\" %s)\n",
		pin.ElectricalType,
		pin.GraphicStyle,
		pin.Position,
		pin.Length,
		pin.Name,
		pin.NameFont,
		pin.Number,
		pin.NumberFont,
	)
	if pin.Alternates != nil {
		for _, alt := range pin.Alternates {
			str = fmt.Sprintf("%s        %s\n", str, alt)
		}
	}
	return fmt.Sprintf("%s      )\n", str)
}
