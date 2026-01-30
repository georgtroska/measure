R__LOAD_LIBRARY(build/Release/test)
#include "../../oneheader.h"
#include <iostream>
int test() {
	A *a = new A();
	std::cout << a->input() << std::endl;
	B *b = new B();
	std::cout << b->input() << std::endl;
	return 0;
}