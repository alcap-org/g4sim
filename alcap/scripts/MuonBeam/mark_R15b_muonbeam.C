/**
 *	This script reads the ROOT output of g4sim typically generated with a macro file that contains the R15b_gen_mu_preSiT generator.
 *	It produces several histograms for sanity checks plus the mustopdist histogram for use in the next step of the MC process,
 *	which is to provide initial positions for the generation of charged particles. The name of this histogram
 *	is defined as the value of PMHFN in the file defined as the value of PMHFN.
 *
 */
#include "TFile.h"
#include "TH3.h"
#include "TH2.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
void mark_R15b_muonbeam(std::string filename = "output/raw_g4sim.root") {
	TChain* tree = new TChain("tree");
	tree->Add(filename.c_str());
	if(tree->GetNtrees()<1) return;
	std::vector<std::string>* particleName = 0;
	std::vector<std::string>* volName = 0;
	std::vector<std::string>* oprocess = 0;
	std::vector<double>* edep = 0;
	std::vector<int>* stopped = 0;
	std::vector<double>* Ox = 0;
	std::vector<double>* Oy = 0;
	std::vector<double>* Oz = 0;

	tree->SetBranchAddress("M_particleName", &particleName);
	tree->SetBranchAddress("M_volName"     , &volName);
	tree->SetBranchAddress("M_oprocess"    , &oprocess);
	tree->SetBranchAddress("M_edep"        , &edep);
	tree->SetBranchAddress("M_stopped"     , &stopped);
	tree->SetBranchAddress("M_local_Ox"    , &Ox);
	tree->SetBranchAddress("M_local_Oy"    , &Oy);
	tree->SetBranchAddress("M_local_Oz"    , &Oz);

	Double_t SiT_mu_E = 0;
	Double_t Target_E = 0;
	Double_t SiT_x = 0;
	Double_t SiT_y = 0;
	Double_t Target_x = 0;

	std::string outputFilename = "muons." + filename.substr(filename.find_last_of("/")+1 );
	TFile* outfile = new  TFile(outputFilename.c_str(), "RECREATE");
	TH1* hSiT_mu = new TH1F("hSiT_mu", "SiT #mu^{-} energy deposit;E [MeV]", 5000,  0, 2);
	TH2* hSurface=new TH2F("hSiTsurface", "SiT surface hits;x [mm];y [mm]", 200,  -50, 50, 200, -50, 50);
	TH1* hTarget = new TH1F("hTargetEnergy", "Target energy deposit; E [MeV]", 5000,  0, 2);
	TH3* hMuStopDist=new TH3F("mustopdist", "Muon Stopping Distribution;x [mm];y [mm];z [mm];", 200, 50, -50, 200, -50, 50, 400, -0.05, 0.05);
	for (int i_entry = 0; i_entry < tree->GetEntries(); ++i_entry) {
		tree->GetEvent(i_entry);
		if (i_entry % 1000000 == 0) {
			std::cout << i_entry << " / " << tree->GetEntries() << std::endl;
		}
		//reset all vars
		SiT_mu_E = 0;
		SiT_y=0;
		SiT_x=0;
		Target_E = 0;
		for (unsigned iElement = 0; iElement < particleName->size(); ++iElement) {
			double i_edep = edep->at(iElement)*1e3; // convert to MeV
			if(volName->at(iElement) == "Target") {
				if(particleName->at(iElement) == "mu-") {
					if(stopped->at(iElement) ) {
						hMuStopDist->Fill(Ox->at(iElement) * 10, Oy->at(iElement) * 10, Oz->at(iElement) * 10);
						Target_E+=i_edep;
					}
				}
			}
			if(volName->at(iElement) == "SiT") {
				if(particleName->at(iElement) == "mu-") {
					SiT_mu_E+=i_edep;
				}
				SiT_x = Ox->at(iElement) * 10;
				SiT_y = Oy->at(iElement) * 10;
			}
		}
		if(SiT_x!=0 || SiT_y!=0) {
			hSurface->Fill(SiT_x, SiT_y);
		}
		if(Target_E!=0) {
			hTarget->Fill(Target_E);
		}
		if(SiT_mu_E!=0) {
			hSiT_mu->Fill(SiT_mu_E);
		}
	}
	outfile->Write();
}
