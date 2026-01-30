#ifndef SCANSAVE_H
#define SCANSAVE_H

#include "Scan1D.h"
#include "MeasEvent.h"
//#include "ScanConditions.h"
#include <RQ_OBJECT.h>
#include <TTree.h>
#include <TFile.h>
#include <string>
#include <cstdio>


class __declspec(dllexport) ScanSave {
	RQ_OBJECT("ScanSave")
	
	Scan1D & _scan;
	TTree * _dataTree;
	//TTree * _conditionsTree;
	TFile* _saveFile;	
	
	MeasEvent _activeMeasEvent;
	//ScanConditions _cond;

	void setSaveFileName(std::string name);
	//void setUserInfo(ScanConditions* cond);
	//void setConditionsTree(ScanConditions* cond);
		
public:
	ScanSave (Scan1D* scan);
	

	void write(std::string name = "noname");
	//void setConditions(ScanConditions* cond);
	void receiveData (MeasEvent* event);					//*RECEIVER*
	void setBranches(MeasEvent * bore);					//*RECEIVER*
		
	
		
	virtual ~ScanSave();
};

#ifdef __CLING__
#pragma link C++ class ScanSave-;
#endif



#endif
