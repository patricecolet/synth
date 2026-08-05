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
LIBS:PS-101-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
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
L AS3340 U1
U 1 1 5DB0AD43
P 6050 2950
F 0 "U1" H 5850 3650 50  0000 C CNN
F 1 "AS3340" H 6250 3650 50  0000 C CNN
F 2 "Housings_DIP:DIP-16_W7.62mm_Socket" H 6550 2650 50  0001 C CNN
F 3 "" H 6650 2500 50  0001 C CNN
	1    6050 2950
	1    0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 5DB0AD48
P 3600 3600
F 0 "R5" V 3680 3600 50  0000 C CNN
F 1 "1K62" V 3600 3600 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 3530 3600 50  0001 C CNN
F 3 "" H 3600 3600 50  0001 C CNN
	1    3600 3600
	-1   0    0    1   
$EndComp
$Comp
L POT P3
U 1 1 5DB0AD49
P 3600 4050
F 0 "P3" V 3425 4050 50  0000 C CNN
F 1 "500ohm" V 3500 4050 50  0001 C CNN
F 2 "Potentiometers:Potentiometer_Trimmer_Bourns_3005" H 3600 4050 50  0001 C CNN
F 3 "" H 3600 4050 50  0001 C CNN
F 4 "500" V 3500 4050 50  0000 C CNN "view"
	1    3600 4050
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 5DB0AD4A
P 4400 3800
F 0 "C2" H 4425 3900 50  0000 L CNN
F 1 "560p" H 4425 3700 50  0000 L CNN
F 2 "Capacitors_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 4438 3650 50  0001 C CNN
F 3 "" H 4400 3800 50  0001 C CNN
	1    4400 3800
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 5DB0AD4B
P 2750 3450
F 0 "R3" V 2830 3450 50  0000 C CNN
F 1 "22K" V 2750 3450 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 2680 3450 50  0001 C CNN
F 3 "" H 2750 3450 50  0001 C CNN
	1    2750 3450
	0    1    1    0   
$EndComp
$Comp
L POT P2
U 1 1 5DB0AD4C
P 2300 3450
F 0 "P2" V 2125 3450 50  0000 C CNN
F 1 "10K" V 2200 3450 50  0000 C CNN
F 2 "Potentiometers:Potentiometer_Trimmer_Bourns_3005" H 2300 3450 50  0001 C CNN
F 3 "" H 2300 3450 50  0001 C CNN
	1    2300 3450
	0    -1   -1   0   
$EndComp
$Comp
L C C4
U 1 1 5DB0AD4D
P 5150 3800
F 0 "C4" H 5175 3900 50  0000 L CNN
F 1 "1nF" H 5175 3700 50  0000 L CNN
F 2 "Capacitors_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 5188 3650 50  0001 C CNN
F 3 "" H 5150 3800 50  0001 C CNN
	1    5150 3800
	1    0    0    -1  
$EndComp
$Comp
L R R9
U 1 1 5DB0AD4F
P 4500 3050
F 0 "R9" V 4580 3050 50  0000 C CNN
F 1 "1M8" V 4500 3050 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 4430 3050 50  0001 C CNN
F 3 "" H 4500 3050 50  0001 C CNN
	1    4500 3050
	0    1    1    0   
$EndComp
$Comp
L R R10
U 1 1 5DB0AD50
P 4800 3250
F 0 "R10" V 4880 3250 50  0000 C CNN
F 1 "470ohm" V 4800 3250 50  0001 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 4730 3250 50  0001 C CNN
F 3 "" H 4800 3250 50  0001 C CNN
F 4 "470" V 4800 3250 60  0000 C CNN "view"
	1    4800 3250
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 5DB0AD51
P 4800 3800
F 0 "C3" H 4825 3900 50  0000 L CNN
F 1 "10nF" H 4825 3700 50  0000 L CNN
F 2 "Capacitors_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 4838 3650 50  0001 C CNN
F 3 "" H 4800 3800 50  0001 C CNN
	1    4800 3800
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 5DB0AD52
P 2550 2900
F 0 "R1" V 2630 2900 50  0000 C CNN
F 1 "220K" V 2550 2900 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 2480 2900 50  0001 C CNN
F 3 "" H 2550 2900 50  0001 C CNN
	1    2550 2900
	0    1    1    0   
$EndComp
$Comp
L POT P1
U 1 1 5DB0AD53
P 2150 2900
F 0 "P1" V 1975 2900 50  0000 C CNN
F 1 "100K" V 2050 2900 50  0000 C CNN
F 2 "Potentiometers:Potentiometer_Trimmer_Bourns_3005" H 2150 2900 50  0001 C CNN
F 3 "" H 2150 2900 50  0001 C CNN
	1    2150 2900
	0    -1   -1   0   
$EndComp
$Comp
L C C1
U 1 1 5DB0AD54
P 4000 3800
F 0 "C1" H 4025 3900 50  0000 L CNN
F 1 "10nF" H 4025 3700 50  0000 L CNN
F 2 "Capacitors_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm" H 4038 3650 50  0001 C CNN
F 3 "" H 4000 3800 50  0001 C CNN
	1    4000 3800
	1    0    0    -1  
$EndComp
$Comp
L R R8
U 1 1 5DB0AD55
P 4000 3250
F 0 "R8" V 4080 3250 50  0000 C CNN
F 1 "470Ohm" V 4000 3250 50  0001 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 3930 3250 50  0001 C CNN
F 3 "" H 4000 3250 50  0001 C CNN
F 4 "470" V 4000 3250 60  0000 C CNN "view"
	1    4000 3250
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 5DB0AD56
P 2750 2650
F 0 "R2" V 2830 2650 50  0000 C CNN
F 1 "100K" V 2750 2650 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 2680 2650 50  0001 C CNN
F 3 "" H 2750 2650 50  0001 C CNN
	1    2750 2650
	1    0    0    -1  
$EndComp
$Comp
L R R7
U 1 1 5DB0AD58
P 3750 2550
F 0 "R7" V 3830 2550 50  0000 C CNN
F 1 "5K6" V 3750 2550 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 3680 2550 50  0001 C CNN
F 3 "" H 3750 2550 50  0001 C CNN
	1    3750 2550
	0    1    1    0   
$EndComp
$Comp
L R R11
U 1 1 5DB0AD59
P 5000 2250
F 0 "R11" V 5080 2250 50  0000 C CNN
F 1 "18K" V 5000 2250 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 4930 2250 50  0001 C CNN
F 3 "" H 5000 2250 50  0001 C CNN
	1    5000 2250
	0    1    1    0   
$EndComp
$Comp
L POT P4
U 1 1 5DB0AD5A
P 4500 2250
F 0 "P4" V 4325 2250 50  0000 C CNN
F 1 "5K" V 4400 2250 50  0000 C CNN
F 2 "Potentiometers:Potentiometer_Trimmer_Bourns_3005" H 4500 2250 50  0001 C CNN
F 3 "" H 4500 2250 50  0001 C CNN
	1    4500 2250
	0    -1   -1   0   
$EndComp
$Comp
L R R6
U 1 1 5DB0AD5C
P 3750 2250
F 0 "R6" V 3830 2250 50  0000 C CNN
F 1 "5K6" V 3750 2250 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 3680 2250 50  0001 C CNN
F 3 "" H 3750 2250 50  0001 C CNN
	1    3750 2250
	0    1    1    0   
$EndComp
$Comp
L TEST_1P J3
U 1 1 5DB0AD5D
P 4050 2000
F 0 "J3" H 4050 2270 50  0000 C CNN
F 1 "T1" H 4050 2200 50  0000 C CNN
F 2 "Measurement_Points:Test_Point_Keystone_5000-5004_Miniature" H 4250 2000 50  0001 C CNN
F 3 "" H 4250 2000 50  0001 C CNN
	1    4050 2000
	1    0    0    -1  
$EndComp
$Comp
L TEST_1P J4
U 1 1 5DB0AD5E
P 4150 2000
F 0 "J4" H 4150 2270 50  0000 C CNN
F 1 "T2" H 4150 2200 50  0000 C CNN
F 2 "Measurement_Points:Test_Point_Keystone_5000-5004_Miniature" H 4350 2000 50  0001 C CNN
F 3 "" H 4350 2000 50  0001 C CNN
	1    4150 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 3450 5550 3650
Wire Wire Line
	5550 4050 5550 3750
Connection ~ 6150 4050
Wire Wire Line
	2900 3450 5550 3450
Connection ~ 5550 4050
Wire Wire Line
	4400 3450 4400 3650
Connection ~ 4400 3450
Wire Wire Line
	4400 4050 4400 3950
Connection ~ 4400 4050
Wire Wire Line
	2450 3450 2600 3450
Wire Wire Line
	5150 3650 5150 3350
Wire Wire Line
	5150 3350 5550 3350
Wire Wire Line
	5150 4050 5150 3950
Connection ~ 5150 4050
Wire Wire Line
	1800 3450 2150 3450
Wire Wire Line
	4650 3050 5550 3050
Wire Wire Line
	4800 3050 4800 3100
Connection ~ 4800 3050
Wire Wire Line
	4800 3400 4800 3650
Wire Wire Line
	4800 4050 4800 3950
Connection ~ 4800 4050
Wire Wire Line
	4350 3050 2300 3050
Wire Wire Line
	2300 3050 2300 3300
Wire Wire Line
	5550 3150 5150 3150
Wire Wire Line
	5150 3150 5150 2900
Wire Wire Line
	5150 2900 2700 2900
Wire Wire Line
	2300 2900 2400 2900
Wire Wire Line
	2150 2750 2350 2750
Wire Wire Line
	2350 2750 2350 2900
Connection ~ 2350 2900
Wire Wire Line
	4000 3400 4000 3650
Wire Wire Line
	4000 3950 4000 4200
Connection ~ 4000 4050
Wire Wire Line
	2750 2800 2750 2900
Wire Wire Line
	5550 2450 5450 2450
Wire Wire Line
	5450 2450 5450 2250
Wire Wire Line
	5450 2250 5150 2250
Wire Wire Line
	3900 2550 5550 2550
Wire Wire Line
	4050 2550 4050 2000
Connection ~ 4050 2550
Wire Wire Line
	4650 2250 4850 2250
Wire Wire Line
	3600 2550 3200 2550
Wire Wire Line
	3900 2250 4350 2250
Wire Wire Line
	3050 2250 3600 2250
Wire Wire Line
	4150 2250 4150 2000
Connection ~ 4150 2250
Wire Wire Line
	4500 2100 4300 2100
Wire Wire Line
	4300 2100 4300 2250
Connection ~ 4300 2250
Connection ~ 5550 3450
Wire Wire Line
	4000 2900 4000 3100
Wire Wire Line
	2750 2500 2050 2500
Wire Wire Line
	5550 2650 5300 2650
Wire Wire Line
	5300 2650 5300 1250
Text HLabel 7350 2750 2    60   Output ~ 0
pwm_out
Text HLabel 7350 2950 2    60   Output ~ 0
sawtooth_out
Text HLabel 7350 3150 2    60   Output ~ 0
triangle_out
Text HLabel 5300 1250 0    60   Input ~ 0
pulsewidth
Text HLabel 2050 2500 0    60   Input ~ 0
cv_in
Wire Wire Line
	6050 1800 6050 2250
Wire Wire Line
	1800 2900 2000 2900
Wire Wire Line
	1800 3450 1800 2900
Connection ~ 1800 2900
Connection ~ 4000 2900
Connection ~ 2750 2900
$Comp
L R R4
U 1 1 5DB12A4C
P 2900 2250
F 0 "R4" V 2980 2250 50  0000 C CNN
F 1 "1K" V 2900 2250 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 2830 2250 50  0001 C CNN
F 3 "" H 2900 2250 50  0001 C CNN
	1    2900 2250
	0    1    1    0   
$EndComp
$Comp
L VCC #PWR06
U 1 1 5DB15BBD
P 6050 1800
F 0 "#PWR06" H 6050 1650 50  0001 C CNN
F 1 "VCC" H 6050 1950 50  0000 C CNN
F 2 "" H 6050 1800 50  0001 C CNN
F 3 "" H 6050 1800 50  0001 C CNN
	1    6050 1800
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR07
U 1 1 5DB15C99
P 1800 2900
F 0 "#PWR07" H 1800 2750 50  0001 C CNN
F 1 "VCC" H 1800 3050 50  0000 C CNN
F 2 "" H 1800 2900 50  0001 C CNN
F 3 "" H 1800 2900 50  0001 C CNN
	1    1800 2900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 5DB15DAA
P 6150 4150
F 0 "#PWR08" H 6150 3900 50  0001 C CNN
F 1 "GND" H 6150 4000 50  0000 C CNN
F 2 "" H 6150 4150 50  0001 C CNN
F 3 "" H 6150 4150 50  0001 C CNN
	1    6150 4150
	1    0    0    -1  
$EndComp
NoConn ~ 5550 2750
NoConn ~ 5550 2850
NoConn ~ 5550 2950
Wire Wire Line
	6150 3650 6150 4150
$Comp
L PWR_FLAG #FLG09
U 1 1 5DB1BF7D
P 6150 4050
F 0 "#FLG09" H 6150 4125 50  0001 C CNN
F 1 "PWR_FLAG" H 6150 4200 50  0000 C CNN
F 2 "" H 6150 4050 50  0001 C CNN
F 3 "" H 6150 4050 50  0001 C CNN
	1    6150 4050
	0    1    1    0   
$EndComp
Wire Wire Line
	3750 4050 6150 4050
Wire Wire Line
	4000 4200 3600 4200
Wire Wire Line
	3600 3900 3600 3750
Wire Wire Line
	6550 2750 7350 2750
Wire Wire Line
	6550 2950 7350 2950
Wire Wire Line
	6550 3150 7350 3150
Wire Wire Line
	3200 2250 3200 4550
Wire Wire Line
	3200 4550 6050 4550
Connection ~ 3200 2250
Wire Wire Line
	6050 4550 6050 3650
Connection ~ 3200 2550
Wire Wire Line
	2100 2250 2750 2250
Wire Wire Line
	2100 1950 2100 2250
$Comp
L VSS #PWR010
U 1 1 5DB58FE3
P 2100 1950
F 0 "#PWR010" H 2100 1800 50  0001 C CNN
F 1 "VSS" H 2100 2100 50  0000 C CNN
F 2 "" H 2100 1950 50  0001 C CNN
F 3 "" H 2100 1950 50  0001 C CNN
	1    2100 1950
	1    0    0    -1  
$EndComp
Connection ~ 3600 3450
$Comp
L PWR_FLAG #FLG011
U 1 1 5DB59FE3
P 6050 3650
F 0 "#FLG011" H 6050 3725 50  0001 C CNN
F 1 "PWR_FLAG" H 6050 3800 50  0000 C CNN
F 2 "" H 6050 3650 50  0001 C CNN
F 3 "" H 6050 3650 50  0001 C CNN
	1    6050 3650
	0    -1   -1   0   
$EndComp
$Comp
L Conn_01x02_Male J5
U 1 1 5DB5A5C4
P 5350 3650
F 0 "J5" H 5350 3750 50  0000 C CNN
F 1 "Conn_01x02_Male" H 5350 3450 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 5350 3650 50  0001 C CNN
F 3 "" H 5350 3650 50  0001 C CNN
	1    5350 3650
	1    0    0    -1  
$EndComp
$EndSCHEMATC
