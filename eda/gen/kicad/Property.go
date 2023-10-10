package kicad

import "fmt"

type Property struct {
	Name     string
	Value    string
	Position Position
	Font     Font
}

func NewProperty(name string, value string) Property {
	return Property{
		Name:     name,
		Value:    value,
		Position: NewPosition(),
		Font:     NewFont(),
	}
}

func NewHiddenProperty(name string, value string) Property {
	font := NewFont()
	font.Hide = true
	return Property{
		Name:     name,
		Value:    value,
		Position: NewPosition(),
		Font:     font,
	}
}

func (prop Property) String() string {
	return fmt.Sprintf(
		""+
			"    (property \"%s\" \"%s\" %s\n"+
			"      %s\n"+
			"    )\n",
		prop.Name,
		prop.Value,
		prop.Position,
		prop.Font,
	)
}
