#ifndef oneheader_h
#define oneheader_h
#include <iostream>

class __declspec(dllexport) Device {
	public:
	Device() {}
	virtual ~Device() {}
	double read() { return 42; }
	void write(double p) { std::cout << "writing " << p << std::endl; }
};

#ifdef __CLING__
#pragma link C++ class Device-;

#endif

#endif
