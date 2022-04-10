#include "Arduino.h"
#include "Pedal.h"

Pedal::Pedal(uint8_t newPinA, uint8_t newPinB, int newPinGND, int newPinSense)
{
	pinA = newPinA;
	pinB = newPinB;
	pinGND = newPinGND;
	pinSense = newPinSense;

	setupPins();
	_updated = false;
}

void Pedal::setupPins(){

	uint8_t pinAMode = mode==1?INPUT_PULLUP:type==1?OUTPUT:INPUT;
	uint8_t pinBMode = mode==1?OUTPUT:type==1?INPUT:OUTPUT;

	pinMode(pinA, pinAMode);
	pinMode(pinB, pinBMode);
	pinMode(pinGND, OUTPUT);
	pinMode(pinSense, INPUT_PULLUP);

	digitalWrite(pinGND, LOW);

	if(mode==1){
		digitalWrite(pinB, LOW);
	}
	else{
		digitalWrite(type==1?pinA:pinB, HIGH);
	}
}

void Pedal::setCC(int newCC){
	_updated = true;
	cc = newCC;
}

void Pedal::setValue(int newValue){
	newValue = polarity?newValue:127-newValue;

	if (mode == 1){
		newValue = newValue > 64 ? 127 : 0;
	}
	if(newValue != value){
		value = newValue;
		_updated = true;
	}
}
void Pedal::setEnabled(bool newEnabled){		
	_updated = true;
	enabled = newEnabled;
}

void Pedal::setMode(int newMode){
	_updated = true;
	mode = newMode;
	setupPins();
}

void Pedal::setType(int newType){
	_updated = true;
	type = newType;
	setupPins();
}

void Pedal::setPolarity(int newPolarity){
	_updated = true;
	polarity = newPolarity;
}

bool Pedal::hasBeenUpdated(){
	bool previouslyUpdated = _updated;
	_updated = false;
	return previouslyUpdated;
}


void Pedal::checkInput(){
	bool newconnected = digitalRead(pinSense);
	if(!newconnected && connected){
		setValue(0);
	}
	connected = newconnected;
	if(connected){
		uint8_t readPin = mode==1?pinA:type==1?pinB:pinA;
		int readValue = analogRead(readPin) >> 3;
		setValue(readValue);
	}
}

void Pedal::reset(){
	_updated = false;
	value = 0;
}