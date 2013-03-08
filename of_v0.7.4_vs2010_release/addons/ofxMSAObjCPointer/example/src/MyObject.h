#include "MSAObjCPointer.h"

#pragma once

/********** Example dummy class which inherits MSAObjCPointer functionality *************/
class MyObject : public msa::ObjCPointer {
public:
	MyObject(float _a, float _b, float _c, float _d) {
		a = _a;
		b = _b;
		c = _c;
		d = _d;
	}
	
	float a, b, c, d;
};