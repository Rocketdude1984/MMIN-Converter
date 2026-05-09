# MMIN-Converter
Simple converter for mm to inches and inches to mm. Meant as a project to advance skills in PCB design and firmware development.

## Prototype
- Button Matrix
- 128x32 OLED
- STM32F103C8 MCU
- TPS63020 buck-boost converter (3.3v)
- TPS61040 boost converter (12v)
- 2xAA battery holder

<img src="docs/Images/MINNConverter-Prototype.jpg" width="50%">

Production took a while longer than expected, but the project is complete.

After correcting a few lines of code the converter works great.

## Construction

1. Make sure to solder the battery holder correctly.
2. Connect the OLED display to the connector before glueing any parts on.
3. I used a resin printer to make the battery shim and display holder and glued them to the board with gorilla glue.

## Changes
I made a few changes to the PCB based on what I learned from this first model.
Mainly I forgot to put the pin designations for the serial connections on the silkscreen. And having an "ON/OFF" indication would have been nice. I tried to implement that in a new version of the board, but it is still not great because screen and battery comepletely cover each side of the switch.
