package kicad

type ElectricalType string

const (
	ElectricalTypeInput         ElectricalType = "input"
	ElectricalTypeOutput        ElectricalType = "output"
	ElectricalTypeBidirectional ElectricalType = "bidirectional"
	ElectricalTypeTristate      ElectricalType = "tri_state"
	ElectricalTypePassive       ElectricalType = "passive"
	ElectricalTypeFree          ElectricalType = "free"
	ElectricalTypeUnspecified   ElectricalType = "unspecified"
	ElectricalTypePowerInput    ElectricalType = "power_in"
	ElectricalTypePowerOutput   ElectricalType = "power_out"
	ElectricalTypeOpenCollector ElectricalType = "open_collector"
	ElectricalTypeOpenEmitter   ElectricalType = "open_emitter"
	ElectricalTypeUnconnected   ElectricalType = "no_connect"
)
