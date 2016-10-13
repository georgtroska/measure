#include "Window1DGraph.h"
#include <iostream>
#include <TAxis.h>
#include <string>

using namespace std;

Window1DGraph::Window1DGraph(Scan1D* scan, unsigned int w, unsigned int h, unsigned int posX, unsigned int posY, unsigned int channelNum) : _scan(*scan), _channelNum(channelNum) {
	cout << "Creating new Window1DGraph (" <<  channelNum << ")" <<endl;
	char canvName[255];
	sprintf(canvName,"GraphCanvas_Channel%i", channelNum);
	_canv = new TCanvas(canvName, canvName, posX, posY, w, h);
	_canv->Divide(1,2);
	
	
	_graphMean = new TGraphErrors();
	_graphSingle = new TGraph();
	
	_nPointsSet = 0;

	cout << "Size: " << _scan.getChannels().size() << endl;
	_channel = _scan.getChannels().at(channelNum);
	cout << "Creation of graph "<< channelNum <<" is done." << endl;
	
}

void Window1DGraph::applyLog() {
	_canv->cd(1);
	gPad->SetLogx();
}

void Window1DGraph::addPoint(MeasEvent* event) {	
	_graphMean->SetPoint(_nPointsSet, event->param, event->channelEvent.at(_channelNum).value);
	_graphMean->SetPointError(_nPointsSet, 1e-12, event->channelEvent.at(_channelNum).RMS);
	
	for(int i=0; i < event->channelEvent.at(_channelNum).singleValue.size(); i++) {
		_graphSingle->SetPoint(i , i+1, event->channelEvent.at(_channelNum).singleValue.at(i));
	}
	_nPointsSet++;
	update();
}

void Window1DGraph::update() {
		makeAxisTitles();
		_canv->cd(1);
		_graphMean->Draw("AC*");
		_canv->Update();
		
		_canv->cd(2);
		_graphSingle->Draw("AC*");
		_canv->Update();

}

void Window1DGraph::makeAxisTitles() {
	_graphMean->SetTitle("Mean");
	_graphMean->GetXaxis()->SetTitle((_scan.getParam()->getKind() + " [" + _scan.getParam()->getUnit() + "]").c_str());
	_graphMean->GetYaxis()->SetTitle((_channel->getKind() + " [" + _channel->getUnit() + "]").c_str());
	
	_graphSingle->SetTitle("SingleValues");
	_graphSingle->GetXaxis()->SetTitle("MeasNumber");
	_graphSingle->GetYaxis()->SetTitle((_channel->getKind() + " [" + _channel->getUnit() + "]").c_str());
}

Window1DGraph::~Window1DGraph() {
	delete _graphSingle;
	delete _graphMean;
	delete _canv;
	
}
/*
Window1DGraph::Window1DGraph(const TGWindow *p,UInt_t w,UInt_t h/*,  unsigned int posX, unsigned int posY): TGMainFrame(p, w, h) {
	cout << "Creating new Window1DGraph" << endl;
	MapWindow();
	_horFrameMean = new TGHorizontalFrame(this);
	
		_eCanvasMean = new TRootEmbeddedCanvas("IV-Curve",_horFrameMean);//,50,50); 
		_horFrameMean->AddFrame(_eCanvasMean, new TGLayoutHints(kLHintsExpandX| kLHintsExpandY, 5,5,5,5));  
	
	
	_horFrameSingle = new TGHorizontalFrame(this);
	
		_eCanvasSingle = new TRootEmbeddedCanvas("Averageing",_horFrameSingle,50,50); 
		_horFrameSingle->AddFrame(_eCanvasSingle, new TGLayoutHints(kLHintsExpandX| kLHintsExpandY, 5,5,5,5));  
	
	AddFrame(_horFrameMean, new TGLayoutHints(kLHintsExpandX| kLHintsExpandY, 5,5,5,5));    
	AddFrame(_horFrameSingle, new TGLayoutHints(kLHintsExpandX| kLHintsExpandY, 5,5,5,5));    
	
	SetWindowName("IV Graph"); 
	MapSubwindows();
	Resize(w,h);
	
	MapWindow();
	
	Move(posX, posY);
	MapWindow();
	
	_graphMean = new TGraphErrors();
	_graphSingle = new TGraph();
	
	_nPointsSet = 0;
}

void Window1DGraph::addPoint(MeasEvent* event) {	
	_graphMean->SetPoint(_nPointsSet, event->param, event->channelEvent.at(0).value);
	_graphMean->SetPointError(_nPointsSet, 1e-12, event->channelEvent.at(0).RMS);
	
	for(int i=0; i < event->channelEvent.at(0).singleValue.size(); i++) {
		_graphSingle->SetPoint(i , i+1, event->channelEvent.at(0).singleValue.at(i));
	}
	_nPointsSet++;
	update();
}

void Window1DGraph::update() {
	TCanvas *fCanvas = _eCanvasMean->GetCanvas();
	fCanvas->cd();
	_graphMean->Draw("AC*");
	fCanvas->Update();
	
	fCanvas = _eCanvasSingle->GetCanvas();
	fCanvas->cd();
	_graphSingle->Draw("AC*");
	fCanvas->Update();
}

Window1DGraph::~Window1DGraph() {
	delete _graphSingle;
	delete _graphMean;
	delete _eCanvasSingle;
	delete _horFrameSingle;
	delete _eCanvasMean;
	delete _horFrameMean;
}

*/

