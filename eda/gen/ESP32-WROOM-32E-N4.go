package main

import (
	"encoding/xml"
	"io/ioutil"
	"os"

	"./kicad"
)

type Datasheet struct {
	XmlName    xml.Name   `xml:"datasheet"`
	IOMux      IOMux      `xml:"io-mux"`
	RtcMux     RtcMux     `xml:"rtc-mux"`
	SpiPins    SpiPins    `xml:"spi-pins"`
	Bootloader Bootloader `xml:"bootloader"`
	Module     Module     `xml:"module"`
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
	XmlName xml.Name `xml:"spi-pins"`
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

func main() {
	var xmlFile *os.File
	var err error
	if xmlFile, err = os.Open("ESP32-WROOM-32E-N4.xml"); err != nil {
		panic(err)
	}
	defer xmlFile.Close()
	var contents []byte
	if contents, err = ioutil.ReadAll(xmlFile); err != nil {
		panic(err)
	}
	var datasheet Datasheet
	xml.Unmarshal(contents, &datasheet)
	var sym kicad.Symbol
	sym.Name = ""
}
