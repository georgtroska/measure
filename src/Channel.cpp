#include "Channel.h"
#include "ExcLimit.h"

void Channel::checkLimit() {
	double actualValue = (*this)();
	if ( _isPosLimitEnabled && actualValue > _posLimit) 
		throw ExcLimit("Limit Exception!", getDevice().getName(), _kind, _saveName, _posLimit, actualValue);
	else if ( _isNegLimitEnabled && actualValue < _negLimit) 
		throw ExcLimit("Limit Exception!", getDevice().getName(), _kind, _saveName, _negLimit, actualValue);
}
