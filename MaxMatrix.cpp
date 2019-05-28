/*
 * MaxMatrix
 * Version 1.0 Feb 2013
 * Copyright 2013 Oscar Kin-Chung Au
 */


#include "Arduino.h"
#include "MaxMatrix.h"

MaxMatrix::MaxMatrix(byte _data, byte _load, byte _clock, byte _num) 
{
	data = _data;
	load = _load;
	clock = _clock;
	num = _num;
	for (int i=0; i<BUFSIZE; i++)
		buffer[i] = 0;
}

void MaxMatrix::init()
{
	pinMode(data,  OUTPUT);
	pinMode(clock, OUTPUT);
	pinMode(load,  OUTPUT);
	digitalWrite(clock, HIGH); 

	setCommand(max7219_reg_scanLimit, 0x07);      
	setCommand(max7219_reg_decodeMode, 0x00);  // using an led matrix (not digits)
	setCommand(max7219_reg_shutdown, 0x01);    // not in shutdown mode
	setCommand(max7219_reg_displayTest, 0x00); // no display test
	
	// empty registers, turn all LEDs off
	clearMatrix();
	
	setIntensity(0x0f);    // the first 0x0f is the value you can set
}

void MaxMatrix::setIntensity(byte intensity)
{
	setCommand(max7219_reg_intensity, intensity);
}

void MaxMatrix::clearMatrix()
{
	for (int i=0; i<BUFSIZE; i++)
		buffer[i] = 0;
	flush();
}

void MaxMatrix::setCommand(byte command, byte value)
{
	digitalWrite(load, LOW);    
	for (int i=0; i<num; i++) 
	{
		shiftOut(data, clock, MSBFIRST, command);
		shiftOut(data, clock, MSBFIRST, value);
	}
	digitalWrite(load, LOW);
	digitalWrite(load, HIGH);
}

// rutina para Zowi, para meter sus caritas en la matriz de 8
void MaxMatrix::writeFull(unsigned long value) {
	for (int r=0; r<5;r++){
		for (int c=0; c<6; c++){
			bitWrite(buffer[6-c], 7-r, (1L & (value >> r*6+c)));
		}
	}
	flush();
}

void MaxMatrix::flush() {
	digitalWrite(load, LOW);    
	for (int i=0; i<num; i++) 
	{
		for(int c=0; c<8; c++)
		{
			shiftOut(data, clock, MSBFIRST, c + 1);
			shiftOut(data, clock, MSBFIRST, buffer[i*8+c]);
		}
	}
	digitalWrite(load, LOW);
	digitalWrite(load, HIGH);
}

