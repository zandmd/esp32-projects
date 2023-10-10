package kicad

import "fmt"

type SymbolVariant struct {
	Symbol     *Symbol
	Major      int
	Minor      int
	Rectangles []Rectangle
	Texts      []Text
	Pins       []Pin
}

func NewSymbolVariant(symbol *Symbol) SymbolVariant {
	return SymbolVariant{
		Symbol:     symbol,
		Major:      1,
		Minor:      1,
		Rectangles: nil,
		Texts:      nil,
		Pins:       nil,
	}
}

func (sym *SymbolVariant) AddRectangle(x0 float32, y0 float32, x1 float32, y1 float32) *Rectangle {
	rect := NewRectangle(x0, y0, x1, y1)
	if sym.Rectangles == nil {
		sym.Rectangles = []Rectangle{rect}
	} else {
		sym.Rectangles = append(sym.Rectangles, rect)
	}
	return &sym.Rectangles[len(sym.Rectangles)-1]
}

func (sym *SymbolVariant) AddText(text string) *Text {
	t := NewText(text)
	if sym.Texts == nil {
		sym.Texts = []Text{t}
	} else {
		sym.Texts = append(sym.Texts, t)
	}
	return &sym.Texts[len(sym.Texts)-1]
}

func (sym *SymbolVariant) AddPin(name string, number string) *Pin {
	pin := NewPin(name, number)
	if sym.Pins == nil {
		sym.Pins = []Pin{pin}
	} else {
		sym.Pins = append(sym.Pins, pin)
	}
	return &sym.Pins[len(sym.Pins)-1]
}

func (sym SymbolVariant) String() string {
	str := fmt.Sprintf("    (symbol \"%s_%d_%d\"\n", sym.Symbol.Name, sym.Major, sym.Minor)
	if sym.Rectangles != nil {
		for _, rect := range sym.Rectangles {
			str += rect.String()
		}
	}
	if sym.Texts != nil {
		for _, text := range sym.Texts {
			str += text.String()
		}
	}
	if sym.Pins != nil {
		for _, pin := range sym.Pins {
			str += pin.String()
		}
	}
	return fmt.Sprintf("%s    )\n", str)
}
