package kicad

import "fmt"

type Position struct {
	X        float32
	Y        float32
	Rotation float32
}

func NewPosition() Position {
	return Position{
		X:        0,
		Y:        0,
		Rotation: 0,
	}
}

func (pos Position) String() string {
	return fmt.Sprintf("(at %g %g %g)", pos.X, pos.Y, pos.Rotation)
}
