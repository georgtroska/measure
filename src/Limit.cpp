#include "Limit.h"
#include "ExcLimit.h"

using namespace std;

void Limit::setPosLimit (double l) {
	if ( l < 0 ) throw ExcLimit("Exception while setting upper limit. Please choose positive upper Limit!");
	else {
		_posLimit =  l; 
		_isPosLimitEnabled = true;
	}
 }
 
 void Limit::setNegLimit (double l) {
	if ( l > 0 ) throw ExcLimit("Exception while setting lower limit. Please choose negative lower Limit!");
	else {
		_negLimit =  l; 
		_isNegLimitEnabled = true;
	}
 }