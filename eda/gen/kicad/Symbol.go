package kicad

import "fmt"

type Symbol struct {
	Name       string
	InBOM      bool
	OnBoard    bool
	Properties []Property
	Variants   []SymbolVariant
}

func NewSymbol(name string) Symbol {
	return Symbol{
		Name:       name,
		InBOM:      true,
		OnBoard:    true,
		Properties: nil,
		Variants:   nil,
	}
}

func (sym *Symbol) AddVariant() *SymbolVariant {
	major := 1
	if sym.Variants == nil {
		sym.Variants = []SymbolVariant{NewSymbolVariant(sym)}
	} else {
		major = sym.Variants[len(sym.Variants)-1].Major + 1
		sym.Variants = append(sym.Variants, NewSymbolVariant(sym))
	}
	variant := &sym.Variants[len(sym.Variants)-1]
	variant.Major = major
	return variant
}

func (sym Symbol) String() string {
	var inBom string
	if sym.InBOM {
		inBom = "yes"
	} else {
		inBom = "no"
	}
	var onBoard string
	if sym.OnBoard {
		onBoard = "yes"
	} else {
		onBoard = "no"
	}
	str := fmt.Sprintf("  (symbol \"%s\" (in_bom %s) (on_board %s)\n", sym.Name, inBom, onBoard)
	if sym.Properties != nil {
		for _, prop := range sym.Properties {
			str += prop.String()
		}
	}
	if sym.Variants != nil {
		for _, variant := range sym.Variants {
			str += variant.String()
		}
	}
	return fmt.Sprintf("%s  )\n", str)
}
