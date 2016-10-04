#include "Signal.h"
#include <iostream>

using namespace std;

void Signal::changedParam() {
	Emit("changedParam()");
}

void Signal::measuredOnce(double val) {
	Emit("measuredOnce(double)", val);
}

void Signal::measuredMean(double mean, double rms) {
	EmitVA("measuredMean(double, double)", 2, mean, rms);
}
