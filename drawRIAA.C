#include <cmath>
int drawRIAA(std::string in = "", std::string inNull = "") {
	TFile *f = NULL;
	TTree *t = NULL;
	
	TFile *fNull = NULL;
	TTree *tNull = NULL;
	if (in != "") {
		f = new TFile(in.c_str(),"READ");
		t = (TTree*) f->Get("Data");
		
	}
	
	if (inNull != "") {
		fNull = new TFile(inNull.c_str(),"READ");
		tNull = (TTree*) fNull->Get("Data");
	}
	TCanvas *c1 = new TCanvas();
	TGraph *grRIAA = new TGraph();
	TGraph *grAIEC = new TGraph();
	
	if (f && !fNull) c1->Divide(1,2);
	if (fNull) c1->Divide(1,3);
	double riaa, aiec;
	double frequency;
	double omega;
	double pi = 3.1415;
	
	const double inputAmplitude = 1.;
	const double tauLowPass1 = 3180e-6; //Unit s
	const double tauHighIncrease = 318e-6; //Unit s
	const double tauLowPass2 = 75e-6; //Unit s
	const double tauHighPass = 7950e-6; //Unit s, Only for AIEC
	
	const int stepsPerDecade = 12;
	for (int decade = 0; decade < 5; decade++) {
		for (int fStep = 0; fStep < stepsPerDecade; fStep++) {
			frequency = TMath::Power(TMath::Power(10,1./stepsPerDecade),fStep);
			frequency *= TMath::Power(10,decade);
		
			/*
			//calc frequency for exp. increase
			frequency = 1*exp(i*log(100)/96);
			*/
			omega = frequency*2*pi;
			
			riaa = inputAmplitude;
			riaa *= 1/sqrt(1+(omega*tauLowPass1)*(omega*tauLowPass1)); //Apply first filter
			riaa *= sqrt(1+(omega*tauHighIncrease)*(omega*tauHighIncrease)); //Apply second filter
			riaa *= 1/sqrt(1+(omega*tauLowPass2)*(omega*tauLowPass2)); //Apply third filter
			aiec = riaa;
			aiec *= 1/sqrt(1+1/((omega*tauHighPass)*(omega*tauHighPass))); //Apply fourth filter only for aiec
			
			grRIAA->SetPoint(grRIAA->GetN(), frequency, riaa); //10*log(riaa));
			//grAIEC->SetPoint(grAIEC->GetN(), frequency, aiec); //10*log(aiec));
		}
	}
	
	
	/*TMultiGraph *mgr = new TMultiGraph();
	mgr->Add(grRIAA,"lp");
	mgr->Add(grAIEC,"lp");
	*/
	if (f) c1->cd(1); else c1->cd();
	gPad->SetLogx();
	//mgr->Draw("alp");
	grRIAA->Draw("alp");
	if (f) {
		
		
		c1->cd(2);
		int n = t->Draw("U:f>>riaa(1000,0,1)","","goff");
		gPad->SetLogx();
		TGraph *grPhono = new TGraph(n,t->GetV2(),t->GetV1());
		grPhono->Draw("al");
		double phonoRef = grPhono->Eval(1000);
		double riaaRef = grRIAA->Eval(1000);
		for (int i=0;i<grPhono->GetN();i++) grPhono->GetY()[i] *= 1./phonoRef;
		for (int i=0;i<grRIAA->GetN();i++) grRIAA->GetY()[i] *= 1./riaaRef;
			c1->cd(1);
		grPhono->Draw("al");
		c1->cd(2);
		grRIAA->Draw("al");
		
		TCanvas *c3 = new TCanvas();
		gPad->SetLogx();
		TMultiGraph *mgr = new TMultiGraph();
		mgr->Add(grRIAA,"lp");
		mgr->Add(grPhono,"lp");
		grPhono->SetLineColor(kRed);
		mgr->Draw("alp");
		
		TGraph *grNull;
		if (fNull) {
			
			
			c1->cd(3);
			int n = tNull->Draw("U:f>>null(1000,0,1)","","goff");
			gPad->SetLogx();
			grNull = new TGraph(n,tNull->GetV2(),tNull->GetV1());
			double nullRef = grNull->Eval(1000);
			for (int i=0;i<grNull->GetN();i++) grNull->GetY()[i] *= 1./nullRef;
			grNull->Draw("al");
		}
		TCanvas *c2 = new TCanvas();
		TGraph *grOffset = new TGraph();
		TGraph *grOffset2 = new TGraph();
		for (int i=0;i<grPhono->GetN();i++) {
			double_t myX = grPhono->GetX()[i];
			//grOffset->SetPoint(i,grPhono->GetX()[i], grPhono->Eval(grPhono->GetX()[i])/grRIAA->Eval(grPhono->GetX()[i]));
			grOffset->SetPoint(i,myX, 10*log10(grPhono->Eval(myX)/grRIAA->Eval(myX)));
			grOffset2->SetPoint(i,myX, 10*log10(grPhono->Eval(myX)/grRIAA->Eval(myX)/grNull->Eval(myX)));

		}
		grOffset->Draw("al");
		grOffset->GetXaxis()->SetTitle("Frequency [Hz]");
		grOffset->GetYaxis()->SetTitle("10*log(U_{Phono}/U_{RIA}) [dB]");
		grOffset2->SetLineColor(kBlue);
		grOffset2->Draw("l");
		gPad->SetLogx();
		gPad->SetGrid(1,1);
	}
	
	return 0;
	
	
}
