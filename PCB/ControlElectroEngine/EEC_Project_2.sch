EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L Device:R R1
U 1 1 6000469C
P 4900 3400
F 0 "R1" V 5000 3500 50  0000 C CNN
F 1 "10 K" V 5000 3300 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 4830 3400 50  0001 C CNN
F 3 "~" H 4900 3400 50  0001 C CNN
	1    4900 3400
	-1   0    0    1   
$EndComp
$Comp
L Device:R R2
U 1 1 600081E0
P 5200 3400
F 0 "R2" V 5300 3325 50  0000 C CNN
F 1 "1 K" V 5300 3475 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 5130 3400 50  0001 C CNN
F 3 "~" H 5200 3400 50  0001 C CNN
	1    5200 3400
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J3
U 1 1 6000B24E
P 6925 5300
F 0 "J3" V 6925 5500 50  0000 C CNN
F 1 "Relay box" V 7100 5275 50  0000 C CNN
F 2 "Connector_JST:JST_XH_B4B-XH-A_1x04_P2.50mm_Vertical" H 6925 5300 50  0001 C CNN
F 3 "~" H 6925 5300 50  0001 C CNN
	1    6925 5300
	0    -1   1    0   
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J1
U 1 1 6000BAC1
P 6375 5300
F 0 "J1" V 6375 5125 50  0000 R CNN
F 1 "Current sensor" V 6550 5550 50  0000 R CNN
F 2 "Connector_JST:JST_XH_B3B-XH-A_1x03_P2.50mm_Vertical" H 6375 5300 50  0001 C CNN
F 3 "~" H 6375 5300 50  0001 C CNN
	1    6375 5300
	0    1    1    0   
$EndComp
$Comp
L Device:Speaker_Crystal LS1
U 1 1 6000D536
P 5200 2550
F 0 "LS1" V 5212 2270 50  0000 R CNN
F 1 "Speaker_Crystal" V 5121 2270 50  0000 R CNN
F 2 "Buzzer_Beeper:Buzzer_12x9.5RM7.6" H 5165 2500 50  0001 C CNN
F 3 "~" H 5165 2500 50  0001 C CNN
	1    5200 2550
	0    1    -1   0   
$EndComp
$Comp
L Device:R R3
U 1 1 6001FE02
P 3100 3300
F 0 "R3" V 3200 3400 50  0000 C CNN
F 1 "470" V 3200 3250 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 3030 3300 50  0001 C CNN
F 3 "~" H 3100 3300 50  0001 C CNN
	1    3100 3300
	-1   0    0    1   
$EndComp
$Comp
L Device:LED D1
U 1 1 60025FB9
P 3100 3700
F 0 "D1" H 3093 3917 50  0000 C CNN
F 1 "LED" H 3093 3826 50  0000 C CNN
F 2 "LED_THT:LED_D3.0mm_Clear" H 3100 3700 50  0001 C CNN
F 3 "~" H 3100 3700 50  0001 C CNN
	1    3100 3700
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_02x03_Odd_Even J2
U 1 1 60028A69
P 7575 3950
F 0 "J2" H 7425 4250 50  0000 C CNN
F 1 "ICSP" H 7625 4250 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x03_P2.54mm_Vertical" H 7575 3950 50  0001 C CNN
F 3 "~" H 7575 3950 50  0001 C CNN
	1    7575 3950
	1    0    0    -1  
$EndComp
$Comp
L MCU_Microchip_ATtiny:ATtiny13A-SSU U1
U 1 1 60003B81
P 3975 4050
F 0 "U1" H 3725 4650 50  0000 R CNN
F 1 "ATtiny13A-SSU" H 4575 4650 50  0000 R CNN
F 2 "Package_SO:SOIC-8_3.9x4.9mm_P1.27mm" H 3975 4050 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/doc8126.pdf" H 3975 4050 50  0001 C CNN
	1    3975 4050
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J4
U 1 1 60009436
P 2800 2950
F 0 "J4" H 2800 2750 50  0000 C CNN
F 1 "Power supply" H 2950 3125 50  0000 C CNN
F 2 "Connector_JST:JST_XH_B2B-XH-A_1x02_P2.50mm_Vertical" H 2800 2950 50  0001 C CNN
F 3 "~" H 2800 2950 50  0001 C CNN
	1    2800 2950
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J5
U 1 1 600CACA9
P 5775 5300
F 0 "J5" V 5775 5125 50  0000 R CNN
F 1 "Reserved" V 5950 5450 50  0000 R CNN
F 2 "Connector_JST:JST_XH_B3B-XH-A_1x03_P2.50mm_Vertical" H 5775 5300 50  0001 C CNN
F 3 "~" H 5775 5300 50  0001 C CNN
	1    5775 5300
	0    1    1    0   
$EndComp
$Comp
L Device:CP_Small C1
U 1 1 600CF095
P 3750 3150
F 0 "C1" H 3800 3250 50  0000 L CNN
F 1 "100mF" V 3875 2900 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D8.0mm_P3.50mm" H 3788 3000 50  0001 C CNN
F 3 "~" H 3750 3150 50  0001 C CNN
	1    3750 3150
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C2
U 1 1 600D150B
P 3425 3150
F 0 "C2" H 3475 3250 50  0000 L CNN
F 1 "0,1mF" H 3450 3050 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D7.0mm_W2.5mm_P5.00mm" H 3425 3150 50  0001 C CNN
F 3 "~" H 3425 3150 50  0001 C CNN
	1    3425 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	3975 4750 3975 4650
Wire Wire Line
	5200 2750 5200 3250
Wire Wire Line
	5100 2750 5100 2950
Wire Wire Line
	4900 3250 4900 2950
Wire Wire Line
	4900 3550 4900 4250
Connection ~ 5100 2950
Wire Wire Line
	3975 2950 3975 3450
Wire Wire Line
	3000 2950 3100 2950
Connection ~ 3975 2950
Wire Wire Line
	3000 3050 3250 3050
Wire Wire Line
	3250 4750 3975 4750
Connection ~ 3975 4750
Wire Wire Line
	6475 2950 6475 5100
Wire Wire Line
	6825 2950 6825 5100
Wire Wire Line
	6375 5100 6375 4750
Wire Wire Line
	7125 4750 7125 5100
Connection ~ 6375 4750
Connection ~ 6475 2950
Wire Wire Line
	6475 2950 6825 2950
Connection ~ 6825 2950
Connection ~ 7125 4750
Wire Wire Line
	5100 2950 5875 2950
Wire Wire Line
	3975 4750 5775 4750
Wire Wire Line
	6375 4750 7125 4750
Connection ~ 3750 2950
Wire Wire Line
	3750 2950 3975 2950
Wire Wire Line
	4575 3850 5200 3850
Wire Wire Line
	7375 4050 7250 4050
Wire Wire Line
	7250 4050 7250 4250
Wire Wire Line
	7875 3850 8075 3850
Wire Wire Line
	8075 3850 8075 2950
Wire Wire Line
	6825 2950 8075 2950
Wire Wire Line
	8075 4750 8075 4050
Wire Wire Line
	8075 4050 7875 4050
Wire Wire Line
	7125 4750 8075 4750
Wire Wire Line
	7875 3950 7950 3950
Wire Wire Line
	7950 3950 7950 3750
Wire Wire Line
	5200 3550 5200 3850
Connection ~ 5200 3850
Wire Wire Line
	5200 3850 7375 3850
Connection ~ 4900 2950
Wire Wire Line
	4900 2950 5100 2950
Connection ~ 4900 4250
Wire Wire Line
	4900 4250 7250 4250
Wire Wire Line
	4575 4250 4900 4250
Wire Wire Line
	3975 2950 4900 2950
Wire Wire Line
	4575 4050 6925 4050
Wire Wire Line
	5875 5100 5875 2950
Connection ~ 5875 2950
Wire Wire Line
	5875 2950 6475 2950
Wire Wire Line
	5775 5100 5775 4750
Connection ~ 5775 4750
Wire Wire Line
	5775 4750 6375 4750
Wire Wire Line
	3750 3250 3750 3350
Wire Wire Line
	3750 3350 3425 3350
Wire Wire Line
	3250 3050 3250 3350
Connection ~ 3250 3350
Wire Wire Line
	3250 3350 3250 4075
Wire Wire Line
	3750 3050 3750 2950
Wire Wire Line
	3425 3050 3425 2950
Connection ~ 3425 2950
Wire Wire Line
	3425 2950 3750 2950
Wire Wire Line
	3425 3250 3425 3350
Connection ~ 3425 3350
Wire Wire Line
	3425 3350 3250 3350
Wire Wire Line
	3100 3150 3100 2950
Connection ~ 3100 2950
Wire Wire Line
	3100 2950 3425 2950
Wire Wire Line
	3100 3450 3100 3550
Wire Wire Line
	3100 3850 3100 4075
Wire Wire Line
	3100 4075 3250 4075
Connection ~ 3250 4075
Wire Wire Line
	3250 4075 3250 4750
Wire Wire Line
	6925 4050 6925 5100
Connection ~ 6275 3950
Wire Wire Line
	6275 3950 7375 3950
Wire Wire Line
	4575 3950 6275 3950
Wire Wire Line
	6275 5100 6275 3950
Wire Wire Line
	7025 3750 7025 5100
Connection ~ 7025 3750
Wire Wire Line
	7025 3750 7950 3750
Wire Wire Line
	4575 3750 7025 3750
Wire Wire Line
	4575 4150 5675 4150
Wire Wire Line
	5675 4150 5675 5100
$EndSCHEMATC
