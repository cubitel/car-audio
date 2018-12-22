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
Sheet 4 6
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L TJA1100 D4
U 1 1 5B100212
P 4100 2200
F 0 "D4" H 4100 2450 60  0000 C CNN
F 1 "TJA1100" H 4100 2350 60  0000 C CNN
F 2 "Housings_DFN_QFN:QFN-36-1EP_6x6mm_Pitch0.5mm" H 4100 2200 60  0001 C CNN
F 3 "" H 4100 2200 60  0001 C CNN
	1    4100 2200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR054
U 1 1 5B100445
P 4800 4700
F 0 "#PWR054" H 4800 4450 50  0001 C CNN
F 1 "GND" H 4800 4550 50  0000 C CNN
F 2 "" H 4800 4700 50  0001 C CNN
F 3 "" H 4800 4700 50  0001 C CNN
	1    4800 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 4200 4800 4700
Wire Wire Line
	4800 4200 4700 4200
Wire Wire Line
	4800 4300 4700 4300
Connection ~ 4800 4300
Wire Wire Line
	4800 4400 4700 4400
Connection ~ 4800 4400
Wire Wire Line
	4700 4500 7700 4500
Connection ~ 4800 4500
$Comp
L L L10
U 1 1 5B10061E
P 5750 2200
F 0 "L10" V 5700 2200 50  0000 C CNN
F 1 "120nH" V 5825 2200 50  0000 C CNN
F 2 "Inductors_SMD:L_0805" H 5750 2200 50  0001 C CNN
F 3 "" H 5750 2200 50  0001 C CNN
	1    5750 2200
	0    -1   -1   0   
$EndComp
$Comp
L L L11
U 1 1 5B100691
P 5750 2500
F 0 "L11" V 5700 2500 50  0000 C CNN
F 1 "120nH" V 5825 2500 50  0000 C CNN
F 2 "Inductors_SMD:L_0805" H 5750 2500 50  0001 C CNN
F 3 "" H 5750 2500 50  0001 C CNN
	1    5750 2500
	0    -1   -1   0   
$EndComp
$Comp
L R R21
U 1 1 5B1006D0
P 5500 2000
F 0 "R21" V 5580 2000 50  0000 C CNN
F 1 "51" V 5500 2000 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5430 2000 50  0001 C CNN
F 3 "" H 5500 2000 50  0001 C CNN
	1    5500 2000
	0    -1   -1   0   
$EndComp
$Comp
L R R23
U 1 1 5B10076B
P 6000 2000
F 0 "R23" V 6080 2000 50  0000 C CNN
F 1 "51" V 6000 2000 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5930 2000 50  0001 C CNN
F 3 "" H 6000 2000 50  0001 C CNN
	1    6000 2000
	0    -1   -1   0   
$EndComp
$Comp
L R R22
U 1 1 5B1007C8
P 5500 2700
F 0 "R22" V 5580 2700 50  0000 C CNN
F 1 "51" V 5500 2700 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5430 2700 50  0001 C CNN
F 3 "" H 5500 2700 50  0001 C CNN
	1    5500 2700
	0    -1   -1   0   
$EndComp
$Comp
L R R24
U 1 1 5B10082B
P 6000 2700
F 0 "R24" V 6080 2700 50  0000 C CNN
F 1 "51" V 6000 2700 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 5930 2700 50  0001 C CNN
F 3 "" H 6000 2700 50  0001 C CNN
	1    6000 2700
	0    -1   -1   0   
$EndComp
$Comp
L EMI_Filter_CommonMode L15
U 1 1 5B10088E
P 7500 2350
F 0 "L15" H 7500 2650 50  0000 C CNN
F 1 "EMI_Filter_CommonMode" H 7500 2550 50  0000 C CNN
F 2 "AUDIO:SIMDAD 1812" V 7500 2390 50  0001 C CNN
F 3 "" V 7500 2390 50  0000 C CNN
	1    7500 2350
	1    0    0    -1  
$EndComp
$Comp
L C C71
U 1 1 5B100915
P 8250 2200
F 0 "C71" H 8275 2300 50  0000 L CNN
F 1 "0.1" H 8275 2100 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 8288 2050 50  0001 C CNN
F 3 "" H 8250 2200 50  0001 C CNN
	1    8250 2200
	0    -1   -1   0   
$EndComp
$Comp
L C C72
U 1 1 5B100972
P 8250 2500
F 0 "C72" H 8275 2600 50  0000 L CNN
F 1 "0.1" H 8275 2400 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 8288 2350 50  0001 C CNN
F 3 "" H 8250 2500 50  0001 C CNN
	1    8250 2500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7700 2250 7800 2250
Wire Wire Line
	7800 2250 7800 2200
Wire Wire Line
	7800 2200 8100 2200
Wire Wire Line
	7700 2450 7800 2450
Wire Wire Line
	7800 2450 7800 2500
Wire Wire Line
	7800 2500 8100 2500
Wire Wire Line
	4700 2200 5600 2200
Wire Wire Line
	5900 2200 7200 2200
Wire Wire Line
	7200 2200 7200 2250
Wire Wire Line
	7200 2250 7300 2250
Wire Wire Line
	4700 2500 5600 2500
Wire Wire Line
	5900 2500 7200 2500
Wire Wire Line
	7200 2500 7200 2450
Wire Wire Line
	7200 2450 7300 2450
Wire Wire Line
	5350 2700 5200 2700
Wire Wire Line
	5200 2700 5200 2500
Connection ~ 5200 2500
Wire Wire Line
	5200 2200 5200 2000
Wire Wire Line
	5200 2000 5350 2000
Connection ~ 5200 2200
Wire Wire Line
	5650 2000 5850 2000
Wire Wire Line
	6150 2000 6300 2000
Wire Wire Line
	6300 2000 6300 2200
Connection ~ 6300 2200
Wire Wire Line
	6150 2700 6300 2700
Wire Wire Line
	6300 2700 6300 2500
Connection ~ 6300 2500
Wire Wire Line
	5650 2700 5850 2700
$Comp
L C C63
U 1 1 5B100D78
P 6000 1700
F 0 "C63" H 6025 1800 50  0000 L CNN
F 1 "47" H 6025 1600 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6038 1550 50  0001 C CNN
F 3 "" H 6000 1700 50  0001 C CNN
	1    6000 1700
	0    -1   -1   0   
$EndComp
$Comp
L C C64
U 1 1 5B100E0F
P 6000 3000
F 0 "C64" H 6025 3100 50  0000 L CNN
F 1 "47" H 6025 2900 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6038 2850 50  0001 C CNN
F 3 "" H 6000 3000 50  0001 C CNN
	1    6000 3000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5750 2000 5750 1700
Wire Wire Line
	5750 1700 5850 1700
Connection ~ 5750 2000
Wire Wire Line
	5850 3000 5750 3000
Wire Wire Line
	5750 3000 5750 2700
Connection ~ 5750 2700
$Comp
L GND #PWR055
U 1 1 5B100EC1
P 6300 1700
F 0 "#PWR055" H 6300 1450 50  0001 C CNN
F 1 "GND" H 6300 1550 50  0000 C CNN
F 2 "" H 6300 1700 50  0001 C CNN
F 3 "" H 6300 1700 50  0001 C CNN
	1    6300 1700
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR056
U 1 1 5B100EED
P 6300 3000
F 0 "#PWR056" H 6300 2750 50  0001 C CNN
F 1 "GND" H 6300 2850 50  0000 C CNN
F 2 "" H 6300 3000 50  0001 C CNN
F 3 "" H 6300 3000 50  0001 C CNN
	1    6300 3000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	6150 3000 6300 3000
Wire Wire Line
	6150 1700 6300 1700
Wire Wire Line
	8400 2200 9200 2200
Wire Wire Line
	8400 2500 9200 2500
Text HLabel 9200 2200 2    60   BiDi ~ 0
DA+
Text HLabel 9200 2500 2    60   BiDi ~ 0
DA-
Wire Wire Line
	3500 2200 2100 2200
Wire Wire Line
	2100 2300 3500 2300
Wire Wire Line
	2100 2400 3500 2400
Wire Wire Line
	3500 2800 2100 2800
Wire Wire Line
	3500 3000 2100 3000
Wire Wire Line
	3500 3100 2100 3100
Wire Wire Line
	3500 3200 2100 3200
Wire Wire Line
	3500 3800 2100 3800
Wire Wire Line
	3500 3900 2100 3900
Text HLabel 2100 2200 0    60   Output ~ 0
REFCLK
Text HLabel 2100 2300 0    60   Output ~ 0
RXD0
Text HLabel 2100 2400 0    60   Output ~ 0
RXD1
Text HLabel 2100 2800 0    60   Output ~ 0
RXDV
Text HLabel 2100 3000 0    60   Input ~ 0
TXEN
Text HLabel 2100 3100 0    60   Input ~ 0
TXD0
Text HLabel 2100 3200 0    60   Input ~ 0
TXD1
Text HLabel 2100 3800 0    60   Input ~ 0
MDC
Text HLabel 2100 3900 0    60   BiDi ~ 0
MDIO
$Comp
L R R25
U 1 1 5B1054C0
P 8600 2850
F 0 "R25" V 8680 2850 50  0000 C CNN
F 1 "1k" V 8600 2850 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8530 2850 50  0001 C CNN
F 3 "" H 8600 2850 50  0001 C CNN
	1    8600 2850
	-1   0    0    1   
$EndComp
$Comp
L R R26
U 1 1 5B10553B
P 8900 2850
F 0 "R26" V 8980 2850 50  0000 C CNN
F 1 "1k" V 8900 2850 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8830 2850 50  0001 C CNN
F 3 "" H 8900 2850 50  0001 C CNN
	1    8900 2850
	-1   0    0    1   
$EndComp
$Comp
L R R27
U 1 1 5B105596
P 8900 3350
F 0 "R27" V 8980 3350 50  0000 C CNN
F 1 "100k" V 8900 3350 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 8830 3350 50  0001 C CNN
F 3 "" H 8900 3350 50  0001 C CNN
	1    8900 3350
	-1   0    0    1   
$EndComp
$Comp
L C C73
U 1 1 5B1055E9
P 8600 3350
F 0 "C73" H 8350 3550 50  0000 L CNN
F 1 "4.7nF" H 8350 3450 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 8638 3200 50  0001 C CNN
F 3 "" H 8600 3350 50  0001 C CNN
	1    8600 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	8600 2700 8600 2200
Connection ~ 8600 2200
Wire Wire Line
	8900 2700 8900 2500
Connection ~ 8900 2500
Wire Wire Line
	8600 3000 8600 3200
Wire Wire Line
	8900 3000 8900 3200
Wire Wire Line
	8600 3100 8900 3100
Connection ~ 8900 3100
Connection ~ 8600 3100
$Comp
L GND #PWR057
U 1 1 5B1057C4
P 8900 3600
F 0 "#PWR057" H 8900 3350 50  0001 C CNN
F 1 "GND" H 8900 3450 50  0000 C CNN
F 2 "" H 8900 3600 50  0001 C CNN
F 3 "" H 8900 3600 50  0001 C CNN
	1    8900 3600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR058
U 1 1 5B1057F8
P 8600 3600
F 0 "#PWR058" H 8600 3350 50  0001 C CNN
F 1 "GND" H 8600 3450 50  0000 C CNN
F 2 "" H 8600 3600 50  0001 C CNN
F 3 "" H 8600 3600 50  0001 C CNN
	1    8600 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	8600 3600 8600 3500
Wire Wire Line
	8900 3600 8900 3500
$Comp
L +12V #PWR059
U 1 1 5B1060C5
P 4900 2800
F 0 "#PWR059" H 4900 2650 50  0001 C CNN
F 1 "+12V" H 4900 2940 50  0000 C CNN
F 2 "" H 4900 2800 50  0001 C CNN
F 3 "" H 4900 2800 50  0001 C CNN
	1    4900 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 2900 4900 2900
Wire Wire Line
	4900 2900 4900 2800
Wire Wire Line
	4700 3100 5100 3100
Text HLabel 5100 3100 2    60   Output ~ 0
PWR_ON
Wire Wire Line
	3500 4100 2100 4100
Wire Wire Line
	3500 4200 2100 4200
Text HLabel 2100 4100 0    60   Input ~ 0
~RESET
Text HLabel 2100 4200 0    60   Input ~ 0
WAKE
$Comp
L C C58
U 1 1 5B10A858
P 3400 5050
F 0 "C58" H 3425 5150 50  0000 L CNN
F 1 "C" H 3425 4950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 3438 4900 50  0001 C CNN
F 3 "" H 3400 5050 50  0001 C CNN
	1    3400 5050
	1    0    0    -1  
$EndComp
$Comp
L C C57
U 1 1 5B10A8CB
P 2700 5050
F 0 "C57" H 2725 5150 50  0000 L CNN
F 1 "C" H 2725 4950 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2738 4900 50  0001 C CNN
F 3 "" H 2700 5050 50  0001 C CNN
	1    2700 5050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR060
U 1 1 5B10A920
P 3400 5300
F 0 "#PWR060" H 3400 5050 50  0001 C CNN
F 1 "GND" H 3400 5150 50  0000 C CNN
F 2 "" H 3400 5300 50  0001 C CNN
F 3 "" H 3400 5300 50  0001 C CNN
	1    3400 5300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR061
U 1 1 5B10A95A
P 2700 5300
F 0 "#PWR061" H 2700 5050 50  0001 C CNN
F 1 "GND" H 2700 5150 50  0000 C CNN
F 2 "" H 2700 5300 50  0001 C CNN
F 3 "" H 2700 5300 50  0001 C CNN
	1    2700 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 4500 3400 4900
Wire Wire Line
	3400 4500 3500 4500
Wire Wire Line
	2700 4400 2700 4900
Wire Wire Line
	2700 4400 3500 4400
Wire Wire Line
	2900 4800 2700 4800
Connection ~ 2700 4800
Wire Wire Line
	3200 4800 3400 4800
Connection ~ 3400 4800
Wire Wire Line
	2700 5300 2700 5200
Wire Wire Line
	3400 5300 3400 5200
$Comp
L C C59
U 1 1 5B10AE87
P 5000 4250
F 0 "C59" H 5025 4350 50  0000 L CNN
F 1 "0.47" H 5025 4150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5038 4100 50  0001 C CNN
F 3 "" H 5000 4250 50  0001 C CNN
	1    5000 4250
	1    0    0    -1  
$EndComp
$Comp
L C C60
U 1 1 5B10AF02
P 5300 4250
F 0 "C60" H 5325 4350 50  0000 L CNN
F 1 "0.47" H 5325 4150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5338 4100 50  0001 C CNN
F 3 "" H 5300 4250 50  0001 C CNN
	1    5300 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 4500 5300 4400
Wire Wire Line
	5000 4500 5000 4400
Connection ~ 5000 4500
Wire Wire Line
	4700 4000 5000 4000
Wire Wire Line
	5000 4000 5000 4100
Wire Wire Line
	4700 3900 5300 3900
Wire Wire Line
	5300 3900 5300 4100
$Comp
L C C65
U 1 1 5B16BDB0
P 6200 4250
F 0 "C65" H 6225 4350 50  0000 L CNN
F 1 "1.0" H 6225 4150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6238 4100 50  0001 C CNN
F 3 "" H 6200 4250 50  0001 C CNN
	1    6200 4250
	1    0    0    -1  
$EndComp
$Comp
L C C66
U 1 1 5B16BE21
P 6500 4250
F 0 "C66" H 6525 4350 50  0000 L CNN
F 1 "0.1" H 6525 4150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6538 4100 50  0001 C CNN
F 3 "" H 6500 4250 50  0001 C CNN
	1    6500 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 3700 6200 4100
$Comp
L C C67
U 1 1 5B16BF61
P 6800 4250
F 0 "C67" H 6825 4350 50  0000 L CNN
F 1 "0.1" H 6825 4150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 6838 4100 50  0001 C CNN
F 3 "" H 6800 4250 50  0001 C CNN
	1    6800 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 4000 6800 4000
Wire Wire Line
	6800 4000 6800 4100
Connection ~ 6200 4000
Wire Wire Line
	6500 4100 6500 4000
Connection ~ 6500 4000
Wire Wire Line
	6200 4500 6200 4400
Connection ~ 5300 4500
Wire Wire Line
	6500 4500 6500 4400
Connection ~ 6200 4500
Wire Wire Line
	6800 4500 6800 4400
Connection ~ 6500 4500
$Comp
L C C68
U 1 1 5B16C252
P 7100 4250
F 0 "C68" H 7125 4350 50  0000 L CNN
F 1 "1.0" H 7125 4150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 7138 4100 50  0001 C CNN
F 3 "" H 7100 4250 50  0001 C CNN
	1    7100 4250
	1    0    0    -1  
$EndComp
$Comp
L C C69
U 1 1 5B16C2B7
P 7400 4250
F 0 "C69" H 7425 4350 50  0000 L CNN
F 1 "0.1" H 7425 4150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 7438 4100 50  0001 C CNN
F 3 "" H 7400 4250 50  0001 C CNN
	1    7400 4250
	1    0    0    -1  
$EndComp
$Comp
L C C70
U 1 1 5B16C30E
P 7700 4250
F 0 "C70" H 7725 4350 50  0000 L CNN
F 1 "0.1" H 7725 4150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 7738 4100 50  0001 C CNN
F 3 "" H 7700 4250 50  0001 C CNN
	1    7700 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7100 3500 7100 4100
Wire Wire Line
	7100 4000 7700 4000
Wire Wire Line
	7400 4000 7400 4100
Connection ~ 7100 4000
Wire Wire Line
	7700 4000 7700 4100
Connection ~ 7400 4000
Wire Wire Line
	7100 4500 7100 4400
Connection ~ 6800 4500
Wire Wire Line
	7400 4500 7400 4400
Connection ~ 7100 4500
Wire Wire Line
	7700 4500 7700 4400
Connection ~ 7400 4500
$Comp
L L L14
U 1 1 5B16C8E7
P 7450 3700
F 0 "L14" V 7400 3700 50  0000 C CNN
F 1 "L" V 7525 3700 50  0000 C CNN
F 2 "Inductors_SMD:L_0805" H 7450 3700 50  0001 C CNN
F 3 "" H 7450 3700 50  0001 C CNN
	1    7450 3700
	0    -1   -1   0   
$EndComp
$Comp
L L L13
U 1 1 5B16C9B8
P 7450 3500
F 0 "L13" V 7400 3500 50  0000 C CNN
F 1 "L" V 7525 3500 50  0000 C CNN
F 2 "Inductors_SMD:L_0805" H 7450 3500 50  0001 C CNN
F 3 "" H 7450 3500 50  0001 C CNN
	1    7450 3500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4700 3700 7300 3700
Connection ~ 6200 3700
Wire Wire Line
	4700 3500 7300 3500
Connection ~ 7100 3500
Wire Wire Line
	4800 3700 4800 3600
Wire Wire Line
	4800 3600 4700 3600
Connection ~ 4800 3700
Wire Wire Line
	4800 3500 4800 3400
Wire Wire Line
	4800 3400 4700 3400
Connection ~ 4800 3500
$Comp
L C C61
U 1 1 5B16D250
P 5600 4250
F 0 "C61" H 5625 4350 50  0000 L CNN
F 1 "1.0" H 5625 4150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5638 4100 50  0001 C CNN
F 3 "" H 5600 4250 50  0001 C CNN
	1    5600 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 3300 5600 4100
Wire Wire Line
	4700 3300 7300 3300
Wire Wire Line
	5600 4500 5600 4400
Connection ~ 5600 4500
$Comp
L L L12
U 1 1 5B16D4AE
P 7450 3300
F 0 "L12" V 7400 3300 50  0000 C CNN
F 1 "L" V 7525 3300 50  0000 C CNN
F 2 "Inductors_SMD:L_0805" H 7450 3300 50  0001 C CNN
F 3 "" H 7450 3300 50  0001 C CNN
	1    7450 3300
	0    -1   -1   0   
$EndComp
Connection ~ 5600 3300
$Comp
L +3.3V #PWR062
U 1 1 5B16D6CA
P 7800 3200
F 0 "#PWR062" H 7800 3050 50  0001 C CNN
F 1 "+3.3V" H 7800 3340 50  0000 C CNN
F 2 "" H 7800 3200 50  0001 C CNN
F 3 "" H 7800 3200 50  0001 C CNN
	1    7800 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	7800 3700 7600 3700
Wire Wire Line
	7800 3200 7800 3700
Wire Wire Line
	7600 3300 7800 3300
Connection ~ 7800 3300
Wire Wire Line
	7600 3500 7800 3500
Connection ~ 7800 3500
$Comp
L ESD-SOT23 VD1
U 1 1 5B16E172
P 7500 1700
F 0 "VD1" H 7500 2100 60  0000 C CNN
F 1 "ESD-SOT23" H 7500 2000 60  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 7500 1700 60  0001 C CNN
F 3 "" H 7500 1700 60  0001 C CNN
	1    7500 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 2500 6900 1800
Wire Wire Line
	6900 1800 7000 1800
Connection ~ 6900 2500
Wire Wire Line
	6800 2200 6800 1600
Wire Wire Line
	6800 1600 7000 1600
Connection ~ 6800 2200
$Comp
L GND #PWR063
U 1 1 5B16E4B5
P 8100 1800
F 0 "#PWR063" H 8100 1550 50  0001 C CNN
F 1 "GND" H 8100 1650 50  0000 C CNN
F 2 "" H 8100 1800 50  0001 C CNN
F 3 "" H 8100 1800 50  0001 C CNN
	1    8100 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 1700 8100 1700
Wire Wire Line
	8100 1700 8100 1800
$Comp
L C C62
U 1 1 5B16E989
P 5900 4250
F 0 "C62" H 5925 4350 50  0000 L CNN
F 1 "0.1" H 5925 4150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 5938 4100 50  0001 C CNN
F 3 "" H 5900 4250 50  0001 C CNN
	1    5900 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5900 4500 5900 4400
Connection ~ 5900 4500
Wire Wire Line
	5600 4000 5900 4000
Wire Wire Line
	5900 4000 5900 4100
Connection ~ 5600 4000
$Comp
L R R14
U 1 1 5B16EFDF
P 2300 1950
F 0 "R14" V 2380 1950 50  0000 C CNN
F 1 "10k" V 2300 1950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2230 1950 50  0001 C CNN
F 3 "" H 2300 1950 50  0001 C CNN
	1    2300 1950
	-1   0    0    1   
$EndComp
$Comp
L R R16
U 1 1 5B16F04C
P 2500 1950
F 0 "R16" V 2580 1950 50  0000 C CNN
F 1 "10k" V 2500 1950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2430 1950 50  0001 C CNN
F 3 "" H 2500 1950 50  0001 C CNN
	1    2500 1950
	-1   0    0    1   
$EndComp
Wire Wire Line
	2300 2300 2300 2100
Connection ~ 2300 2300
Wire Wire Line
	2500 2400 2500 2100
Connection ~ 2500 2400
$Comp
L R R17
U 1 1 5B16F260
P 2700 1950
F 0 "R17" V 2780 1950 50  0000 C CNN
F 1 "10k" V 2700 1950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2630 1950 50  0001 C CNN
F 3 "" H 2700 1950 50  0001 C CNN
	1    2700 1950
	-1   0    0    1   
$EndComp
$Comp
L R R18
U 1 1 5B16F2FB
P 2900 1950
F 0 "R18" V 2980 1950 50  0000 C CNN
F 1 "10k" V 2900 1950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2830 1950 50  0001 C CNN
F 3 "" H 2900 1950 50  0001 C CNN
	1    2900 1950
	-1   0    0    1   
$EndComp
$Comp
L R R19
U 1 1 5B16F364
P 3100 1950
F 0 "R19" V 3180 1950 50  0000 C CNN
F 1 "10k" V 3100 1950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3030 1950 50  0001 C CNN
F 3 "" H 3100 1950 50  0001 C CNN
	1    3100 1950
	-1   0    0    1   
$EndComp
$Comp
L R R20
U 1 1 5B16F492
P 3300 1950
F 0 "R20" V 3380 1950 50  0000 C CNN
F 1 "10k" V 3300 1950 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 3230 1950 50  0001 C CNN
F 3 "" H 3300 1950 50  0001 C CNN
	1    3300 1950
	-1   0    0    1   
$EndComp
Wire Wire Line
	2700 2100 2700 2500
Wire Wire Line
	2700 2500 3500 2500
Wire Wire Line
	2900 2100 2900 2600
Wire Wire Line
	2900 2600 3500 2600
Wire Wire Line
	3100 2100 3100 2700
Wire Wire Line
	3100 2700 3500 2700
Wire Wire Line
	3300 2100 3300 2800
Connection ~ 3300 2800
$Comp
L GND #PWR064
U 1 1 5B16FCF5
P 2000 1800
F 0 "#PWR064" H 2000 1550 50  0001 C CNN
F 1 "GND" H 2000 1650 50  0000 C CNN
F 2 "" H 2000 1800 50  0001 C CNN
F 3 "" H 2000 1800 50  0001 C CNN
	1    2000 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 1800 2000 1700
Wire Wire Line
	2000 1700 3300 1700
Wire Wire Line
	2300 1700 2300 1800
Wire Wire Line
	2500 1700 2500 1800
Connection ~ 2300 1700
Wire Wire Line
	2900 1700 2900 1800
Connection ~ 2500 1700
Wire Wire Line
	3300 1700 3300 1800
Connection ~ 2900 1700
$Comp
L +3.3V #PWR065
U 1 1 5B170550
P 2000 1500
F 0 "#PWR065" H 2000 1350 50  0001 C CNN
F 1 "+3.3V" H 2000 1640 50  0000 C CNN
F 2 "" H 2000 1500 50  0001 C CNN
F 3 "" H 2000 1500 50  0001 C CNN
	1    2000 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 1500 2000 1600
Wire Wire Line
	2000 1600 3100 1600
Wire Wire Line
	3100 1600 3100 1800
Wire Wire Line
	2700 1800 2700 1700
Connection ~ 2700 1700
$Comp
L R R15
U 1 1 5B187D6A
P 2300 4550
F 0 "R15" V 2380 4550 50  0000 C CNN
F 1 "1.5k" V 2300 4550 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" V 2230 4550 50  0001 C CNN
F 3 "" H 2300 4550 50  0001 C CNN
	1    2300 4550
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR066
U 1 1 5B187DFB
P 2300 4800
F 0 "#PWR066" H 2300 4550 50  0001 C CNN
F 1 "GND" H 2300 4650 50  0000 C CNN
F 2 "" H 2300 4800 50  0001 C CNN
F 3 "" H 2300 4800 50  0001 C CNN
	1    2300 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 4800 2300 4700
Wire Wire Line
	2300 4400 2300 4200
Connection ~ 2300 4200
$Comp
L Crystal_GND24 Z2
U 1 1 5B1FA5C6
P 3050 4800
F 0 "Z2" H 3150 5100 50  0000 L CNN
F 1 "25 MHz" H 3100 5000 50  0000 L CNN
F 2 "AUDIO:Crystal_5x3" H 3050 4800 50  0001 C CNN
F 3 "" H 3050 4800 50  0001 C CNN
	1    3050 4800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR067
U 1 1 5B1FA66B
P 3050 5300
F 0 "#PWR067" H 3050 5050 50  0001 C CNN
F 1 "GND" H 3050 5150 50  0000 C CNN
F 2 "" H 3050 5300 50  0001 C CNN
F 3 "" H 3050 5300 50  0001 C CNN
	1    3050 5300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR068
U 1 1 5B1FA6CF
P 2500 5300
F 0 "#PWR068" H 2500 5050 50  0001 C CNN
F 1 "GND" H 2500 5150 50  0000 C CNN
F 2 "" H 2500 5300 50  0001 C CNN
F 3 "" H 2500 5300 50  0001 C CNN
	1    2500 5300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 5300 3050 5000
Wire Wire Line
	3050 4600 3050 4500
Wire Wire Line
	3050 4500 2500 4500
Wire Wire Line
	2500 4500 2500 5300
$EndSCHEMATC
