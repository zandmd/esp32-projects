package kicad

import "fmt"

type Text struct {
	Text     string
	Position Position
	Font     Font
}

func NewText(text string) Text {
	return Text{
		Text:     text,
		Position: NewPosition(),
		Font:     NewFont(),
	}
}

func (text Text) String() string {
	return fmt.Sprintf(
		""+
			"      (text \"%s\" %s\n"+
			"        %s\n"+
			"      )\n",
		text.Text,
		text.Position,
		text.Font,
	)
}
