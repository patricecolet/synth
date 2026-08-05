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
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 3
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
L TL084 U1
U 2 1 5DB0802F
P 3200 1350
F 0 "U1" H 3200 1550 50  0000 L CNN
F 1 "TL084" H 3200 1150 50  0000 L CNN
F 2 "Housings_DIP:DIP-14_W7.62mm_Socket" H 3150 1450 50  0001 C CNN
F 3 "" H 3250 1550 50  0001 C CNN
	2    3200 1350
	1    0    0    -1  
$EndComp
$Comp
L LM13700 U2
U 1 1 5DB08031
P 4600 1250
F 0 "U2" H 4750 1450 50  0000 C CNN
F 1 "LM13700" H 4800 1050 50  0000 C CNN
F 2 "Housings_DIP:DIP-16_W7.62mm_Socket" H 4500 1275 50  0001 C CNN
F 3 "" H 4500 1275 50  0001 C CNN
	1    4600 1250
	1    0    0    -1  
$EndComp
$Comp
L LM13700 U2
U 3 1 5DB08033
P 4600 2700
F 0 "U2" H 4750 2900 50  0000 C CNN
F 1 "LM13700" H 4800 2500 50  0000 C CNN
F 2 "Housings_DIP:DIP-16_W7.62mm_Socket" H 4500 2725 50  0001 C CNN
F 3 "" H 4500 2725 50  0001 C CNN
	3    4600 2700
	1    0    0    -1  
$EndComp
$Comp
L LM13700 U3
U 1 1 5DB08035
P 4600 4050
F 0 "U3" H 4750 4250 50  0000 C CNN
F 1 "LM13700" H 4800 3850 50  0000 C CNN
F 2 "Housings_DIP:DIP-16_W7.62mm_Socket" H 4500 4075 50  0001 C CNN
F 3 "" H 4500 4075 50  0001 C CNN
	1    4600 4050
	1    0    0    -1  
$EndComp
$Comp
L TL084 U1
U 3 1 5DB08037
P 3200 2800
F 0 "U1" H 3200 3000 50  0000 L CNN
F 1 "TL084" H 3200 2600 50  0000 L CNN
F 2 "Housings_DIP:DIP-14_W7.62mm_Socket" H 3150 2900 50  0001 C CNN
F 3 "" H 3250 3000 50  0001 C CNN
	3    3200 2800
	1    0    0    -1  
$EndComp
$Comp
L TL084 U1
U 1 1 5DB08038
P 3150 4200
F 0 "U1" H 3150 4400 50  0000 L CNN
F 1 "TL084" H 3150 4000 50  0000 L CNN
F 2 "Housings_DIP:DIP-14_W7.62mm_Socket" H 3100 4300 50  0001 C CNN
F 3 "" H 3200 4400 50  0001 C CNN
	1    3150 4200
	1    0    0    -1  
$EndComp
$Comp
L TL084 U1
U 4 1 5DB08039
P 6600 1450
F 0 "U1" H 6600 1650 50  0000 L CNN
F 1 "TL084" H 6600 1250 50  0000 L CNN
F 2 "Housings_DIP:DIP-14_W7.62mm_Socket" H 6550 1550 50  0001 C CNN
F 3 "" H 6650 1650 50  0001 C CNN
	4    6600 1450
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR01
U 1 1 5DB08045
P 3900 3650
F 0 "#PWR01" H 3900 3500 50  0001 C CNN
F 1 "VCC" H 3900 3800 50  0000 C CNN
F 2 "" H 3900 3650 50  0001 C CNN
F 3 "" H 3900 3650 50  0001 C CNN
	1    3900 3650
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 5DB08046
P 3950 1350
F 0 "R4" V 4030 1350 50  0000 C CNN
F 1 "33K" V 3950 1350 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 3880 1350 50  0001 C CNN
F 3 "" H 3950 1350 50  0001 C CNN
	1    3950 1350
	0    1    1    0   
$EndComp
$Comp
L R R1
U 1 1 5DB08047
P 3900 1050
F 0 "R1" V 3980 1050 50  0000 C CNN
F 1 "47K" V 3900 1050 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 3830 1050 50  0001 C CNN
F 3 "" H 3900 1050 50  0001 C CNN
	1    3900 1050
	1    0    0    -1  
$EndComp
$Comp
L R R15
U 1 1 5DB0804B
P 5250 1550
F 0 "R15" V 5330 1550 50  0000 C CNN
F 1 "18K" V 5250 1550 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 5180 1550 50  0001 C CNN
F 3 "" H 5250 1550 50  0001 C CNN
	1    5250 1550
	1    0    0    -1  
$EndComp
$Comp
L R R6
U 1 1 5DB0804F
P 3950 2800
F 0 "R6" V 4030 2800 50  0000 C CNN
F 1 "15K" V 3950 2800 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 3880 2800 50  0001 C CNN
F 3 "" H 3950 2800 50  0001 C CNN
	1    3950 2800
	0    1    1    0   
$EndComp
$Comp
L R R5
U 1 1 5DB08050
P 3950 2550
F 0 "R5" V 4030 2550 50  0000 C CNN
F 1 "47K" V 3950 2550 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 3880 2550 50  0001 C CNN
F 3 "" H 3950 2550 50  0001 C CNN
	1    3950 2550
	1    0    0    -1  
$EndComp
$Comp
L R R13
U 1 1 5DB08052
P 5150 3000
F 0 "R13" V 5230 3000 50  0000 C CNN
F 1 "18K" V 5150 3000 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 5080 3000 50  0001 C CNN
F 3 "" H 5150 3000 50  0001 C CNN
	1    5150 3000
	1    0    0    -1  
$EndComp
$Comp
L R R18
U 1 1 5DB08054
P 5850 1600
F 0 "R18" V 5930 1600 50  0000 C CNN
F 1 "10K" V 5850 1600 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 5780 1600 50  0001 C CNN
F 3 "" H 5850 1600 50  0001 C CNN
	1    5850 1600
	1    0    0    -1  
$EndComp
$Comp
L R R21
U 1 1 5DB08055
P 6000 1250
F 0 "R21" V 6080 1250 50  0000 C CNN
F 1 "33K" V 6000 1250 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 5930 1250 50  0001 C CNN
F 3 "" H 6000 1250 50  0001 C CNN
	1    6000 1250
	0    1    1    0   
$EndComp
$Comp
L R R16
U 1 1 5DB08056
P 5750 2950
F 0 "R16" V 5830 2950 50  0000 C CNN
F 1 "10K" V 5750 2950 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 5680 2950 50  0001 C CNN
F 3 "" H 5750 2950 50  0001 C CNN
	1    5750 2950
	1    0    0    -1  
$EndComp
$Comp
L R R19
U 1 1 5DB08057
P 5900 2500
F 0 "R19" V 5980 2500 50  0000 C CNN
F 1 "22K" V 5900 2500 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 5830 2500 50  0001 C CNN
F 3 "" H 5900 2500 50  0001 C CNN
	1    5900 2500
	0    1    1    0   
$EndComp
$Comp
L R R3
U 1 1 5DB0805B
P 3900 4200
F 0 "R3" V 3980 4200 50  0000 C CNN
F 1 "10K" V 3900 4200 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 3830 4200 50  0001 C CNN
F 3 "" H 3900 4200 50  0001 C CNN
	1    3900 4200
	0    1    1    0   
$EndComp
$Comp
L R R2
U 1 1 5DB0805C
P 3900 3900
F 0 "R2" V 3980 3900 50  0000 C CNN
F 1 "47K" V 3900 3900 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 3830 3900 50  0001 C CNN
F 3 "" H 3900 3900 50  0001 C CNN
	1    3900 3900
	1    0    0    -1  
$EndComp
$Comp
L R R14
U 1 1 5DB0805E
P 5200 4350
F 0 "R14" V 5280 4350 50  0000 C CNN
F 1 "18K" V 5200 4350 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 5130 4350 50  0001 C CNN
F 3 "" H 5200 4350 50  0001 C CNN
	1    5200 4350
	1    0    0    -1  
$EndComp
$Comp
L R R17
U 1 1 5DB0805F
P 5800 4450
F 0 "R17" V 5880 4450 50  0000 C CNN
F 1 "10K" V 5800 4450 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 5730 4450 50  0001 C CNN
F 3 "" H 5800 4450 50  0001 C CNN
	1    5800 4450
	1    0    0    -1  
$EndComp
$Comp
L R R20
U 1 1 5DB08060
P 5950 4000
F 0 "R20" V 6030 4000 50  0000 C CNN
F 1 "15K" V 5950 4000 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 5880 4000 50  0001 C CNN
F 3 "" H 5950 4000 50  0001 C CNN
	1    5950 4000
	0    1    1    0   
$EndComp
$Comp
L LM13700 U2
U 2 1 5DB08728
P 5550 1250
F 0 "U2" H 5700 1450 50  0000 C CNN
F 1 "LM13700" H 5750 1050 50  0000 C CNN
F 2 "Housings_DIP:DIP-16_W7.62mm_Socket" H 5450 1275 50  0001 C CNN
F 3 "" H 5450 1275 50  0001 C CNN
	2    5550 1250
	1    0    0    -1  
$EndComp
$Comp
L LM13700 U2
U 4 1 5DB087BF
P 5450 2500
F 0 "U2" H 5600 2700 50  0000 C CNN
F 1 "LM13700" H 5650 2300 50  0000 C CNN
F 2 "Housings_DIP:DIP-16_W7.62mm_Socket" H 5350 2525 50  0001 C CNN
F 3 "" H 5350 2525 50  0001 C CNN
	4    5450 2500
	1    0    0    -1  
$EndComp
Text HLabel 2250 1250 0    60   Output ~ 0
pwm_sig
Text HLabel 2250 2700 0    60   Output ~ 0
sawtooth_sig
Text HLabel 2200 4100 0    60   Output ~ 0
triangle_sig
Text HLabel 7650 1450 2    60   Output ~ 0
mix_out
Text HLabel 7650 2000 2    60   Input ~ 0
pwm_level
Text HLabel 7650 3300 2    60   Input ~ 0
sawtooth_level
Text HLabel 7650 4800 2    60   Input ~ 0
triangle_level
Wire Wire Line
	2900 1450 2800 1450
Wire Wire Line
	2800 1450 2800 1000
Wire Wire Line
	2800 1000 3500 1000
Wire Wire Line
	3500 1000 3500 1350
Wire Wire Line
	3100 1750 3100 1650
Wire Wire Line
	4400 1250 3900 1250
Wire Wire Line
	3900 1250 3900 1200
Wire Wire Line
	4400 1400 4400 1350
Wire Wire Line
	4400 1350 4100 1350
Wire Wire Line
	4400 1150 4100 1150
Wire Wire Line
	4100 1150 4100 1400
Connection ~ 4400 1350
Connection ~ 3500 1350
Wire Wire Line
	4250 2050 4250 1850
Wire Wire Line
	5250 1850 5250 1700
Connection ~ 4250 1850
Wire Wire Line
	4600 1750 4600 1550
Wire Wire Line
	3900 800  3900 900 
Wire Wire Line
	4600 2200 4600 2400
Wire Wire Line
	1450 2300 4600 2300
Wire Wire Line
	3100 2500 3100 2300
Wire Wire Line
	2850 2900 2850 2450
Wire Wire Line
	2850 2450 3500 2450
Wire Wire Line
	3500 2450 3500 2800
Wire Wire Line
	5000 2500 5000 2700
Wire Wire Line
	5150 2500 5150 2850
Wire Wire Line
	3950 2400 3950 2300
Connection ~ 3950 2300
Wire Wire Line
	3950 2700 4400 2700
Wire Wire Line
	4100 2800 4400 2800
Wire Wire Line
	4400 2800 4400 2850
Wire Wire Line
	4100 2850 4100 2600
Wire Wire Line
	4100 2600 4400 2600
Connection ~ 4400 2800
Wire Wire Line
	3100 3100 3100 3200
Wire Wire Line
	4600 3200 4600 3000
Connection ~ 3100 3200
Wire Wire Line
	5150 3350 5150 3150
Connection ~ 3500 2800
Connection ~ 4600 1750
Wire Wire Line
	5000 2500 5150 2500
Wire Wire Line
	5750 2800 5750 2500
Connection ~ 5150 2500
Wire Wire Line
	3900 3750 3900 3650
Wire Wire Line
	1450 3750 4600 3750
Wire Wire Line
	3050 3900 3050 3750
Connection ~ 3900 3750
Wire Wire Line
	3900 4050 4400 4050
Wire Wire Line
	4400 4150 4400 4250
Wire Wire Line
	4050 4200 4400 4200
Connection ~ 4400 4200
Wire Wire Line
	4100 4250 4100 3950
Wire Wire Line
	4100 3950 4400 3950
Wire Wire Line
	5000 4050 5000 4000
Wire Wire Line
	5000 4000 5200 4000
Wire Wire Line
	5200 4000 5200 4200
Wire Wire Line
	3050 4500 3050 4600
Connection ~ 3050 4600
Wire Wire Line
	4600 4600 4600 4350
Wire Wire Line
	5200 4700 5200 4500
Wire Wire Line
	2800 4300 2800 3900
Wire Wire Line
	2800 3900 3450 3900
Wire Wire Line
	3450 3900 3450 4200
Connection ~ 5200 4000
Connection ~ 3450 4200
Wire Wire Line
	6000 4300 5950 4300
Wire Wire Line
	6300 1550 6300 1850
Wire Wire Line
	6900 1850 6900 1450
Connection ~ 6900 1450
Wire Wire Line
	5100 2000 7650 2000
Wire Wire Line
	5000 2800 5000 3300
Wire Wire Line
	5000 3300 7650 3300
Wire Wire Line
	5000 4150 5000 4800
Wire Wire Line
	5000 4800 7650 4800
Wire Wire Line
	1350 4600 4600 4600
Wire Wire Line
	1250 4700 5800 4700
Wire Wire Line
	950  1750 6500 1750
Wire Wire Line
	1350 3200 5750 3200
Wire Wire Line
	4100 1850 5250 1850
Wire Wire Line
	2250 1250 2900 1250
Wire Wire Line
	2850 2900 2900 2900
Wire Wire Line
	2900 2700 2250 2700
Wire Wire Line
	2800 4300 2850 4300
Wire Wire Line
	2200 4100 2850 4100
Wire Wire Line
	3450 4200 3750 4200
Wire Wire Line
	3500 2800 3800 2800
Wire Wire Line
	3500 1350 3800 1350
Wire Wire Line
	6900 1450 7650 1450
Wire Wire Line
	1450 800  1450 3750
Connection ~ 3100 2300
Connection ~ 3050 3750
Connection ~ 1450 2300
Wire Wire Line
	1350 1750 1350 4600
Wire Wire Line
	1250 2050 1250 4700
Connection ~ 1250 2050
Connection ~ 1250 3350
$Comp
L R R7
U 1 1 5DB0CFDF
P 4100 1550
F 0 "R7" V 4180 1550 50  0000 C CNN
F 1 "390" V 4100 1550 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 4030 1550 50  0001 C CNN
F 3 "" H 4100 1550 50  0001 C CNN
	1    4100 1550
	1    0    0    -1  
$EndComp
$Comp
L R R10
U 1 1 5DB0D061
P 4400 1550
F 0 "R10" V 4480 1550 50  0000 C CNN
F 1 "390" V 4400 1550 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 4330 1550 50  0001 C CNN
F 3 "" H 4400 1550 50  0001 C CNN
	1    4400 1550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 1700 4100 1850
Wire Wire Line
	4400 1700 4400 1850
Connection ~ 4400 1850
Wire Wire Line
	1250 3350 5150 3350
$Comp
L R R8
U 1 1 5DB0D536
P 4100 3000
F 0 "R8" V 4180 3000 50  0000 C CNN
F 1 "1K" V 4100 3000 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 4030 3000 50  0001 C CNN
F 3 "" H 4100 3000 50  0001 C CNN
	1    4100 3000
	1    0    0    -1  
$EndComp
$Comp
L R R11
U 1 1 5DB0D61B
P 4400 3000
F 0 "R11" V 4480 3000 50  0000 C CNN
F 1 "1K" V 4400 3000 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 4330 3000 50  0001 C CNN
F 3 "" H 4400 3000 50  0001 C CNN
	1    4400 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 3150 4100 3350
Connection ~ 4100 3350
Wire Wire Line
	4400 3150 4400 3350
Connection ~ 4400 3350
$Comp
L R R9
U 1 1 5DB0E24E
P 4100 4400
F 0 "R9" V 4180 4400 50  0000 C CNN
F 1 "1.2K" V 4100 4400 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 4030 4400 50  0001 C CNN
F 3 "" H 4100 4400 50  0001 C CNN
	1    4100 4400
	1    0    0    -1  
$EndComp
$Comp
L R R12
U 1 1 5DB0E2F6
P 4400 4400
F 0 "R12" V 4480 4400 50  0000 C CNN
F 1 "1.2K" V 4400 4400 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 4330 4400 50  0001 C CNN
F 3 "" H 4400 4400 50  0001 C CNN
	1    4400 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 4700 4100 4550
Connection ~ 4100 4700
Wire Wire Line
	4400 4550 4400 4700
Connection ~ 4400 4700
$Comp
L R R22
U 1 1 5DB103A7
P 6600 1850
F 0 "R22" V 6680 1850 50  0000 C CNN
F 1 "1M" V 6600 1850 50  0000 C CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P5.08mm_Horizontal" V 6530 1850 50  0001 C CNN
F 3 "" H 6600 1850 50  0001 C CNN
	1    6600 1850
	0    1    1    0   
$EndComp
Wire Wire Line
	6300 1850 6450 1850
Wire Wire Line
	6750 1850 6900 1850
$Comp
L LM13700 U3
U 2 1 5DB11DD7
P 5500 4000
F 0 "U3" H 5650 4200 50  0000 C CNN
F 1 "LM13700" H 5700 3800 50  0000 C CNN
F 2 "Housings_DIP:DIP-16_W7.62mm_Socket" H 5400 4025 50  0001 C CNN
F 3 "" H 5400 4025 50  0001 C CNN
	2    5500 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5100 2000 5100 1350
Wire Wire Line
	5100 1350 5000 1350
$Comp
L GND #PWR02
U 1 1 5DB1609A
P 1100 2050
F 0 "#PWR02" H 1100 1800 50  0001 C CNN
F 1 "GND" H 1100 1900 50  0000 C CNN
F 2 "" H 1100 2050 50  0001 C CNN
F 3 "" H 1100 2050 50  0001 C CNN
	1    1100 2050
	1    0    0    -1  
$EndComp
Connection ~ 3900 800 
Wire Wire Line
	3100 800  3100 1050
Connection ~ 3100 800 
Wire Wire Line
	5850 1450 5850 1250
Connection ~ 3100 1750
Connection ~ 1350 1750
Connection ~ 1350 3200
Wire Wire Line
	5550 1400 5550 1750
Connection ~ 5550 1750
Wire Wire Line
	5450 2200 4600 2200
Connection ~ 4600 2300
Wire Wire Line
	5500 3700 4600 3700
Wire Wire Line
	4600 3700 4600 3750
Wire Wire Line
	5800 4300 5800 4000
Wire Wire Line
	6150 1250 6150 4000
Wire Wire Line
	6150 1350 6300 1350
Wire Wire Line
	6150 2500 6050 2500
Connection ~ 6150 1350
Wire Wire Line
	6150 4000 6100 4000
Connection ~ 6150 2500
Connection ~ 5850 1750
Connection ~ 4600 800 
Wire Wire Line
	6500 800  6500 1150
Connection ~ 5550 800 
Wire Wire Line
	5000 1250 5250 1250
Wire Wire Line
	5250 1250 5250 1400
Wire Wire Line
	5550 800  5550 950 
Connection ~ 1450 800 
Connection ~ 6500 1750
Connection ~ 5550 1000
Wire Wire Line
	1100 2050 4250 2050
Wire Wire Line
	1100 800  6500 800 
Wire Wire Line
	4600 800  4600 950 
Wire Wire Line
	5450 3200 5450 2800
Connection ~ 4600 3200
Wire Wire Line
	5750 3200 5750 3100
Connection ~ 5450 3200
Wire Wire Line
	5800 4700 5800 4600
Connection ~ 5200 4700
Connection ~ 5500 4700
Wire Wire Line
	5500 4300 5500 4700
$Comp
L VCC #PWR03
U 1 1 5DB1AE48
P 1100 800
F 0 "#PWR03" H 1100 650 50  0001 C CNN
F 1 "VCC" H 1100 950 50  0000 C CNN
F 2 "" H 1100 800 50  0001 C CNN
F 3 "" H 1100 800 50  0001 C CNN
	1    1100 800 
	1    0    0    -1  
$EndComp
$Comp
L VEE #PWR04
U 1 1 5DB1AEEB
P 950 1750
F 0 "#PWR04" H 950 1600 50  0001 C CNN
F 1 "VEE" H 950 1900 50  0000 C CNN
F 2 "" H 950 1750 50  0001 C CNN
F 3 "" H 950 1750 50  0001 C CNN
	1    950  1750
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG05
U 1 1 5DB1BBDE
P 3050 3750
F 0 "#FLG05" H 3050 3825 50  0001 C CNN
F 1 "PWR_FLAG" H 3050 3900 50  0000 C CNN
F 2 "" H 3050 3750 50  0001 C CNN
F 3 "" H 3050 3750 50  0001 C CNN
	1    3050 3750
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG06
U 1 1 5DB1BE8E
P 3050 4500
F 0 "#FLG06" H 3050 4575 50  0001 C CNN
F 1 "PWR_FLAG" H 3050 4650 50  0000 C CNN
F 2 "" H 3050 4500 50  0001 C CNN
F 3 "" H 3050 4500 50  0001 C CNN
	1    3050 4500
	0    1    1    0   
$EndComp
$EndSCHEMATC
