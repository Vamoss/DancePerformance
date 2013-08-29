#include "NanoSoftSensor.h" 

void NanoSoftSensor::setup(int _readings){
	if(_readings > READINGS){
		readings = READINGS;
	} else {
		readings = _readings;
	}
	for (int i = 0; i < readings; i++){
		array[i] = 0;
	}
	index = 0;
	total = 0;
}

int NanoSoftSensor::update(int _value) {
	total -= array[index];
	array[index] = _value;
	total += array[index];
	index = (index + 1) % readings;
	value = total / readings;
	return value;
}