#ifndef Window1DGraph_h
#define Window1DGraph_h

#include <TCanvas.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include<RQ_OBJECT.h>

#include "MeasEvent.h"
#include "Scan1D.h"

class Scan1D;

class Window1DGraph {
	RQ_OBJECT("Window1DGraph")
protected:
	TCanvas* _canv;
	Scan1D& _scan;

	TGraphErrors* _graphMean;
	TGraph* _graphSingle;

	double _nPointsSet;
	const unsigned int _channelNum;
	Channel* _channel;

	void makeAxisTitles();

public:
	Window1DGraph(Scan1D* scan, unsigned int w, unsigned int h, unsigned int posX, unsigned int posY, unsigned int channelNum);

	void addPoint(MeasEvent* event); 	//*RECEIVER*
	void update();
	
	virtual ~Window1DGraph();
};

/*
class Window1DGraph : public TGMainFrame {
	//RQ_OBJECT("Window1DGraph")
protected:
	TGHorizontalFrame* _horFrameMean;
	TGHorizontalFrame* _horFrameSingle;

	TRootEmbeddedCanvas* _eCanvasMean; 
	TRootEmbeddedCanvas* _eCanvasSingle;
	
	
	TGraphErrors* _graphMean;
	TGraph* _graphSingle;

	double _nPointsSet;

	
public:
	Window1DGraph(const TGWindow *p,UInt_t w, UInt_t h,unsigned int posX, unsigned int posY); 

	void addPoint(MeasEvent* event); 	//*RECEIVER*
	void update();
	
	virtual ~Window1DGraph();
	
};
*/

#ifdef __CINT__
#pragma link C++ class Window1DGraph-;
#endif

#endif