package kicad

type GraphicStyle string

const (
	GraphicStyleLine             GraphicStyle = "line"
	GraphicStyleInverted         GraphicStyle = "inverted"
	GraphicStyleClock            GraphicStyle = "clock"
	GraphicStyleInvertedClock    GraphicStyle = "inverted_clock"
	GraphicStyleInputLow         GraphicStyle = "input_low"
	GraphicStyleClockLow         GraphicStyle = "clock_low"
	GraphicStyleOutputLow        GraphicStyle = "output_low"
	GraphicStyleFallingEdgeClock GraphicStyle = "edge_clock_high"
	GraphicStyleNonLogic         GraphicStyle = "non_logic"
)
