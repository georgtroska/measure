#ifndef EXCLIMIT_H
#define EXCLIMIT_H

#include <exception>
#include <stdexcept>
#include<iostream>
#include<string.h>

class __declspec(dllexport) ExcLimit : public std::exception{
	std::string _devName;
	std::string _limitKind;
	std::string _channelName;
	std::string _what;
	double _limitVal;
	double _lastVal;
	
public:
	ExcLimit(std::string w, std::string name, std::string kind, std::string chName, double limValue, double lastValue):  _what(w), _devName(name), _limitKind(kind), _channelName(chName), _limitVal(limValue), _lastVal(lastValue) {}
	ExcLimit(std::string w): _what(w) {}
	ExcLimit(): _what("An unspecified Limit Exception occurred!") {}
	std::string getDevName () throw() {return _devName;}
	std::string getLimitKind () throw() {return _limitKind;}
	std::string getChannelName () throw() {return _channelName;}
	double getLimitVal () throw() {return _limitVal;}
	double getLastVal () throw() { return _lastVal; }
	//double getLastParam () throw() {return _lastParam; }
	virtual const char* what() const throw() {return _what.c_str();}
	~ExcLimit() throw() {}
};

#ifdef __CLING__
#pragma link C++ class ExcLimit-;
#endif

#endif