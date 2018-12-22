EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:audio-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 6
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Sheet
S 6300 900  1300 1600
U 5B016CD8
F0 "Amplifier" 60
F1 "amplifier.sch" 60
F2 "MCLK" I L 6300 1100 60 
F3 "SCLK" I L 6300 1200 60 
F4 "FS" I L 6300 1300 60 
F5 "SD1" I L 6300 1400 60 
F6 "SD2" I L 6300 1500 60 
F7 "~STBY" I L 6300 2000 60 
F8 "~MUTE" I L 6300 2100 60 
F9 "~FAULT" O L 6300 2200 60 
F10 "~WARN" O L 6300 2300 60 
F11 "SCL" I L 6300 1700 60 
F12 "SDA" B L 6300 1800 60 
F13 "OUT1-" O R 7600 1200 60 
F14 "OUT1+" O R 7600 1100 60 
F15 "OUT2-" O R 7600 1500 60 
F16 "OUT2+" O R 7600 1400 60 
F17 "OUT3-" O R 7600 1800 60 
F18 "OUT3+" O R 7600 1700 60 
F19 "OUT4-" O R 7600 2100 60 
F20 "OUT4+" O R 7600 2000 60 
$EndSheet
Wire Wire Line
	5300 1100 6300 1100
Wire Wire Line
	5300 1200 6300 1200
Wire Wire Line
	5300 1300 6300 1300
Wire Wire Line
	5300 1400 6300 1400
Wire Wire Line
	5300 1500 6300 1500
$Comp
L QUADLOCK X3
U 1 1 5B0EA55D
P 9100 1000
F 0 "X3" H 9200 1200 60  0000 C CNN
F 1 "1241768-1" H 9350 1100 60  0000 C CNN
F 2 "AUDIO:1241768" H 9100 700 60  0001 C CNN
F 3 "" H 9100 700 60  0001 C CNN
	1    9100 1000
	1    0    0    -1  
$EndComp
Text Notes 9300 1050 0    60   ~ 0
RR+
Text Notes 9300 1150 0    60   ~ 0
FR+
Text Notes 9300 1250 0    60   ~ 0
FL+
Text Notes 9300 1350 0    60   ~ 0
RL+
Text Notes 9300 1450 0    60   ~ 0
RR-
Text Notes 9300 1550 0    60   ~ 0
FR-
Text Notes 9300 1650 0    60   ~ 0
FL-
Text Notes 9300 1750 0    60   ~ 0
RL-
Text Notes 9300 2150 0    60   ~ 0
GND
Text Notes 9300 2450 0    60   ~ 0
BAT+
Text Notes 9300 1850 0    60   ~ 0
CAN H
Text Notes 9300 1950 0    60   ~ 0
CAN L
$Comp
L QUADLOCK X3
U 2 1 5B0EEA26
P 9100 3200
F 0 "X3" H 9200 3400 60  0000 C CNN
F 1 "1241768-1" H 9350 3300 60  0000 C CNN
F 2 "AUDIO:1241768" H 9100 2900 60  0001 C CNN
F 3 "" H 9100 2900 60  0001 C CNN
	2    9100 3200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 5B0EEE35
P 8800 4500
F 0 "#PWR01" H 8800 4250 50  0001 C CNN
F 1 "GND" H 8800 4350 50  0000 C CNN
F 2 "" H 8800 4500 50  0001 C CNN
F 3 "" H 8800 4500 50  0001 C CNN
	1    8800 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8800 2100 8800 4500
Wire Wire Line
	8800 2100 8900 2100
Wire Wire Line
	5300 3000 6300 3000
Wire Wire Line
	5300 3100 6300 3100
Wire Wire Line
	7600 3000 8500 3000
Wire Wire Line
	8500 3000 8500 1800
Wire Wire Line
	8500 1800 8900 1800
Wire Wire Line
	7600 3100 8600 3100
Wire Wire Line
	8600 3100 8600 1900
Wire Wire Line
	8600 1900 8900 1900
$Sheet
S 3900 900  1400 4900
U 5B016CBC
F0 "Controller" 60
F1 "controller.sch" 60
F2 "CAN1RX" I R 5300 3000 60 
F3 "CAN1TX" O R 5300 3100 60 
F4 "CAN2RX" I R 5300 3200 60 
F5 "CAN2TX" O R 5300 3300 60 
F6 "AOUT_MCK" O R 5300 1100 60 
F7 "AOUT_SCLK" O R 5300 1200 60 
F8 "AOUT_SD1" O R 5300 1400 60 
F9 "AOUT_SD2" O R 5300 1500 60 
F10 "AOUT_FS" O R 5300 1300 60 
F11 "RADIO_FS" O L 3900 1300 60 
F12 "RADIO_SCLK" O L 3900 1200 60 
F13 "RADIO_SD1" I L 3900 1400 60 
F14 "RADIO_SD2" I L 3900 1500 60 
F15 "USB1_DM" B L 3900 3500 60 
F16 "USB1_DP" B L 3900 3400 60 
F17 "USB2_DM" B L 3900 3800 60 
F18 "USB2_DP" B L 3900 3700 60 
F19 "REFCLK" I R 5300 4500 60 
F20 "MDIO" B R 5300 5400 60 
F21 "CRSDV" I R 5300 4600 60 
F22 "TXEN" O R 5300 4900 60 
F23 "TXD0" O R 5300 5000 60 
F24 "TXD1" O R 5300 5100 60 
F25 "MDC" O R 5300 5300 60 
F26 "RXD0" I R 5300 4700 60 
F27 "RXD1" I R 5300 4800 60 
F28 "AOUT_SDA" B R 5300 1800 60 
F29 "AOUT_SCL" O R 5300 1700 60 
F30 "RADIO_SDA" B L 3900 1800 60 
F31 "RADIO_SCL" O L 3900 1700 60 
F32 "KEYS" I L 3900 5000 60 
F33 "AOUT_STBY" O R 5300 2000 60 
F34 "AOUT_FAULT" I R 5300 2200 60 
F35 "AOUT_WARN" I R 5300 2300 60 
F36 "AOUT_MUTE" O R 5300 2100 60 
F37 "CAN_EN" O R 5300 3500 60 
F38 "CAN_STBN" O R 5300 3600 60 
F39 "CAN1ERR" I R 5300 3700 60 
F40 "CAN2ERR" I R 5300 3800 60 
F41 "RADIO_RESET" O L 3900 2400 60 
F42 "ETH_RESET" O R 5300 5500 60 
F43 "USB1_VBUS" O L 3900 3600 60 
F44 "USB2_VBUS" O L 3900 3900 60 
F45 "RADIO_RX" O L 3900 2500 60 
F46 "RADIO_TX" I L 3900 2600 60 
F47 "ETH_WAKE" O R 5300 5600 60 
$EndSheet
$Sheet
S 6300 4300 1300 1500
U 5B0F5683
F0 "Ethernet" 60
F1 "ethernet.sch" 60
F2 "DA+" B R 7600 4600 60 
F3 "DA-" B R 7600 4700 60 
F4 "REFCLK" O L 6300 4500 60 
F5 "RXD0" O L 6300 4700 60 
F6 "RXD1" O L 6300 4800 60 
F7 "RXDV" O L 6300 4600 60 
F8 "TXEN" I L 6300 4900 60 
F9 "TXD0" I L 6300 5000 60 
F10 "TXD1" I L 6300 5100 60 
F11 "MDC" I L 6300 5300 60 
F12 "MDIO" B L 6300 5400 60 
F13 "PWR_ON" O R 7600 5500 60 
F14 "~RESET" I L 6300 5500 60 
F15 "WAKE" I L 6300 5600 60 
$EndSheet
$Comp
L Conn_01x08 X1
U 1 1 5B0F6A31
P 2600 1400
F 0 "X1" H 2600 1800 50  0000 C CNN
F 1 "Conn_01x08" H 2600 900 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x08_Pitch2.54mm" H 2600 1400 50  0001 C CNN
F 3 "" H 2600 1400 50  0001 C CNN
	1    2600 1400
	-1   0    0    -1  
$EndComp
$Comp
L Conn_01x08 X2
U 1 1 5B0F6BBA
P 2600 2500
F 0 "X2" H 2600 2900 50  0000 C CNN
F 1 "Conn_01x08" H 2600 2000 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x08_Pitch2.54mm" H 2600 2500 50  0001 C CNN
F 3 "" H 2600 2500 50  0001 C CNN
	1    2600 2500
	-1   0    0    -1  
$EndComp
$Comp
L QUADLOCK X3
U 3 1 5B0F6D11
P 2600 3400
F 0 "X3" H 2700 3600 60  0000 C CNN
F 1 "1241768-1" H 2850 3500 60  0000 C CNN
F 2 "AUDIO:1241768" H 2600 3100 60  0001 C CNN
F 3 "" H 2600 3100 60  0001 C CNN
	3    2600 3400
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 5B0F7F92
P 2900 4700
F 0 "#PWR02" H 2900 4450 50  0001 C CNN
F 1 "GND" H 2900 4550 50  0000 C CNN
F 2 "" H 2900 4700 50  0001 C CNN
F 3 "" H 2900 4700 50  0001 C CNN
	1    2900 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 3400 2800 3400
Wire Wire Line
	3900 3500 3600 3500
Wire Wire Line
	3600 3500 3600 4000
Wire Wire Line
	3600 4000 2800 4000
Wire Wire Line
	3900 3700 2800 3700
Wire Wire Line
	3900 3800 3700 3800
Wire Wire Line
	3700 3800 3700 4300
Wire Wire Line
	3700 4300 2800 4300
Wire Wire Line
	2900 1600 2900 4700
Wire Wire Line
	2900 4100 2800 4100
Wire Wire Line
	2900 4400 2800 4400
Connection ~ 2900 4400
Wire Wire Line
	2900 1600 2800 1600
Connection ~ 2900 4100
Wire Wire Line
	2800 1500 3900 1500
Wire Wire Line
	2800 1400 3900 1400
Wire Wire Line
	2800 1300 3900 1300
Wire Wire Line
	2800 1200 3900 1200
Wire Wire Line
	5300 1700 6300 1700
Wire Wire Line
	5300 1800 6300 1800
Wire Wire Line
	3900 1800 2800 1800
Wire Wire Line
	3900 1700 2800 1700
Wire Wire Line
	8800 4300 8900 4300
Connection ~ 8800 4300
Wire Wire Line
	3900 5000 3700 5000
Wire Wire Line
	3700 5000 3700 6100
Wire Wire Line
	3700 6100 8600 6100
Wire Wire Line
	8600 6100 8600 3700
Wire Wire Line
	8600 3700 8900 3700
Wire Wire Line
	5300 3200 6300 3200
Wire Wire Line
	5300 3300 6300 3300
Wire Wire Line
	7600 3300 8100 3300
Wire Wire Line
	8100 3300 8100 3400
Wire Wire Line
	8100 3400 8900 3400
Wire Wire Line
	7600 3400 8000 3400
Wire Wire Line
	8000 3400 8000 4000
Wire Wire Line
	8000 4000 8900 4000
Wire Wire Line
	7600 4600 8300 4600
Wire Wire Line
	8300 4600 8300 3200
Wire Wire Line
	8300 3200 8900 3200
Wire Wire Line
	7600 4700 8400 4700
Wire Wire Line
	8400 4700 8400 3800
Wire Wire Line
	8400 3800 8900 3800
Wire Wire Line
	5300 4500 6300 4500
Wire Wire Line
	5300 4600 6300 4600
Wire Wire Line
	5300 4700 6300 4700
Wire Wire Line
	5300 4800 6300 4800
Wire Wire Line
	5300 4900 6300 4900
Wire Wire Line
	5300 5000 6300 5000
Wire Wire Line
	5300 5100 6300 5100
Wire Wire Line
	5300 5300 6300 5300
Wire Wire Line
	5300 5400 6300 5400
$Sheet
S 6300 2800 1300 1200
U 5B01FB4E
F0 "CAN" 60
F1 "can.sch" 60
F2 "CAN1TX" I L 6300 3100 60 
F3 "CAN1RX" O L 6300 3000 60 
F4 "CAN1H" B R 7600 3000 60 
F5 "CAN1L" B R 7600 3100 60 
F6 "CAN2TX" I L 6300 3300 60 
F7 "CAN2RX" O L 6300 3200 60 
F8 "CAN2H" B R 7600 3300 60 
F9 "CAN2L" B R 7600 3400 60 
F10 "EN" I L 6300 3500 60 
F11 "ERRN1" O L 6300 3700 60 
F12 "STBN" I L 6300 3600 60 
F13 "ERRN2" O L 6300 3800 60 
F14 "PWR_ON" O R 7600 3800 60 
$EndSheet
Wire Wire Line
	5300 2000 6300 2000
Wire Wire Line
	5300 2100 6300 2100
Wire Wire Line
	5300 2200 6300 2200
Wire Wire Line
	5300 2300 6300 2300
Wire Wire Line
	5300 3500 6300 3500
Wire Wire Line
	5300 3600 6300 3600
Wire Wire Line
	5300 3700 6300 3700
Wire Wire Line
	5300 3800 6300 3800
$Sheet
S 3900 6400 1400 800 
U 5B10CCDE
F0 "Power" 60
F1 "power.sch" 60
F2 "VIN" I R 5300 6500 60 
F3 "EN" I R 5300 6700 60 
$EndSheet
$Comp
L +3.3V #PWR03
U 1 1 5B10DF09
P 3100 2100
F 0 "#PWR03" H 3100 1950 50  0001 C CNN
F 1 "+3.3V" H 3100 2240 50  0000 C CNN
F 2 "" H 3100 2100 50  0001 C CNN
F 3 "" H 3100 2100 50  0001 C CNN
	1    3100 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 2200 3100 2200
Wire Wire Line
	3100 2200 3100 2100
Wire Wire Line
	2900 2300 2800 2300
Connection ~ 2900 2300
Wire Wire Line
	8700 2700 8700 6200
Wire Wire Line
	8700 6200 5900 6200
Wire Wire Line
	5900 6200 5900 6500
Wire Wire Line
	5900 6500 5300 6500
Wire Wire Line
	3900 2400 2800 2400
Wire Wire Line
	5300 5500 6300 5500
Wire Wire Line
	5300 6700 6000 6700
Wire Wire Line
	6000 6700 6000 6300
Wire Wire Line
	6000 6300 7800 6300
Wire Wire Line
	7800 6300 7800 3800
Wire Wire Line
	7800 5500 7600 5500
Wire Wire Line
	7800 3800 7600 3800
Connection ~ 7800 5500
Wire Wire Line
	3900 3600 3100 3600
Wire Wire Line
	3100 3600 3100 3500
Wire Wire Line
	3100 3500 2800 3500
Wire Wire Line
	3900 3900 3100 3900
Wire Wire Line
	3100 3900 3100 3800
Wire Wire Line
	3100 3800 2800 3800
Wire Wire Line
	3900 2500 2800 2500
Wire Wire Line
	3900 2600 2800 2600
Wire Wire Line
	2900 2700 2800 2700
Connection ~ 2900 2700
Wire Wire Line
	5300 5600 6300 5600
Wire Wire Line
	7600 1100 7800 1100
Wire Wire Line
	7800 1100 7800 1000
Wire Wire Line
	7800 1000 8900 1000
Wire Wire Line
	7600 1400 7900 1400
Wire Wire Line
	7900 1400 7900 1100
Wire Wire Line
	7900 1100 8900 1100
Wire Wire Line
	7600 1700 8100 1700
Wire Wire Line
	8100 1700 8100 1200
Wire Wire Line
	8100 1200 8900 1200
Wire Wire Line
	7600 2000 8200 2000
Wire Wire Line
	8200 2000 8200 1300
Wire Wire Line
	8200 1300 8900 1300
Wire Wire Line
	7600 1200 8000 1200
Wire Wire Line
	8000 1200 8000 1400
Wire Wire Line
	8000 1400 8900 1400
Wire Wire Line
	7600 1500 8900 1500
Wire Wire Line
	7600 1800 8300 1800
Wire Wire Line
	8300 1800 8300 1600
Wire Wire Line
	8300 1600 8900 1600
Wire Wire Line
	7600 2100 8400 2100
Wire Wire Line
	8400 2100 8400 1700
Wire Wire Line
	8400 1700 8900 1700
Wire Wire Line
	8700 2700 8900 2700
Wire Wire Line
	8900 2600 8700 2600
Wire Wire Line
	8700 2600 8700 2400
Wire Wire Line
	8700 2400 8900 2400
$EndSCHEMATC
