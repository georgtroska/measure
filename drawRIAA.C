#include <cmath>
int drawRIAA() {
	TCanvas *c1 = new TCanvas();
	TGraph *grRIAA = new TGraph();
	TGraph *grAIEC = new TGraph();
	
	double riaa, aiec;
	double frequency;
	double omega;
	double pi = 3.1415;
	
	const double inputAmplitude = 1.;
	const double tauLowPass1 = 3180e-6; //Unit s
	const double tauHighIncrease = 318e-6; //Unit s
	const double tauLowPass2 = 75e-6; //Unit s
	const double tauHighPass = 7950e-6; //Unit s, Only for AIEC
	
	for (int i = 0; i < 250; i++) {
		
		//calc frequency for exp. increase
		frequency = 1*exp(i*log(100)/96);
		omega = frequency*2*pi;
		
		riaa = inputAmplitude;
		riaa *= 1/sqrt(1+(omega*tauLowPass1)*(omega*tauLowPass1)); //Apply first filter
		riaa *= sqrt(1+(omega*tauHighIncrease)*(omega*tauHighIncrease)); //Apply second filter
		riaa *= 1/sqrt(1+(omega*tauLowPass2)*(omega*tauLowPass2)); //Apply third filter
		aiec = riaa;
		aiec *= 1/sqrt(1+1/((omega*tauHighPass)*(omega*tauHighPass))); //Apply fourth filter only for aiec
		
		grRIAA->SetPoint(grRIAA->GetN(), frequency, riaa); //10*log(riaa));
		grAIEC->SetPoint(grAIEC->GetN(), frequency, aiec); //10*log(aiec));
		
		
	}
	
	
	TMultiGraph *mgr = new TMultiGraph();
	mgr->Add(grRIAA,"lp");
	mgr->Add(grAIEC,"lp");
	c1->cd();
	gPad->SetLogx();
	mgr->Draw("alp");
	
	return 0;
	
	
}
