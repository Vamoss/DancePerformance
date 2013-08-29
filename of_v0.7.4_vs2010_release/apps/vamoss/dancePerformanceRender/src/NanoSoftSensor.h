//REF: https://github.com/hiperorganicos/ino-nanosoftsensor

#pragma once


#define READINGS 30

class NanoSoftSensor {
	public:
		void setup(int _readings);
		int update(int _value);
		int value;

	private:
		int index;
		int total;
		int readings;
		int array[READINGS];
};