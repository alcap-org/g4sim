#include "TFile.h"
#include "TTree.h"
#include "TFitResult.h"

#include <iostream>
#include <vector>
#include <string>
#include <math.h>
Double_t smear (const char * detector) {
	Double_t x= gRandom->Rndm();
	Double_t res = 0;
	if(detector == "SiR1") {
		res = 100/2.355;
	} else if(detector == "SiR2") {
		res = 80/2.355;
	} else if(detector == "SiR3") {
		res = 150/2.355;
	}
	Double_t xsmear = gRandom->Gaus(0, res);     // bias and smear
	return xsmear;
}
static const double degree = 45 / std::atan(1.);
void R15b_TM(const char * filename) {
	gROOT->ProcessLine(".L /home/m-wong/bayesian-unfolding/libRooUnfold.so");
	double M = 938.27;
	const char *path = "/home/m-wong/g4sim/alcap/output";
	TFile *file = new TFile(Form("%s/%s", path, filename), "READ");
	TTree *tree = (TTree *)file->Get("tree");

	std::vector<std::string>* particleName = 0;
	std::vector<std::string>* volName = 0;
	std::vector<std::string>* ovolName = 0;
	std::vector<std::string>* oprocess = 0;
	std::vector<int>* stopped = 0;
	std::vector<double>* Ox = 0;
	std::vector<double>* Oy = 0;
	std::vector<double>* Oz = 0;
	std::vector<double>* edep = 0;
	std::vector<int>* stopped = 0;
	double x=0, y=0, z=0;
	double px=0, py=0, pz=0;

	tree->SetBranchAddress("M_particleName", &particleName);
	tree->SetBranchAddress("M_volName"     , &volName);
	tree->SetBranchAddress("M_ovolName"    , &ovolName);
	tree->SetBranchAddress("M_oprocess"    , &oprocess);
	tree->SetBranchAddress("M_edep"        , &edep);
	tree->SetBranchAddress("M_stopped"     , &stopped);
	tree->SetBranchAddress("i_px"          , &px);
	tree->SetBranchAddress("i_py"          , &py);
	tree->SetBranchAddress("i_pz"          , &pz);
	//initial position
	tree->SetBranchAddress("i_x"    , &x);
	tree->SetBranchAddress("i_y"    , &y);
	tree->SetBranchAddress("i_z"    , &z);
	//stopped position
	tree->SetBranchAddress("M_x"    , &Ox);
	tree->SetBranchAddress("M_y"    , &Oy);
	tree->SetBranchAddress("M_z"    , &Oz);

	Double_t i_edep = 0;
	Double_t truth_e = 0;
	Double_t e1 = 0;
	Double_t e2 = 0;
	Double_t e3 = 0;
	TString channel = TString("");
	TString pid = TString("");

	TFile *outfile = new TFile(Form("transfer.%s", filename), "RECREATE");
	TLorentzVector mom;
	TVector3 pos, stopped_pos;
	TTree *oTree = new TTree("oTree", "oTree");
	oTree->Branch("channel", &channel);
	oTree->Branch("pid", &pid);
	oTree->Branch("mom", &mom);
	oTree->Branch("pos", &pos);
	oTree->Branch("stopped_pos", &stopped_pos);
	oTree->Branch("truth_e", &truth_e);
	oTree->Branch("e1", &e1);
	oTree->Branch("e2", &e2);
	oTree->Branch("e3", &e3);

	RooUnfoldResponse *SiR_TM = new RooUnfoldResponse(40, 0, 20000, "SiR_TM", "SiR_TM");
	RooUnfoldResponse *SiL_TM = new RooUnfoldResponse(40, 0, 20000, "SiL_TM", "SiL_TM");
	Double_t l1, l2, l3, r1, r2, r3;

	for (int i_entry = 0; i_entry < tree->GetEntries(); ++i_entry) {
		tree->GetEvent(i_entry);
		if (i_entry % 100000 == 0) {
			std::cout << i_entry << " / " << tree->GetEntries() << std::endl;
		}
		// Calculate the energy at the target
		if(px!=px || py!=py || pz!=pz ) continue; //check for NAN values
		if(!px && !py && !pz) continue;
		mom = TLorentzVector(px, py, pz, M);
		truth_e =  M - mom.Mag();
		truth_e = truth_e * 1000; // convert to keV
		pos = TVector3(x, y, z);
		l1=0; l2=0; l3=0;
		r1=0; r2=0; r3=0;
		channel = TString("");
		pid = TString("");
		for (unsigned iElement = 0; iElement < particleName->size(); ++iElement) {
			i_edep = edep->at(iElement) * 1000000; // convert to keV
			if(particleName->at(iElement) == "proton") {
				pid = TString("p");
				if(volName->at(iElement) == "SiR1") {
					r1+=i_edep;
				} else if(volName->at(iElement) == "SiR2") {
					r2+=i_edep;
				} else if(volName->at(iElement) == "SiR3") {
					r3+=i_edep;
				}
				if(volName->at(iElement) == "SiL1") {
					l1+=i_edep;
				} else if(volName->at(iElement) == "SiL3") {
					l2+=i_edep;
				} else if(volName->at(iElement) == "SiL2") {
					l3+=i_edep;
				}
			}
			if(particleName->at(iElement) == "deuteron") {
				pid = TString("d");
				if(volName->at(iElement) == "SiR1") {
					r1+=i_edep;
				} else if(volName->at(iElement) == "SiR2") {
					r2+=i_edep;
				} else if(volName->at(iElement) == "SiR3") {
					r3+=i_edep;
				}
				if(volName->at(iElement) == "SiL1") {
					l1+=i_edep;
				} else if(volName->at(iElement) == "SiL3") {
					l2+=i_edep;
				} else if(volName->at(iElement) == "SiL2") {
					l3+=i_edep;
				}
			}
			if(particleName->at(iElement) == "triton") {
				pid = TString("t");
				if(volName->at(iElement) == "SiR1") {
					r1+=i_edep;
				} else if(volName->at(iElement) == "SiR2") {
					r2+=i_edep;
				} else if(volName->at(iElement) == "SiR3") {
					r3+=i_edep;
				}
				if(volName->at(iElement) == "SiL1") {
					l1+=i_edep;
				} else if(volName->at(iElement) == "SiL3") {
					l2+=i_edep;
				} else if(volName->at(iElement) == "SiL2") {
					l3+=i_edep;
				}
			}
			if(particleName->at(iElement) == "mu-") {
				if(volName->at(iElement) == "Target" && stopped->at(iElement) ) {
					stopped_pos = TVector3(Ox->at(iElement), Oy->at(iElement), Oz->at(iElement) );
				}
			}
		}
//		if(r3==0) {
			if(r1 == 0 || r2 == 0) {
				SiR_TM->Miss(truth_e);
			} else {
				if(pid.Contains("p") ) {
					SiR_TM->Fill(r1+r2+r3, truth_e);
				} else {
					SiR_TM->Fake(r1+r2+r3);
				}
				e1 = r1 + smear("SiR1");
				e2 = r2 + smear("SiR2");
				e3 = r3 + smear("SiR3");
				channel = TString("SiR1");
			}
//		}
//		if(l3==0) {
			if(l1 == 0 || l2 == 0) {
				SiL_TM->Miss(truth_e);
			} else {
				if(pid.Contains("p") ) {
					SiL_TM->Fill(l1+l2+l3, truth_e);
				} else {
					SiL_TM->Fake(l1+l2+l3);
				}
				e1 = l1 + smear("SiR1");
				e2 = l2 + smear("SiR2");
				e3 = l3 + smear("SiR3");
				channel = TString("SiL1");
			}
//		}
		oTree->Fill();
	}
	SiR_TM->Write();
	SiL_TM->Write();
	outfile->Write();
}
