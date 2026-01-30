R__LOAD_LIBRARY(test)
#include "../../oneheader.h"
#include "../../A.h"
#include <iostream>
int test() {
	A *a = new A();
	std::cout << a->input() << std::endl;
	/*B *b = new B();
	b->output1(42);
	b->output2(42);
	*/
	return 0;
}