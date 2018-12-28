#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
//compare TALYS calculation with unfolded proton spectra
void Reader() {
	gROOT->ProcessLine(".L /home/m-wong/bayesian-unfolding/libRooUnfold.so");

	TFile *fUnfolded = new TFile("/home/m-wong/data/unfolded/unfolded.unfolding.localhisto.7m.root", "READ"); //without Fake() events
	//TFile *fUnfolded = new TFile("/home/m-wong/data/unfolded.transfer.pdt.localhisto.7m.root", "READ"); //with Fake() events
	TH1D *hRecoRight = (TH1D *)fUnfolded->Get("hRecoRight"); //Unfolded from al50 data
	hRecoRight->SetTitle("Right Al50 data unfolded with unbiased energy");
	hRecoRight->SetLineColor(kRed);
	hRecoRight->Draw();
	TH1D *hRecoLeft = (TH1D *)fUnfolded->Get("hRecoLeft"); //Unfolded from al50 data
	hRecoLeft->SetTitle("Left Al50 data unfolded with unbiased energy");
	hRecoLeft->SetLineColor(kBlue);
	hRecoLeft->Draw("SAME");

	TString csv[6] = {"neutron.csv", "proton.csv", "deuteron.csv", "triton.csv", "he3.csv", "alpha.csv"};
	for(int j=1; j<2; j++) { //just show protons for now
		std::ifstream data(csv[j]);
		std::string line;
		int linenum=0;

		std::vector<Double_t> x;
		std::vector<Double_t> y;
		while(std::getline(data,line))
		{
			if(linenum==0) {
				linenum++; continue;
			};
			std::stringstream  lineStream(line);
			std::string        cell;
			int i=0;
			while(std::getline(lineStream,cell, ' '))
			{
				if(i == 0) {
					Double_t energybin = atof(cell.c_str() );
					x.push_back(1e3*energybin);
				}
				if(i == 1) {
					Double_t counts = atof(cell.c_str() );
					y.push_back(4.5e4*counts);
				}
				i++;
			}
		}
		//TString gName = csv[j](0, csv[j].First(".") );
		TString gName = TString("TALYS");
		TGraph *g = new TGraph(x.size(), &x[0], &y[0]);
		g->SetTitle(gName.Data() );
		g->GetYaxis()->SetTitle("Arbitrary units");
		g->GetXaxis()->SetTitle("E [MeV]");
		g->SetLineColor(kGreen);
		g->SetLineWidth(3);
		g->SetLineStyle(kDashed);
		g->Draw("SAME");
	}
}
