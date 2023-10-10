package kicad

import (
	"fmt"
)

type Font struct {
	Width          float32
	Height         float32
	Hide           bool
	ColorR         int
	ColorG         int
	ColorB         int
	Opacity        float32
	Justificiation Justificiation
}

func NewFont() Font {
	return Font{
		Width:          1.27,
		Height:         1.27,
		Hide:           false,
		ColorR:         -1,
		ColorG:         -1,
		ColorB:         -1,
		Opacity:        -1,
		Justificiation: JustifyCenter,
	}
}

func (font Font) String() string {
	str := fmt.Sprintf("font (size %g %g)", font.Width, font.Height)
	if font.ColorR >= 0 || font.ColorG >= 0 || font.ColorB >= 0 || font.Opacity >= 0 {
		r := font.ColorR
		if r < 0 {
			r = 0
		}
		g := font.ColorG
		if g < 0 {
			g = 0
		}
		b := font.ColorB
		if b < 0 {
			b = 0
		}
		opacity := font.Opacity
		if opacity < 0 {
			opacity = 1
		}
		str = fmt.Sprintf("%s (color %d %d %d %g)", str, r, g, b, opacity)
	}
	str = fmt.Sprintf("effects (%s)", str)
	if font.Hide {
		str = fmt.Sprintf("%s hide", str)
	}
	if font.Justificiation != JustifyCenter {
		str = fmt.Sprintf("%s (justify %s)", str, font.Justificiation)
	}
	return fmt.Sprintf("(%s)", str)
}
