package kicad

import "fmt"

type Rectangle struct {
	X0 float32
	Y0 float32
	X1 float32
	Y1 float32
}

func NewRectangle(x0 float32, y0 float32, x1 float32, y1 float32) Rectangle {
	return Rectangle{
		X0: x0,
		Y0: y0,
		X1: x1,
		Y1: y1,
	}
}

func (rect Rectangle) String() string {
	return fmt.Sprintf(
		""+
			"      (rectangle (start %g %g) (end %g %g)\n"+
			"        (stroke (width 0) (type default))\n"+
			"        (fill (type none))\n"+
			"      )\n",
		rect.X0,
		rect.Y0,
		rect.X1,
		rect.Y1,
	)
}
