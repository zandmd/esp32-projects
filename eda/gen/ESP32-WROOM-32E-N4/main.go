package main

import (
	"encoding/xml"
	"fmt"
	"io/ioutil"
	"os"
	"strconv"
	"strings"

	"github.com/zandmd/esp32-projects/eda/gen/kicad"
)

type Datasheet struct {
	XmlName      xml.Name     `xml:"datasheet"`
	IOMux        IOMux        `xml:"io-mux"`
	RtcMux       RtcMux       `xml:"rtc-mux"`
	SpiPins      SpiPins      `xml:"spi-pins"`
	Bootloader   Bootloader   `xml:"bootloader"`
	Module       Module       `xml:"module"`
	PhysicalPins PhysicalPins `xml:"physical"`
}

type IOResetConfig int

const (
	ResetInputDisabled        IOResetConfig = 0
	ResetInputEnabled         IOResetConfig = 1
	ResetInputEnabledPullDown IOResetConfig = 2
	ResetInputEnabledPullUp   IOResetConfig = 3
)

type IOMux struct {
	XmlName xml.Name   `xml:"io-mux"`
	Pins    []IOMuxPin `xml:"io"`
}

type IOMuxPin struct {
	XmlName     xml.Name      `xml:"io"`
	GpioNum     int           `xml:"gpio"`
	PadName     string        `xml:"pad-name"`
	Function0   string        `xml:"func-0"`
	Function1   string        `xml:"func-1"`
	Function2   string        `xml:"func-2"`
	Function3   string        `xml:"func-3"`
	Function4   string        `xml:"func-4"`
	Function5   string        `xml:"func-5"`
	ResetAction IOResetConfig `xml:"reset"`
	Notes       string        `xml:"notes"`
}

type RtcMux struct {
	XmlName xml.Name    `xml:"rtc-mux"`
	Pins    []RtcMuxPin `xml:"io"`
}

type RtcMuxPin struct {
	XmlName    xml.Name `xml:"io"`
	RtcGpioNum int      `xml:"rtc-gpio-num"`
	GpioNum    int      `xml:"gpio-num"`
	PadName    string   `xml:"pad-name"`
	Analog0    string   `xml:"analog-0"`
	Analog1    string   `xml:"analog-1"`
	Analog2    string   `xml:"analog-2"`
	Rtc0       string   `xml:"rtc-0"`
	Rtc1       string   `xml:"rtc-1"`
}

type SpiPins struct {
	XmlName xml.Name    `xml:"spi-pins"`
	Configs []SpiConfig `xml:"config"`
}

type SpiConfig struct {
	XmlName xml.Name `xml:"config"`
	Name    string   `xml:"name,attr"`
	Pins    []SpiPin `xml:"pin"`
}

type SpiPin struct {
	XmlName xml.Name `xml:"pin"`
	Name    string   `xml:"name,attr"`
	Spi     string   `xml:"spi"`
	HSpi    string   `xml:"hspi"`
	VSpi    string   `xml:"vspi"`
}

type Bootloader struct {
	XmlName   xml.Name `xml:"bootloader"`
	PullDowns []int    `xml:"pull-down"`
	PullUps   []int    `xml:"pull-up"`
	Pwms      []int    `xml:"pwm"`
}

type Module struct {
	XmlName    xml.Name `xml:"module"`
	NoConnects []int    `xml:"no-connect"`
}

type PhysicalPins struct {
	XmlName xml.Name      `xml:"physical"`
	Pins    []PhysicalPin `xml:"gpio"`
}

type PhysicalPin struct {
	XmlName  xml.Name `xml:"gpio"`
	PinNum   int      `xml:"pin,attr"`
	GpioName string   `xml:",chardata"`
}

func main() {
	sym := kicad.NewSymbol("ESP32-WROOM-32E-N4")
	sym.Properties = []kicad.Property{
		kicad.NewProperty("Reference", "U"),
		kicad.NewHiddenProperty("Value", "ESP32-WROOM-32E-N4"),
		kicad.NewHiddenProperty("Footprint", "zandmd:ESP32-WROOM-32E-N4"),
		kicad.NewHiddenProperty("Datasheet", "https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32e_esp32-wroom-32ue_datasheet_en.pdf"),
		kicad.NewHiddenProperty("Design Guide", "https://www.espressif.com/sites/default/files/documentation/esp32_hardware_design_guidelines_en.pdf"),
		kicad.NewHiddenProperty("Technical Reference Manual", "https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf"),
		kicad.NewHiddenProperty("Sim.Enable", "0"),
		kicad.NewHiddenProperty("ki_locked", ""),
		kicad.NewHiddenProperty("ki_fp_filters", "ESP32-WROOM-32E-N4"),
	}
	powerSym := sym.AddVariant()
	powerSym.AddRectangle(2.54, 20.32, 15.24, 0)

	pin := powerSym.AddPin("3V3", "2")
	pin.ElectricalType = kicad.ElectricalTypePowerInput
	pin.Position.Y = 17.78

	pin = powerSym.AddPin("EN", "3")
	pin.ElectricalType = kicad.ElectricalTypeInput
	pin.Position.Y = 15.24

	pin = powerSym.AddPin("GND", "1")
	pin.ElectricalType = kicad.ElectricalTypePassive
	pin.Position.Y = 10.16

	pin = powerSym.AddPin("GND", "15")
	pin.ElectricalType = kicad.ElectricalTypePassive
	pin.Position.Y = 7.62

	pin = powerSym.AddPin("GND", "38")
	pin.ElectricalType = kicad.ElectricalTypePassive
	pin.Position.Y = 5.08

	pin = powerSym.AddPin("GND", "39")
	pin.ElectricalType = kicad.ElectricalTypePassive
	pin.Position.Y = 2.54

	pin = powerSym.AddPin("NC", "17")
	pin.ElectricalType = kicad.ElectricalTypeUnconnected
	pin.Position.X = 17.78
	pin.Position.Y = 17.78
	pin.Position.Rotation = 180

	pin = powerSym.AddPin("NC", "18")
	pin.ElectricalType = kicad.ElectricalTypeUnconnected
	pin.Position.X = 17.78
	pin.Position.Y = 15.24
	pin.Position.Rotation = 180

	pin = powerSym.AddPin("NC", "19")
	pin.ElectricalType = kicad.ElectricalTypeUnconnected
	pin.Position.X = 17.78
	pin.Position.Y = 12.7
	pin.Position.Rotation = 180

	pin = powerSym.AddPin("NC", "20")
	pin.ElectricalType = kicad.ElectricalTypeUnconnected
	pin.Position.X = 17.78
	pin.Position.Y = 10.16
	pin.Position.Rotation = 180

	pin = powerSym.AddPin("NC", "21")
	pin.ElectricalType = kicad.ElectricalTypeUnconnected
	pin.Position.X = 17.78
	pin.Position.Y = 7.62
	pin.Position.Rotation = 180

	pin = powerSym.AddPin("NC", "22")
	pin.ElectricalType = kicad.ElectricalTypeUnconnected
	pin.Position.X = 17.78
	pin.Position.Y = 5.08
	pin.Position.Rotation = 180

	pin = powerSym.AddPin("NC", "32")
	pin.ElectricalType = kicad.ElectricalTypeUnconnected
	pin.Position.X = 17.78
	pin.Position.Y = 2.54
	pin.Position.Rotation = 180

	var xmlFile *os.File
	var err error
	if xmlFile, err = os.Open("datasheet.xml"); err != nil {
		panic(err)
	}
	defer xmlFile.Close()
	var contents []byte
	if contents, err = ioutil.ReadAll(xmlFile); err != nil {
		panic(err)
	}
	var ds Datasheet
	xml.Unmarshal(contents, &ds)
	ioSym := sym.AddVariant()

	AddAllPins(ioSym, &ds)
	ioSym.AddRectangle(2.54, float32(len(ioSym.Pins)+1)*2.54, 45*2.54, 0)
	AddGpioModes(ioSym)
	AddUarts(ioSym)
	AddI2c(ioSym, 2)
	AddSpi(ioSym, &ds)
	AddAdc(ioSym)
	AddStrapping(ioSym, &ds)
	AddNonConnected(ioSym)
	MaskNonConnected(ioSym, &ds)
	UpdatePinNumbers(ioSym, &ds)

	println(sym.String())
}

func AddAllPins(sym *kicad.SymbolVariant, ds *Datasheet) {
	for _, io := range ds.IOMux.Pins {
		names := make(map[string]bool)
		names[io.PadName] = true
		if len(io.Function0) > 0 {
			names[io.Function0] = true
		}
		if len(io.Function1) > 0 {
			names[io.Function1] = true
		}
		if len(io.Function2) > 0 {
			names[io.Function2] = true
		}
		if len(io.Function3) > 0 {
			names[io.Function3] = true
		}
		if len(io.Function4) > 0 {
			names[io.Function4] = true
		}
		if len(io.Function5) > 0 {
			names[io.Function5] = true
		}
		if strings.ContainsRune(io.Notes, 'R') {
			for _, rtc := range ds.RtcMux.Pins {
				if rtc.GpioNum == io.GpioNum {
					if len(rtc.Analog0) > 0 {
						names[rtc.Analog0] = true
					}
					if len(rtc.Analog1) > 0 {
						names[rtc.Analog1] = true
					}
					if len(rtc.Analog2) > 0 {
						names[rtc.Analog2] = true
					}
					if len(rtc.Rtc0) > 0 {
						names[rtc.Rtc0] = true
					}
					if len(rtc.Rtc1) > 0 {
						names[rtc.Rtc1] = true
					}
					break
				}
			}
		}
		pinName := ""
		for name := range names {
			if len(pinName) == 0 {
				pinName = name
			} else {
				pinName = fmt.Sprintf("%s/%s", pinName, name)
			}
		}
		pin := sym.AddPin(pinName, strconv.FormatInt(int64(io.GpioNum), 10))
		if strings.ContainsRune(io.Notes, 'I') {
			pin.ElectricalType = kicad.ElectricalTypeInput
		} else {
			pin.ElectricalType = kicad.ElectricalTypeBidirectional
		}
	}
	for i, _ := range sym.Pins {
		sym.Pins[i].Position.Y = float32(len(sym.Pins)-i) * 2.54
	}
}

func AddGpioModes(sym *kicad.SymbolVariant) {
	for i, pin := range sym.Pins {
		parts := strings.Split(pin.Name, "/")
		for _, part := range parts {
			if strings.HasPrefix(part, "GPIO") {
				sym.Pins[i].AddAlternate(part)
				if pin.ElectricalType == kicad.ElectricalTypeBidirectional {
					sym.Pins[i].AddAlternate("GPI" + part[4:]).ElectricalType = kicad.ElectricalTypeInput
					sym.Pins[i].AddAlternate("GPO" + part[4:]).ElectricalType = kicad.ElectricalTypeOutput
				}
			}
		}
	}
}

func AddUarts(sym *kicad.SymbolVariant) {
	for i, pin := range sym.Pins {
		parts := strings.Split(pin.Name, "/")
		for _, part := range parts {
			if part[0] == 'U' {
				var et kicad.ElectricalType
				if strings.HasSuffix(part, "RXD") {
					et = kicad.ElectricalTypeInput
				} else if strings.HasSuffix(part, "TXD") {
					et = kicad.ElectricalTypeOutput
				} else if strings.HasSuffix(part, "CTS") {
					et = kicad.ElectricalTypeInput
				} else if strings.HasSuffix(part, "RTS") {
					et = kicad.ElectricalTypeOutput
				} else {
					continue
				}
				sym.Pins[i].AddAlternate(part).ElectricalType = et
			}
		}
	}
}

func AddI2c(sym *kicad.SymbolVariant, busses int) {
	for i, pin := range sym.Pins {
		if pin.ElectricalType == kicad.ElectricalTypeBidirectional {
			for b := 0; b < busses; b++ {
				sym.Pins[i].AddAlternate(fmt.Sprintf("I2C%dSDA", b)).ElectricalType = kicad.ElectricalTypeOpenCollector
				sym.Pins[i].AddAlternate(fmt.Sprintf("I2C%dSCL", b)).ElectricalType = kicad.ElectricalTypeOpenCollector
			}
		}
	}
}

func AddSpi(sym *kicad.SymbolVariant, ds *Datasheet) {
	for _, config := range ds.SpiPins.Configs {
		for _, pin := range config.Pins {
			for pre, name := range map[string]string{
				"":  pin.Spi,
				"H": pin.HSpi,
				"V": pin.VSpi,
			} {
				var master kicad.ElectricalType
				var slave kicad.ElectricalType
				if pin.Name == "MOSI" {
					master = kicad.ElectricalTypeOutput
					slave = kicad.ElectricalTypeInput
				} else if pin.Name == "MISO" {
					master = kicad.ElectricalTypeInput
					slave = kicad.ElectricalTypeOutput
				} else if pin.Name == "CS" {
					master = kicad.ElectricalTypeOutput
					slave = kicad.ElectricalTypeInput
				} else if pin.Name == "SCLK" {
					master = kicad.ElectricalTypeOutput
					slave = kicad.ElectricalTypeInput
				} else if pin.Name == "DATA" {
					master = kicad.ElectricalTypeBidirectional
					slave = kicad.ElectricalTypeBidirectional
				} else if strings.HasPrefix(pin.Name, "IO") {
					master = kicad.ElectricalTypeBidirectional
					slave = kicad.ElectricalTypeBidirectional
				} else {
					panic("Unknown SPI pin name: " + pin.Name)
				}
				needle := fmt.Sprintf("/%s/", name)
				for i, p := range sym.Pins {
					if strings.Contains(fmt.Sprintf("/%s/", p.Name), needle) {
						sym.Pins[i].AddAlternate(fmt.Sprintf("%s%sM.%s", pre, config.Name, pin.Name)).ElectricalType = master
						sym.Pins[i].AddAlternate(fmt.Sprintf("%s%sS.%s", pre, config.Name, pin.Name)).ElectricalType = slave
					}
				}
			}
		}
	}
}

func AddAdc(sym *kicad.SymbolVariant) {
	for i, pin := range sym.Pins {
		parts := strings.Split(pin.Name, "/")
		for _, part := range parts {
			if strings.HasPrefix(part, "ADC") {
				sym.Pins[i].AddAlternate(part).ElectricalType = kicad.ElectricalTypeInput
			}
		}
	}
}

func AddStrapping(sym *kicad.SymbolVariant, ds *Datasheet) {
	for i, pin := range sym.Pins {
		pullDown := false
		pullUp := false
		pwm := false
		for _, n := range ds.Bootloader.PullDowns {
			if pin.Number == strconv.FormatInt(int64(n), 10) {
				pullDown = true
				break
			}
		}
		for _, n := range ds.Bootloader.PullUps {
			if pin.Number == strconv.FormatInt(int64(n), 10) {
				pullUp = true
				break
			}
		}
		for _, n := range ds.Bootloader.Pwms {
			if pin.Number == strconv.FormatInt(int64(n), 10) {
				pwm = true
				break
			}
		}
		var suffix string
		ignore := make(map[kicad.ElectricalType]bool)
		ignore[kicad.ElectricalTypeFree] = true
		ignore[kicad.ElectricalTypeUnspecified] = true
		ignore[kicad.ElectricalTypeUnconnected] = true
		if pullDown {
			if pullUp {
				panic("Pulled down and up")
			} else {
				if pwm {
					suffix = "(PULL_DOWN+PWM)"
				} else {
					suffix = "(PULL_DOWN)"
				}
			}
		} else if pullUp {
			if pwm {
				suffix = "(PULL_UP+PWM)"
			} else {
				suffix = "(PULL_UP)"
			}
		} else if pwm {
			suffix = "(PWM)"
			ignore[kicad.ElectricalTypeInput] = true
		} else {
			continue
		}
		sym.Pins[i].Name += suffix
		for j, _ := range pin.Alternates {
			sym.Pins[i].Alternates[j].Name += suffix
		}
	}
}

func AddNonConnected(sym *kicad.SymbolVariant) {
	for i, _ := range sym.Pins {
		sym.Pins[i].AddAlternate("NC").ElectricalType = kicad.ElectricalTypeUnconnected
	}
}

func MaskNonConnected(sym *kicad.SymbolVariant, ds *Datasheet) {
	for _, nc := range ds.Module.NoConnects {
		for i, pin := range sym.Pins {
			if pin.Number == strconv.FormatInt(int64(nc), 10) {
				text := sym.AddText(pin.Name)
				text.Position.X = 2.9718
				text.Position.Y = pin.Position.Y
				text.Font.ColorG = 100
				text.Font.ColorB = 100
				text.Font.Justificiation = kicad.JustifyLeft
				sym.Pins = append(sym.Pins[:i], sym.Pins[i+1:]...)
				break
			}
		}
	}
}

func UpdatePinNumbers(sym *kicad.SymbolVariant, ds *Datasheet) {
	for i, pin := range sym.Pins {
		found := false
		haystack := fmt.Sprintf("/%s/", strings.Split(pin.Name, "(")[0])
		for _, p := range ds.PhysicalPins.Pins {
			if strings.Contains(haystack, fmt.Sprintf("/%s/", p.GpioName)) {
				sym.Pins[i].Number = strconv.FormatInt(int64(p.PinNum), 10)
				found = true
				break
			}
		}
		if !found {
			panic("Unknown pin")
		}
	}
}
