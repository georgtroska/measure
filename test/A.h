#ifndef A_h
#define A_h
#include "oneheader.h"

class __declspec(dllexport) A : public Device {
	public:
	class __declspec(dllexport) Input { 
		public:
		A &_dev;
		Device &getDevice() { return _dev; }
		Input(A *d) : _dev(*d) {} 
		virtual double operator()()  {
			std::cout << "A::input()" << std::endl;
			return _dev.read();
		}
		virtual ~Input() {}
	} input;
	
	A() : Device(), input(this) {}
	virtual ~A() {}
	
};

#ifdef __CLING__
#pragma link C++ class A-;
#endif

#endif