EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Pocket Voltex"
Date "2019-07-07"
Rev "8"
Comp "mon.im"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector_Generic:Conn_01x06 J1
U 1 1 5A9D6366
P 2950 1250
F 0 "J1" H 2950 1550 50  0000 C CNN
F 1 "Programming header" H 2950 850 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x03_P2.54mm_Vertical" H 2950 1250 50  0001 C CNN
F 3 "" H 2950 1250 50  0001 C CNN
	1    2950 1250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5A9D65B2
P 2700 4550
F 0 "C2" H 2725 4650 50  0000 L CNN
F 1 "10uF" H 2725 4450 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2738 4400 50  0001 C CNN
F 3 "" H 2700 4550 50  0001 C CNN
F 4 "CC0603MRX5R5BB106" H 2700 4550 50  0001 C CNN "Part number"
	1    2700 4550
	1    0    0    -1  
$EndComp
$Comp
L Device:Rotary_Encoder ENC1
U 1 1 5A9D6825
P 8250 1300
F 0 "ENC1" H 8250 1560 50  0000 C CNN
F 1 "VOL_L" H 8250 1040 50  0000 C CNN
F 2 "Pocket Voltex:Encoder_Bourns_PEC16_hotswap" H 8150 1460 50  0001 C CNN
F 3 "" H 8250 1560 50  0001 C CNN
	1    8250 1300
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW8
U 1 1 5A9D74BE
P 7700 6100
F 0 "SW8" H 7750 6200 50  0000 L CNN
F 1 "SW_Push" H 7700 6040 50  0000 C CNN
F 2 "Pocket Voltex:SW_SPST_KSC-J" H 7700 6300 50  0001 C CNN
F 3 "" H 7700 6300 50  0001 C CNN
F 4 "KSC521J" H 7700 6100 50  0001 C CNN "Part number"
F 5 "https://www.mouser.com/ProductDetail/CK/KSC521J-ROHS" H 7700 6100 50  0001 C CNN "Link"
	1    7700 6100
	1    0    0    -1  
$EndComp
$Comp
L PocketVoltex:SK9822 D1
U 1 1 5A9D7030
P 1800 5900
F 0 "D1" H 1800 6000 60  0000 C CNN
F 1 "SK9822" H 1800 5900 60  0000 C CNN
F 2 "Pocket Voltex:LED_SK9822" H 1800 5900 60  0001 C CNN
F 3 "" H 1800 5900 60  0001 C CNN
F 4 "SK9822" H 1800 5900 50  0001 C CNN "Part number"
F 5 "https://www.aliexpress.com/item/32829381843.html" H 1800 5900 50  0001 C CNN "Link"
	1    1800 5900
	1    0    0    -1  
$EndComp
$Comp
L PocketVoltex:SK9822 D2
U 1 1 5A9D785F
P 1800 6950
F 0 "D2" H 1800 7050 60  0000 C CNN
F 1 "SK9822" H 1800 6950 60  0000 C CNN
F 2 "Pocket Voltex:LED_SK9822" H 1800 6950 60  0001 C CNN
F 3 "" H 1800 6950 60  0001 C CNN
F 4 "SK9822" H 1800 6950 50  0001 C CNN "Part number"
F 5 "https://www.aliexpress.com/item/32829381843.html" H 1800 6950 50  0001 C CNN "Link"
	1    1800 6950
	-1   0    0    1   
$EndComp
$Comp
L Connector:USB_C_Receptacle_USB2.0 J2
U 1 1 5A9D95F5
P 1350 3450
F 0 "J2" H 950 4200 50  0000 L CNN
F 1 "USB_C_Receptacle" H 1750 4200 50  0000 R CNN
F 2 "Pocket Voltex:USB_C_Receptacle_GCT_USB4085" H 1500 3450 50  0001 C CNN
F 3 "" H 1500 3450 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/gct/USB4085-GF-A/2073-USB4085-GF-ACT-ND/9859733" H 1350 3450 50  0001 C CNN "Link"
F 5 "USB4085-GF-A" H 1350 3450 50  0001 C CNN "Part number"
	1    1350 3450
	1    0    0    -1  
$EndComp
NoConn ~ 1950 4050
NoConn ~ 1950 3950
$Comp
L power:GND #PWR01
U 1 1 5A9DD074
P 1200 4450
F 0 "#PWR01" H 1200 4200 50  0001 C CNN
F 1 "GND" H 1200 4300 50  0000 C CNN
F 2 "" H 1200 4450 50  0001 C CNN
F 3 "" H 1200 4450 50  0001 C CNN
	1    1200 4450
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR02
U 1 1 5A9DDEF1
P 1950 2850
F 0 "#PWR02" H 1950 2700 50  0001 C CNN
F 1 "VCC" H 1950 3000 50  0000 C CNN
F 2 "" H 1950 2850 50  0001 C CNN
F 3 "" H 1950 2850 50  0001 C CNN
	1    1950 2850
	0    1    1    0   
$EndComp
$Comp
L Device:R R3
U 1 1 5A9E01EE
P 3350 2200
F 0 "R3" V 3430 2200 50  0000 C CNN
F 1 "10k" V 3350 2200 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3280 2200 50  0001 C CNN
F 3 "" H 3350 2200 50  0001 C CNN
F 4 "RC0603JR-0710KL" H 3350 2200 50  0001 C CNN "Part number"
	1    3350 2200
	0    -1   -1   0   
$EndComp
$Comp
L power:VCC #PWR03
U 1 1 5A9E05F9
P 3200 2200
F 0 "#PWR03" H 3200 2050 50  0001 C CNN
F 1 "VCC" H 3200 2350 50  0000 C CNN
F 2 "" H 3200 2200 50  0001 C CNN
F 3 "" H 3200 2200 50  0001 C CNN
	1    3200 2200
	0    -1   -1   0   
$EndComp
Text Label 3350 2050 0    60   ~ 0
RESET
Text Label 2200 3400 2    60   ~ 0
D-
Text Label 2200 3600 2    60   ~ 0
D+
$Comp
L power:GND #PWR08
U 1 1 5A9E8043
P 2600 1550
F 0 "#PWR08" H 2600 1300 50  0001 C CNN
F 1 "GND" H 2600 1400 50  0000 C CNN
F 2 "" H 2600 1550 50  0001 C CNN
F 3 "" H 2600 1550 50  0001 C CNN
	1    2600 1550
	0    1    1    0   
$EndComp
$Comp
L power:VCC #PWR09
U 1 1 5A9E8095
P 2550 1150
F 0 "#PWR09" H 2550 1000 50  0001 C CNN
F 1 "VCC" H 2550 1300 50  0000 C CNN
F 2 "" H 2550 1150 50  0001 C CNN
F 3 "" H 2550 1150 50  0001 C CNN
	1    2550 1150
	0    -1   -1   0   
$EndComp
Text Label 2450 1450 0    60   ~ 0
RESET
Text Label 2450 1050 0    60   ~ 0
MISO
Text Label 2450 1250 0    60   ~ 0
SCLK
Text Label 2450 1350 0    60   ~ 0
MOSI
$Comp
L Device:C_Small C3
U 1 1 5A9F13E2
P 7150 1200
F 0 "C3" H 7250 1200 50  0000 L CNN
F 1 "10nF" H 6900 1200 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7150 1200 50  0001 C CNN
F 3 "" H 7150 1200 50  0001 C CNN
F 4 "CC0603KRX7R9BB103" H 7150 1200 50  0001 C CNN "Part number"
	1    7150 1200
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C4
U 1 1 5A9F150E
P 7150 1400
F 0 "C4" H 7250 1400 50  0000 L CNN
F 1 "10nF" H 6900 1400 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7150 1400 50  0001 C CNN
F 3 "" H 7150 1400 50  0001 C CNN
F 4 "CC0603KRX7R9BB103" H 7150 1400 50  0001 C CNN "Part number"
	1    7150 1400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR010
U 1 1 5A9F1623
P 7000 1300
F 0 "#PWR010" H 7000 1050 50  0001 C CNN
F 1 "GND" H 7000 1150 50  0000 C CNN
F 2 "" H 7000 1300 50  0001 C CNN
F 3 "" H 7000 1300 50  0001 C CNN
	1    7000 1300
	0    1    1    0   
$EndComp
Text Label 7500 1100 2    60   ~ 0
ENC1A
Text Label 7500 1550 2    60   ~ 0
ENC1B
$Comp
L Device:Rotary_Encoder ENC2
U 1 1 5A9F2BB5
P 8250 2500
F 0 "ENC2" H 8250 2760 50  0000 C CNN
F 1 "VOL_R" H 8250 2240 50  0000 C CNN
F 2 "Pocket Voltex:Encoder_Bourns_PEC16_hotswap" H 8150 2660 50  0001 C CNN
F 3 "" H 8250 2760 50  0001 C CNN
	1    8250 2500
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C5
U 1 1 5A9F2BBB
P 7150 2400
F 0 "C5" H 7250 2400 50  0000 L CNN
F 1 "10nF" H 6900 2400 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7150 2400 50  0001 C CNN
F 3 "" H 7150 2400 50  0001 C CNN
F 4 "CC0603KRX7R9BB103" H 7150 2400 50  0001 C CNN "Part number"
	1    7150 2400
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C6
U 1 1 5A9F2BC1
P 7150 2600
F 0 "C6" H 7250 2600 50  0000 L CNN
F 1 "10nF" H 6900 2600 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 7150 2600 50  0001 C CNN
F 3 "" H 7150 2600 50  0001 C CNN
F 4 "CC0603KRX7R9BB103" H 7150 2600 50  0001 C CNN "Part number"
	1    7150 2600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR011
U 1 1 5A9F2BC7
P 7000 2500
F 0 "#PWR011" H 7000 2250 50  0001 C CNN
F 1 "GND" H 7000 2350 50  0000 C CNN
F 2 "" H 7000 2500 50  0001 C CNN
F 3 "" H 7000 2500 50  0001 C CNN
	1    7000 2500
	0    1    1    0   
$EndComp
Text Label 7500 2300 2    60   ~ 0
ENC2A
Text Label 7500 2750 2    60   ~ 0
ENC2B
Text Label 6950 3150 0    60   ~ 0
SW_START
Text Label 7150 3550 0    60   ~ 0
SW_A
Text Label 7150 3950 0    60   ~ 0
SW_B
Text Label 7150 4350 0    60   ~ 0
SW_C
Text Label 7150 4750 0    60   ~ 0
SW_D
Text Label 7050 5150 0    60   ~ 0
SW_FXL
Text Label 7050 5550 0    60   ~ 0
SW_FXR
Text Label 6950 6100 0    60   ~ 0
SW_MACRO
$Comp
L power:GND #PWR012
U 1 1 5A9F9807
P 7900 6100
F 0 "#PWR012" H 7900 5850 50  0001 C CNN
F 1 "GND" H 7900 5950 50  0000 C CNN
F 2 "" H 7900 6100 50  0001 C CNN
F 3 "" H 7900 6100 50  0001 C CNN
	1    7900 6100
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR013
U 1 1 5A9FC86F
P 2700 4700
F 0 "#PWR013" H 2700 4450 50  0001 C CNN
F 1 "GND" H 2700 4550 50  0000 C CNN
F 2 "" H 2700 4700 50  0001 C CNN
F 3 "" H 2700 4700 50  0001 C CNN
	1    2700 4700
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR014
U 1 1 5A9FC8C1
P 2700 4400
F 0 "#PWR014" H 2700 4250 50  0001 C CNN
F 1 "VCC" H 2700 4550 50  0000 C CNN
F 2 "" H 2700 4400 50  0001 C CNN
F 3 "" H 2700 4400 50  0001 C CNN
	1    2700 4400
	1    0    0    -1  
$EndComp
$Comp
L PocketVoltex:SK9822 D3
U 1 1 5A9FEAE9
P 2900 5900
F 0 "D3" H 2900 6000 60  0000 C CNN
F 1 "SK9822" H 2900 5900 60  0000 C CNN
F 2 "Pocket Voltex:LED_SK9822" H 2900 5900 60  0001 C CNN
F 3 "" H 2900 5900 60  0001 C CNN
F 4 "SK9822" H 2900 5900 50  0001 C CNN "Part number"
F 5 "https://www.aliexpress.com/item/32829381843.html" H 2900 5900 50  0001 C CNN "Link"
	1    2900 5900
	1    0    0    -1  
$EndComp
$Comp
L PocketVoltex:SK9822 D4
U 1 1 5A9FEAEF
P 2900 6950
F 0 "D4" H 2900 7050 60  0000 C CNN
F 1 "SK9822" H 2900 6950 60  0000 C CNN
F 2 "Pocket Voltex:LED_SK9822" H 2900 6950 60  0001 C CNN
F 3 "" H 2900 6950 60  0001 C CNN
F 4 "SK9822" H 2900 6950 50  0001 C CNN "Part number"
F 5 "https://www.aliexpress.com/item/32829381843.html" H 2900 6950 50  0001 C CNN "Link"
	1    2900 6950
	-1   0    0    1   
$EndComp
$Comp
L PocketVoltex:SK9822 D5
U 1 1 5A9FEF13
P 4000 5900
F 0 "D5" H 4000 6000 60  0000 C CNN
F 1 "SK9822" H 4000 5900 60  0000 C CNN
F 2 "Pocket Voltex:LED_SK9822" H 4000 5900 60  0001 C CNN
F 3 "" H 4000 5900 60  0001 C CNN
F 4 "SK9822" H 4000 5900 50  0001 C CNN "Part number"
F 5 "https://www.aliexpress.com/item/32829381843.html" H 4000 5900 50  0001 C CNN "Link"
	1    4000 5900
	1    0    0    -1  
$EndComp
$Comp
L PocketVoltex:SK9822 D6
U 1 1 5A9FEF19
P 4000 6950
F 0 "D6" H 4000 7050 60  0000 C CNN
F 1 "SK9822" H 4000 6950 60  0000 C CNN
F 2 "Pocket Voltex:LED_SK9822" H 4000 6950 60  0001 C CNN
F 3 "" H 4000 6950 60  0001 C CNN
F 4 "SK9822" H 4000 6950 50  0001 C CNN "Part number"
F 5 "https://www.aliexpress.com/item/32829381843.html" H 4000 6950 50  0001 C CNN "Link"
	1    4000 6950
	-1   0    0    1   
$EndComp
$Comp
L PocketVoltex:SK9822 D7
U 1 1 5A9FEF1F
P 5100 5900
F 0 "D7" H 5100 6000 60  0000 C CNN
F 1 "SK9822" H 5100 5900 60  0000 C CNN
F 2 "Pocket Voltex:LED_SK9822" H 5100 5900 60  0001 C CNN
F 3 "" H 5100 5900 60  0001 C CNN
F 4 "SK9822" H 5100 5900 50  0001 C CNN "Part number"
F 5 "https://www.aliexpress.com/item/32829381843.html" H 5100 5900 50  0001 C CNN "Link"
	1    5100 5900
	1    0    0    -1  
$EndComp
$Comp
L PocketVoltex:SK9822 D8
U 1 1 5A9FEF25
P 5100 6950
F 0 "D8" H 5100 7050 60  0000 C CNN
F 1 "SK9822" H 5100 6950 60  0000 C CNN
F 2 "Pocket Voltex:LED_SK9822" H 5100 6950 60  0001 C CNN
F 3 "" H 5100 6950 60  0001 C CNN
F 4 "SK9822" H 5100 6950 50  0001 C CNN "Part number"
F 5 "https://www.aliexpress.com/item/32829381843.html" H 5100 6950 50  0001 C CNN "Link"
	1    5100 6950
	-1   0    0    1   
$EndComp
NoConn ~ 1350 6750
NoConn ~ 1350 6850
Wire Wire Line
	3500 2200 3700 2200
Wire Wire Line
	3700 2200 3700 2050
Wire Wire Line
	3700 2050 3350 2050
Wire Wire Line
	1950 3350 2050 3350
Wire Wire Line
	2050 3350 2050 3400
Wire Wire Line
	2050 3450 1950 3450
Wire Wire Line
	1950 3550 2050 3550
Wire Wire Line
	2050 3550 2050 3600
Wire Wire Line
	2050 3650 1950 3650
Wire Wire Line
	2200 3600 2050 3600
Connection ~ 2050 3600
Wire Wire Line
	2200 3400 2050 3400
Connection ~ 2050 3400
Wire Wire Line
	2750 1050 2450 1050
Wire Wire Line
	2450 1250 2750 1250
Wire Wire Line
	2750 1350 2450 1350
Wire Wire Line
	2450 1450 2750 1450
Wire Wire Line
	2750 1550 2600 1550
Wire Wire Line
	7000 1300 7150 1300
Connection ~ 7150 1300
Wire Wire Line
	7500 1400 7500 1550
Wire Wire Line
	7500 1550 7150 1550
Wire Wire Line
	7500 1200 7500 1100
Wire Wire Line
	7500 1100 7150 1100
Wire Wire Line
	7150 1550 7150 1500
Wire Wire Line
	7000 2500 7150 2500
Connection ~ 7150 2500
Wire Wire Line
	7500 2600 7500 2750
Wire Wire Line
	7500 2750 7150 2750
Wire Wire Line
	7500 2400 7500 2300
Wire Wire Line
	7500 2300 7150 2300
Wire Wire Line
	7150 2750 7150 2700
Wire Wire Line
	6950 3150 7450 3150
Wire Wire Line
	7450 3550 7150 3550
Wire Wire Line
	7450 3950 7150 3950
Wire Wire Line
	7450 4350 7150 4350
Wire Wire Line
	7450 4750 7150 4750
Wire Wire Line
	7450 5150 7050 5150
Wire Wire Line
	7450 5550 7050 5550
Wire Wire Line
	7500 6100 6950 6100
Wire Wire Line
	2250 6850 2450 6850
Wire Wire Line
	2250 6750 2450 6750
Wire Wire Line
	3350 6750 3550 6750
Wire Wire Line
	3550 6850 3350 6850
Wire Wire Line
	4450 6750 4650 6750
Wire Wire Line
	4650 6850 4450 6850
Wire Wire Line
	2250 6000 2450 6000
Wire Wire Line
	2250 6100 2450 6100
Wire Wire Line
	3350 6000 3550 6000
Wire Wire Line
	3550 6100 3350 6100
Wire Wire Line
	4450 6000 4650 6000
Wire Wire Line
	4650 6100 4450 6100
Wire Wire Line
	5550 6750 5750 6750
Wire Wire Line
	5750 6750 5750 6100
Wire Wire Line
	5750 6100 5550 6100
Wire Wire Line
	5550 6000 5850 6000
Wire Wire Line
	5850 6000 5850 6850
Wire Wire Line
	5850 6850 5550 6850
$Comp
L power:VCC #PWR015
U 1 1 5AA00CB2
P 2250 6200
F 0 "#PWR015" H 2250 6050 50  0001 C CNN
F 1 "VCC" H 2250 6350 50  0000 C CNN
F 2 "" H 2250 6200 50  0001 C CNN
F 3 "" H 2250 6200 50  0001 C CNN
	1    2250 6200
	-1   0    0    1   
$EndComp
$Comp
L power:VCC #PWR016
U 1 1 5AA00D04
P 3350 6200
F 0 "#PWR016" H 3350 6050 50  0001 C CNN
F 1 "VCC" H 3350 6350 50  0000 C CNN
F 2 "" H 3350 6200 50  0001 C CNN
F 3 "" H 3350 6200 50  0001 C CNN
	1    3350 6200
	-1   0    0    1   
$EndComp
$Comp
L power:VCC #PWR017
U 1 1 5AA00D56
P 4450 6200
F 0 "#PWR017" H 4450 6050 50  0001 C CNN
F 1 "VCC" H 4450 6350 50  0000 C CNN
F 2 "" H 4450 6200 50  0001 C CNN
F 3 "" H 4450 6200 50  0001 C CNN
	1    4450 6200
	-1   0    0    1   
$EndComp
$Comp
L power:VCC #PWR018
U 1 1 5AA00DA8
P 5550 6200
F 0 "#PWR018" H 5550 6050 50  0001 C CNN
F 1 "VCC" H 5550 6350 50  0000 C CNN
F 2 "" H 5550 6200 50  0001 C CNN
F 3 "" H 5550 6200 50  0001 C CNN
	1    5550 6200
	-1   0    0    1   
$EndComp
$Comp
L power:VCC #PWR019
U 1 1 5AA00DFA
P 1350 6650
F 0 "#PWR019" H 1350 6500 50  0001 C CNN
F 1 "VCC" H 1350 6800 50  0000 C CNN
F 2 "" H 1350 6650 50  0001 C CNN
F 3 "" H 1350 6650 50  0001 C CNN
	1    1350 6650
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR020
U 1 1 5AA00E4C
P 2450 6650
F 0 "#PWR020" H 2450 6500 50  0001 C CNN
F 1 "VCC" H 2450 6800 50  0000 C CNN
F 2 "" H 2450 6650 50  0001 C CNN
F 3 "" H 2450 6650 50  0001 C CNN
	1    2450 6650
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR021
U 1 1 5AA00E9E
P 3550 6650
F 0 "#PWR021" H 3550 6500 50  0001 C CNN
F 1 "VCC" H 3550 6800 50  0000 C CNN
F 2 "" H 3550 6650 50  0001 C CNN
F 3 "" H 3550 6650 50  0001 C CNN
	1    3550 6650
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR022
U 1 1 5AA00EF0
P 4650 6650
F 0 "#PWR022" H 4650 6500 50  0001 C CNN
F 1 "VCC" H 4650 6800 50  0000 C CNN
F 2 "" H 4650 6650 50  0001 C CNN
F 3 "" H 4650 6650 50  0001 C CNN
	1    4650 6650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR023
U 1 1 5AA00F42
P 1350 6200
F 0 "#PWR023" H 1350 5950 50  0001 C CNN
F 1 "GND" H 1350 6050 50  0000 C CNN
F 2 "" H 1350 6200 50  0001 C CNN
F 3 "" H 1350 6200 50  0001 C CNN
	1    1350 6200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR024
U 1 1 5AA00F94
P 2450 6200
F 0 "#PWR024" H 2450 5950 50  0001 C CNN
F 1 "GND" H 2450 6050 50  0000 C CNN
F 2 "" H 2450 6200 50  0001 C CNN
F 3 "" H 2450 6200 50  0001 C CNN
	1    2450 6200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR025
U 1 1 5AA00FE6
P 3550 6200
F 0 "#PWR025" H 3550 5950 50  0001 C CNN
F 1 "GND" H 3550 6050 50  0000 C CNN
F 2 "" H 3550 6200 50  0001 C CNN
F 3 "" H 3550 6200 50  0001 C CNN
	1    3550 6200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR026
U 1 1 5AA01038
P 4650 6200
F 0 "#PWR026" H 4650 5950 50  0001 C CNN
F 1 "GND" H 4650 6050 50  0000 C CNN
F 2 "" H 4650 6200 50  0001 C CNN
F 3 "" H 4650 6200 50  0001 C CNN
	1    4650 6200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR027
U 1 1 5AA0108A
P 5550 6650
F 0 "#PWR027" H 5550 6400 50  0001 C CNN
F 1 "GND" H 5550 6500 50  0000 C CNN
F 2 "" H 5550 6650 50  0001 C CNN
F 3 "" H 5550 6650 50  0001 C CNN
	1    5550 6650
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR028
U 1 1 5AA010DC
P 4450 6650
F 0 "#PWR028" H 4450 6400 50  0001 C CNN
F 1 "GND" H 4450 6500 50  0000 C CNN
F 2 "" H 4450 6650 50  0001 C CNN
F 3 "" H 4450 6650 50  0001 C CNN
	1    4450 6650
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR029
U 1 1 5AA0112E
P 3350 6650
F 0 "#PWR029" H 3350 6400 50  0001 C CNN
F 1 "GND" H 3350 6500 50  0000 C CNN
F 2 "" H 3350 6650 50  0001 C CNN
F 3 "" H 3350 6650 50  0001 C CNN
	1    3350 6650
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR030
U 1 1 5AA01180
P 2250 6650
F 0 "#PWR030" H 2250 6400 50  0001 C CNN
F 1 "GND" H 2250 6500 50  0000 C CNN
F 2 "" H 2250 6650 50  0001 C CNN
F 3 "" H 2250 6650 50  0001 C CNN
	1    2250 6650
	-1   0    0    1   
$EndComp
Text Label 1050 6000 0    60   ~ 0
MOSI
Text Label 1050 6100 0    60   ~ 0
SCLK
Wire Wire Line
	1350 6000 1050 6000
Wire Wire Line
	1350 6100 1050 6100
$Comp
L power:GND #PWR031
U 1 1 5AA028F9
P 2850 2500
F 0 "#PWR031" H 2850 2250 50  0001 C CNN
F 1 "GND" H 2850 2350 50  0000 C CNN
F 2 "" H 2850 2500 50  0001 C CNN
F 3 "" H 2850 2500 50  0001 C CNN
	1    2850 2500
	0    1    1    0   
$EndComp
$Comp
L Connector:Conn_01x02_Female SW1
U 1 1 5AA04426
P 7650 3150
F 0 "SW1" H 7650 3250 50  0000 C CNN
F 1 "MX socket" H 7650 2950 50  0000 C CNN
F 2 "Pocket Voltex:SW_Cherry_MX1A_1.00u_PCB_Socket" H 7650 3150 50  0001 C CNN
F 3 "" H 7650 3150 50  0001 C CNN
F 4 "PG151101S11" H 7650 3150 50  0001 C CNN "Part number"
F 5 "https://www.aliexpress.com/item/32875065054.html" H 7650 3150 50  0001 C CNN "Link"
	1    7650 3150
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female SW2
U 1 1 5AA048F5
P 7650 3550
F 0 "SW2" H 7650 3650 50  0000 C CNN
F 1 "MX socket" H 7650 3350 50  0000 C CNN
F 2 "Pocket Voltex:SW_Cherry_MX1A_1.25u_PCB_Socket" H 7650 3550 50  0001 C CNN
F 3 "" H 7650 3550 50  0001 C CNN
F 4 "PG151101S11" H 7650 3550 50  0001 C CNN "Part number"
F 5 "https://www.aliexpress.com/item/32875065054.html" H 7650 3550 50  0001 C CNN "Link"
	1    7650 3550
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female SW3
U 1 1 5AA04B53
P 7650 3950
F 0 "SW3" H 7650 4050 50  0000 C CNN
F 1 "MX socket" H 7650 3750 50  0000 C CNN
F 2 "Pocket Voltex:SW_Cherry_MX1A_1.25u_PCB_Socket" H 7650 3950 50  0001 C CNN
F 3 "" H 7650 3950 50  0001 C CNN
F 4 "PG151101S11" H 7650 3950 50  0001 C CNN "Part number"
F 5 "https://www.aliexpress.com/item/32875065054.html" H 7650 3950 50  0001 C CNN "Link"
	1    7650 3950
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female SW4
U 1 1 5AA05186
P 7650 4350
F 0 "SW4" H 7650 4450 50  0000 C CNN
F 1 "MX socket" H 7650 4150 50  0000 C CNN
F 2 "Pocket Voltex:SW_Cherry_MX1A_1.25u_PCB_Socket" H 7650 4350 50  0001 C CNN
F 3 "" H 7650 4350 50  0001 C CNN
F 4 "PG151101S11" H 7650 4350 50  0001 C CNN "Part number"
F 5 "https://www.aliexpress.com/item/32875065054.html" H 7650 4350 50  0001 C CNN "Link"
	1    7650 4350
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female SW5
U 1 1 5AA051F2
P 7650 4750
F 0 "SW5" H 7650 4850 50  0000 C CNN
F 1 "MX socket" H 7650 4550 50  0000 C CNN
F 2 "Pocket Voltex:SW_Cherry_MX1A_1.25u_PCB_Socket" H 7650 4750 50  0001 C CNN
F 3 "" H 7650 4750 50  0001 C CNN
F 4 "PG151101S11" H 7650 4750 50  0001 C CNN "Part number"
F 5 "https://www.aliexpress.com/item/32875065054.html" H 7650 4750 50  0001 C CNN "Link"
	1    7650 4750
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female SW6
U 1 1 5AA05280
P 7650 5150
F 0 "SW6" H 7650 5250 50  0000 C CNN
F 1 "MX socket" H 7650 4950 50  0000 C CNN
F 2 "Pocket Voltex:SW_Cherry_MX1A_2.25u_PCB_Socket" H 7650 5150 50  0001 C CNN
F 3 "" H 7650 5150 50  0001 C CNN
F 4 "PG151101S11" H 7650 5150 50  0001 C CNN "Part number"
F 5 "https://www.aliexpress.com/item/32875065054.html" H 7650 5150 50  0001 C CNN "Link"
	1    7650 5150
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Female SW7
U 1 1 5AA052F4
P 7650 5550
F 0 "SW7" H 7650 5650 50  0000 C CNN
F 1 "MX socket" H 7650 5350 50  0000 C CNN
F 2 "Pocket Voltex:SW_Cherry_MX1A_2.25u_PCB_Socket" H 7650 5550 50  0001 C CNN
F 3 "" H 7650 5550 50  0001 C CNN
F 4 "PG151101S11" H 7650 5550 50  0001 C CNN "Part number"
F 5 "https://www.aliexpress.com/item/32875065054.html" H 7650 5550 50  0001 C CNN "Link"
	1    7650 5550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR032
U 1 1 5AA062AF
P 7450 5650
F 0 "#PWR032" H 7450 5400 50  0001 C CNN
F 1 "GND" H 7450 5500 50  0000 C CNN
F 2 "" H 7450 5650 50  0001 C CNN
F 3 "" H 7450 5650 50  0001 C CNN
	1    7450 5650
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR033
U 1 1 5AA0631E
P 7450 5250
F 0 "#PWR033" H 7450 5000 50  0001 C CNN
F 1 "GND" H 7450 5100 50  0000 C CNN
F 2 "" H 7450 5250 50  0001 C CNN
F 3 "" H 7450 5250 50  0001 C CNN
	1    7450 5250
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR034
U 1 1 5AA06386
P 7450 4850
F 0 "#PWR034" H 7450 4600 50  0001 C CNN
F 1 "GND" H 7450 4700 50  0000 C CNN
F 2 "" H 7450 4850 50  0001 C CNN
F 3 "" H 7450 4850 50  0001 C CNN
	1    7450 4850
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR035
U 1 1 5AA063EE
P 7450 4450
F 0 "#PWR035" H 7450 4200 50  0001 C CNN
F 1 "GND" H 7450 4300 50  0000 C CNN
F 2 "" H 7450 4450 50  0001 C CNN
F 3 "" H 7450 4450 50  0001 C CNN
	1    7450 4450
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR036
U 1 1 5AA06BDE
P 7450 4050
F 0 "#PWR036" H 7450 3800 50  0001 C CNN
F 1 "GND" H 7450 3900 50  0000 C CNN
F 2 "" H 7450 4050 50  0001 C CNN
F 3 "" H 7450 4050 50  0001 C CNN
	1    7450 4050
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR037
U 1 1 5AA06C46
P 7450 3650
F 0 "#PWR037" H 7450 3400 50  0001 C CNN
F 1 "GND" H 7450 3500 50  0000 C CNN
F 2 "" H 7450 3650 50  0001 C CNN
F 3 "" H 7450 3650 50  0001 C CNN
	1    7450 3650
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR038
U 1 1 5AA06CAE
P 7450 3250
F 0 "#PWR038" H 7450 3000 50  0001 C CNN
F 1 "GND" H 7450 3100 50  0000 C CNN
F 2 "" H 7450 3250 50  0001 C CNN
F 3 "" H 7450 3250 50  0001 C CNN
	1    7450 3250
	0    1    1    0   
$EndComp
$Comp
L power:VCC #PWR039
U 1 1 5AA0CE5C
P 4400 1800
F 0 "#PWR039" H 4400 1650 50  0001 C CNN
F 1 "VCC" H 4400 1950 50  0000 C CNN
F 2 "" H 4400 1800 50  0001 C CNN
F 3 "" H 4400 1800 50  0001 C CNN
	1    4400 1800
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG040
U 1 1 5AA0E1C6
P 1450 1300
F 0 "#FLG040" H 1450 1375 50  0001 C CNN
F 1 "PWR_FLAG" H 1450 1450 50  0000 C CNN
F 2 "" H 1450 1300 50  0001 C CNN
F 3 "" H 1450 1300 50  0001 C CNN
	1    1450 1300
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG041
U 1 1 5AA0E218
P 1850 1300
F 0 "#FLG041" H 1850 1375 50  0001 C CNN
F 1 "PWR_FLAG" H 1850 1450 50  0000 C CNN
F 2 "" H 1850 1300 50  0001 C CNN
F 3 "" H 1850 1300 50  0001 C CNN
	1    1850 1300
	1    0    0    -1  
$EndComp
$Comp
L power:VCC #PWR042
U 1 1 5AA0E26A
P 1850 1300
F 0 "#PWR042" H 1850 1150 50  0001 C CNN
F 1 "VCC" H 1850 1450 50  0000 C CNN
F 2 "" H 1850 1300 50  0001 C CNN
F 3 "" H 1850 1300 50  0001 C CNN
	1    1850 1300
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR043
U 1 1 5AA0E2BC
P 1450 1300
F 0 "#PWR043" H 1450 1050 50  0001 C CNN
F 1 "GND" H 1450 1150 50  0000 C CNN
F 2 "" H 1450 1300 50  0001 C CNN
F 3 "" H 1450 1300 50  0001 C CNN
	1    1450 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 1150 2550 1150
Wire Wire Line
	2050 3600 2050 3650
Wire Wire Line
	2050 3400 2050 3450
Wire Wire Line
	1050 4350 1050 4450
Wire Wire Line
	1050 4450 1200 4450
Wire Wire Line
	1350 4450 1350 4350
Connection ~ 1200 4450
Wire Wire Line
	1200 4450 1350 4450
Wire Wire Line
	2100 3150 1950 3150
Wire Wire Line
	1950 3050 2100 3050
Wire Wire Line
	2100 3050 2100 3150
Wire Wire Line
	2400 3150 2300 3150
$Comp
L power:GND #PWR07
U 1 1 5A9E5C39
P 4500 4700
F 0 "#PWR07" H 4500 4450 50  0001 C CNN
F 1 "GND" H 4500 4550 50  0000 C CNN
F 2 "" H 4500 4700 50  0001 C CNN
F 3 "" H 4500 4700 50  0001 C CNN
	1    4500 4700
	0    -1   -1   0   
$EndComp
$Comp
L Device:Resonator X1
U 1 1 5A9D88C0
P 3100 2500
F 0 "X1" V 3100 2650 50  0000 C CNN
F 1 "16MHz" V 3000 2700 50  0000 C CNN
F 2 "Pocket Voltex:murata-resonator_SMD_CSTCE16M0V53-R0" H 3075 2500 50  0001 C CNN
F 3 "" H 3075 2500 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/murata-electronics/CSTCE16M0V53-R0/490-1198-1-ND/584635" H 3100 2500 50  0001 C CNN "Link"
F 5 "CSTCE16M0V53-R0" H 3100 2500 50  0001 C CNN "Part number"
	1    3100 2500
	0    1    1    0   
$EndComp
Wire Wire Line
	3100 2350 3700 2350
Wire Wire Line
	3700 2350 3700 2400
Wire Wire Line
	3100 2650 3700 2650
Wire Wire Line
	3700 2650 3700 2600
Wire Wire Line
	2850 2500 2900 2500
Wire Wire Line
	3050 2900 2800 2900
Wire Wire Line
	3050 2800 2800 2800
Wire Wire Line
	3350 2900 3700 2900
Wire Wire Line
	3700 2800 3350 2800
Text Label 2800 2800 0    60   ~ 0
D+
Text Label 2800 2900 0    60   ~ 0
D-
$Comp
L Device:R R1
U 1 1 5A9D79F6
P 3200 2800
F 0 "R1" V 3280 2800 50  0000 C CNN
F 1 "22R" V 3200 2800 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3130 2800 50  0001 C CNN
F 3 "" H 3200 2800 50  0001 C CNN
F 4 "RC0603JR-0722RL" H 3200 2800 50  0001 C CNN "Part number"
	1    3200 2800
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R2
U 1 1 5A9D795E
P 3200 2900
F 0 "R2" V 3280 2900 50  0000 C CNN
F 1 "22R" V 3200 2900 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 3130 2900 50  0001 C CNN
F 3 "" H 3200 2900 50  0001 C CNN
F 4 "RC0603JR-0722RL" H 3200 2900 50  0001 C CNN "Part number"
	1    3200 2900
	0    1    1    0   
$EndComp
Wire Wire Line
	4300 1900 4300 1800
Wire Wire Line
	4300 1800 4400 1800
Wire Wire Line
	4400 1900 4400 1800
Connection ~ 4400 1800
Wire Wire Line
	4500 1900 4500 1800
Wire Wire Line
	4500 1800 4400 1800
Wire Wire Line
	3700 3100 3000 3100
$Comp
L power:GND #PWR04
U 1 1 5A9E38E3
P 3000 3400
F 0 "#PWR04" H 3000 3150 50  0001 C CNN
F 1 "GND" H 3000 3250 50  0000 C CNN
F 2 "" H 3000 3400 50  0001 C CNN
F 3 "" H 3000 3400 50  0001 C CNN
	1    3000 3400
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5A9D641D
P 3000 3250
F 0 "C1" H 3025 3350 50  0000 L CNN
F 1 "1uF" H 3025 3150 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 3038 3100 50  0001 C CNN
F 3 "" H 3000 3250 50  0001 C CNN
F 4 "CC0603KRX5R5BB105" H 3000 3250 50  0001 C CNN "Part number"
	1    3000 3250
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 5D2A9442
P 2400 3150
F 0 "#PWR0101" H 2400 2900 50  0001 C CNN
F 1 "GND" H 2400 3000 50  0000 C CNN
F 2 "" H 2400 3150 50  0001 C CNN
F 3 "" H 2400 3150 50  0001 C CNN
	1    2400 3150
	0    -1   -1   0   
$EndComp
Text Label 5650 2600 2    60   ~ 0
SW_MACRO
Text Label 5650 2300 2    60   ~ 0
SCLK
Text Label 5650 2400 2    60   ~ 0
MOSI
Text Label 5650 2500 2    60   ~ 0
MISO
Text Label 5650 2700 2    60   ~ 0
SW_FXL
Text Label 5650 2800 2    60   ~ 0
SW_A
NoConn ~ 5100 2200
NoConn ~ 5100 2900
Text Label 5650 3100 2    60   ~ 0
SW_START
NoConn ~ 5100 3200
NoConn ~ 5100 3300
NoConn ~ 5100 3400
NoConn ~ 5100 3500
Text Label 5650 3700 2    60   ~ 0
ENC1B
Text Label 5650 3800 2    60   ~ 0
ENC1A
Text Label 5650 3900 2    60   ~ 0
ENC2B
Text Label 5650 4000 2    60   ~ 0
ENC2A
Text Label 5650 4100 2    60   ~ 0
SW_D
Text Label 5650 4200 2    60   ~ 0
SW_C
Text Label 5650 4300 2    60   ~ 0
SW_FXR
Text Label 5650 4400 2    60   ~ 0
SW_B
Wire Wire Line
	5650 2600 5100 2600
Wire Wire Line
	5650 2500 5100 2500
Wire Wire Line
	5650 2400 5100 2400
Wire Wire Line
	5650 2300 5100 2300
Wire Wire Line
	5650 2700 5100 2700
Wire Wire Line
	5650 2800 5100 2800
Wire Wire Line
	5650 3100 5100 3100
Wire Wire Line
	5650 3700 5100 3700
Wire Wire Line
	5650 3800 5100 3800
Wire Wire Line
	5650 3900 5100 3900
Wire Wire Line
	5650 4000 5100 4000
Wire Wire Line
	5650 4100 5100 4100
Wire Wire Line
	5650 4200 5100 4200
Wire Wire Line
	5650 4300 5100 4300
Wire Wire Line
	5650 4400 5100 4400
Connection ~ 2100 3150
$Comp
L Device:R_Small R4
U 1 1 5D2425E8
P 2200 3150
F 0 "R4" V 2004 3150 50  0000 C CNN
F 1 "5.1k" V 2095 3150 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 2200 3150 50  0001 C CNN
F 3 "~" H 2200 3150 50  0001 C CNN
F 4 "RC0603JR-075K1L" H 2200 3150 50  0001 C CNN "Part number"
	1    2200 3150
	0    1    1    0   
$EndComp
Connection ~ 3700 2200
Wire Wire Line
	4300 4700 4400 4700
Connection ~ 4400 4700
Wire Wire Line
	4400 4700 4500 4700
$Comp
L Mechanical:MountingHole H1
U 1 1 5D2D8B25
P 9550 1050
F 0 "H1" H 9650 1096 50  0000 L CNN
F 1 "MountingHole" H 9650 1005 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 9550 1050 50  0001 C CNN
F 3 "~" H 9550 1050 50  0001 C CNN
	1    9550 1050
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H2
U 1 1 5D2D9A3B
P 9550 1250
F 0 "H2" H 9650 1296 50  0000 L CNN
F 1 "MountingHole" H 9650 1205 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 9550 1250 50  0001 C CNN
F 3 "~" H 9550 1250 50  0001 C CNN
	1    9550 1250
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H3
U 1 1 5D2DA64A
P 9550 1450
F 0 "H3" H 9650 1496 50  0000 L CNN
F 1 "MountingHole" H 9650 1405 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 9550 1450 50  0001 C CNN
F 3 "~" H 9550 1450 50  0001 C CNN
	1    9550 1450
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H4
U 1 1 5D2DA650
P 9550 1650
F 0 "H4" H 9650 1696 50  0000 L CNN
F 1 "MountingHole" H 9650 1605 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 9550 1650 50  0001 C CNN
F 3 "~" H 9550 1650 50  0001 C CNN
	1    9550 1650
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H5
U 1 1 5D2DE57C
P 9550 1850
F 0 "H5" H 9650 1896 50  0000 L CNN
F 1 "MountingHole" H 9650 1805 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 9550 1850 50  0001 C CNN
F 3 "~" H 9550 1850 50  0001 C CNN
	1    9550 1850
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H6
U 1 1 5D2DE582
P 9550 2050
F 0 "H6" H 9650 2096 50  0000 L CNN
F 1 "MountingHole" H 9650 2005 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 9550 2050 50  0001 C CNN
F 3 "~" H 9550 2050 50  0001 C CNN
	1    9550 2050
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H7
U 1 1 5D2E2506
P 9550 2250
F 0 "H7" H 9650 2296 50  0000 L CNN
F 1 "MountingHole" H 9650 2205 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 9550 2250 50  0001 C CNN
F 3 "~" H 9550 2250 50  0001 C CNN
	1    9550 2250
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H8
U 1 1 5D2E250C
P 9550 2450
F 0 "H8" H 9650 2496 50  0000 L CNN
F 1 "MountingHole" H 9650 2405 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3" H 9550 2450 50  0001 C CNN
F 3 "~" H 9550 2450 50  0001 C CNN
	1    9550 2450
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:Fiducial LOGO1
U 1 1 5D2EBA8F
P 9550 2850
F 0 "LOGO1" H 9635 2896 50  0000 L CNN
F 1 "Rasis" H 9635 2805 50  0000 L CNN
F 2 "Pocket Voltex:Rasis" H 9550 2850 50  0001 C CNN
F 3 "~" H 9550 2850 50  0001 C CNN
	1    9550 2850
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole EH2
U 1 1 5E9786EF
P 8550 1400
F 0 "EH2" H 8650 1446 50  0000 L CNN
F 1 "Encoder nut" H 8650 1355 50  0000 L CNN
F 2 "Pocket Voltex:SMD_Standoff_M2_Wurth" H 8550 1400 50  0001 C CNN
F 3 "https://katalog.we-online.de/em/datasheet/9774020243R.pdf" H 8550 1400 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/w-rth-elektronik/9774020243R/732-7073-1-ND/5320678" H 8550 1400 50  0001 C CNN "Link"
F 5 "9774020243R" H 8550 1400 50  0001 C CNN "Part number"
	1    8550 1400
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole EH1
U 1 1 5E978C67
P 8550 1200
F 0 "EH1" H 8650 1246 50  0000 L CNN
F 1 "Encoder nut" H 8650 1155 50  0000 L CNN
F 2 "Pocket Voltex:SMD_Standoff_M2_Wurth" H 8550 1200 50  0001 C CNN
F 3 "https://katalog.we-online.de/em/datasheet/9774020243R.pdf" H 8550 1200 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/w-rth-elektronik/9774020243R/732-7073-1-ND/5320678" H 8550 1200 50  0001 C CNN "Link"
F 5 "9774020243R" H 8550 1200 50  0001 C CNN "Part number"
	1    8550 1200
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole EH4
U 1 1 5E979C9D
P 8550 2600
F 0 "EH4" H 8650 2646 50  0000 L CNN
F 1 "Encoder nut" H 8650 2555 50  0000 L CNN
F 2 "Pocket Voltex:SMD_Standoff_M2_Wurth" H 8550 2600 50  0001 C CNN
F 3 "https://katalog.we-online.de/em/datasheet/9774020243R.pdf" H 8550 2600 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/w-rth-elektronik/9774020243R/732-7073-1-ND/5320678" H 8550 2600 50  0001 C CNN "Link"
F 5 "9774020243R" H 8550 2600 50  0001 C CNN "Part number"
	1    8550 2600
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole EH3
U 1 1 5E979CA3
P 8550 2400
F 0 "EH3" H 8650 2446 50  0000 L CNN
F 1 "Encoder nut" H 8650 2355 50  0000 L CNN
F 2 "Pocket Voltex:SMD_Standoff_M2_Wurth" H 8550 2400 50  0001 C CNN
F 3 "https://katalog.we-online.de/em/datasheet/9774020243R.pdf" H 8550 2400 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/w-rth-elektronik/9774020243R/732-7073-1-ND/5320678" H 8550 2400 50  0001 C CNN "Link"
F 5 "9774020243R" H 8550 2400 50  0001 C CNN "Part number"
	1    8550 2400
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 EJ1
U 1 1 5E984BF0
P 7600 800
F 0 "EJ1" V 7700 850 50  0000 R CNN
F 1 "Hotswap" V 7600 1200 50  0000 R CNN
F 2 "Pocket Voltex:Harwin-SMD-Socket-S9091-46R" H 7600 800 50  0001 C CNN
F 3 "" H 7600 800 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/harwin-inc/S9091-46R/952-2639-1-ND/5175732" H 7600 800 50  0001 C CNN "Link"
F 5 "S9091-46R" H 7600 800 50  0001 C CNN "Part number"
	1    7600 800 
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 EJ2
U 1 1 5E98652C
P 7750 800
F 0 "EJ2" V 7850 850 50  0000 R CNN
F 1 "Hotswap" V 7750 1200 50  0001 R CNN
F 2 "Pocket Voltex:Harwin-SMD-Socket-S9091-46R" H 7750 800 50  0001 C CNN
F 3 "" H 7750 800 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/harwin-inc/S9091-46R/952-2639-1-ND/5175732" H 7750 800 50  0001 C CNN "Link"
F 5 "S9091-46R" H 7750 800 50  0001 C CNN "Part number"
	1    7750 800 
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 EJ3
U 1 1 5E986959
P 7900 800
F 0 "EJ3" V 8000 850 50  0000 R CNN
F 1 "Hotswap" V 7900 1200 50  0001 R CNN
F 2 "Pocket Voltex:Harwin-SMD-Socket-S9091-46R" H 7900 800 50  0001 C CNN
F 3 "" H 7900 800 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/harwin-inc/S9091-46R/952-2639-1-ND/5175732" H 7900 800 50  0001 C CNN "Link"
F 5 "S9091-46R" H 7900 800 50  0001 C CNN "Part number"
	1    7900 800 
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7600 1000 7600 1200
Connection ~ 7600 1200
Wire Wire Line
	7600 1200 7500 1200
Wire Wire Line
	7600 1200 7950 1200
Wire Wire Line
	7500 2400 7600 2400
$Comp
L Connector_Generic:Conn_01x01 EJ4
U 1 1 5E9CC55B
P 7600 2000
F 0 "EJ4" V 7700 2050 50  0000 R CNN
F 1 "Hotswap" V 7600 2400 50  0000 R CNN
F 2 "Pocket Voltex:Harwin-SMD-Socket-S9091-46R" H 7600 2000 50  0001 C CNN
F 3 "" H 7600 2000 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/harwin-inc/S9091-46R/952-2639-1-ND/5175732" H 7600 2000 50  0001 C CNN "Link"
F 5 "S9091-46R" H 7600 2000 50  0001 C CNN "Part number"
	1    7600 2000
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 EJ5
U 1 1 5E9CC561
P 7750 2000
F 0 "EJ5" V 7850 2050 50  0000 R CNN
F 1 "Hotswap" V 7750 2400 50  0001 R CNN
F 2 "Pocket Voltex:Harwin-SMD-Socket-S9091-46R" H 7750 2000 50  0001 C CNN
F 3 "" H 7750 2000 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/harwin-inc/S9091-46R/952-2639-1-ND/5175732" H 7750 2000 50  0001 C CNN "Link"
F 5 "S9091-46R" H 7750 2000 50  0001 C CNN "Part number"
	1    7750 2000
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 EJ6
U 1 1 5E9CC567
P 7900 2000
F 0 "EJ6" V 8000 2050 50  0000 R CNN
F 1 "Hotswap" V 7900 2400 50  0001 R CNN
F 2 "Pocket Voltex:Harwin-SMD-Socket-S9091-46R" H 7900 2000 50  0001 C CNN
F 3 "" H 7900 2000 50  0001 C CNN
F 4 "https://www.digikey.com/product-detail/en/harwin-inc/S9091-46R/952-2639-1-ND/5175732" H 7900 2000 50  0001 C CNN "Link"
F 5 "S9091-46R" H 7900 2000 50  0001 C CNN "Part number"
	1    7900 2000
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7600 2200 7600 2400
Connection ~ 7600 2400
Wire Wire Line
	7600 2400 7950 2400
Wire Wire Line
	7150 2500 7900 2500
Wire Wire Line
	7500 2600 7750 2600
Wire Wire Line
	7750 2200 7750 2600
Connection ~ 7750 2600
Wire Wire Line
	7750 2600 7950 2600
Wire Wire Line
	7900 2200 7900 2500
Connection ~ 7900 2500
Wire Wire Line
	7900 2500 7950 2500
Wire Wire Line
	7150 1300 7900 1300
Wire Wire Line
	7500 1400 7750 1400
Wire Wire Line
	7750 1000 7750 1400
Connection ~ 7750 1400
Wire Wire Line
	7750 1400 7950 1400
Wire Wire Line
	7900 1000 7900 1300
Connection ~ 7900 1300
Wire Wire Line
	7900 1300 7950 1300
$Comp
L MCU_Microchip_ATmega:ATmega32U2-AU U1
U 1 1 5E9879FF
P 4400 3300
F 0 "U1" H 4400 1811 50  0000 C CNN
F 1 "ATmega32U2-AU" H 4400 1720 50  0000 C CNN
F 2 "Package_QFP:TQFP-32_7x7mm_P0.8mm" H 4400 3300 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/doc7799.pdf" H 4400 3300 50  0001 C CNN
	1    4400 3300
	1    0    0    -1  
$EndComp
$EndSCHEMATC
