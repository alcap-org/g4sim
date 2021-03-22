#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TF1.h"

#include <iostream>
#include <string>
#include <sstream>

void R15b_MuonBeamSim_StopRateValidation() {
  //  gStyle->SetOptFit(1);

  //  std::string filename = "output/R15b_100k_Geom-P5_Mom-15-9MeV_MomSpread-0-203MeV_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-P5_Ekin-1-19MeV_EkinSpread-0-157MeV_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-P5_Ekin-1-19MeV_EkinSpread-0-157MeV_ThetaSpread-1-71887deg_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-P5_Ekin-1-19MeV_EkinSpread-0-157MeV_new-beam-pos_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-P5_Ekin-1-3MeV_EkinSpread-0-157MeV_new-beam-pos_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-P5_Ekin-1-3MeV_EkinSpread-0MeV_new-beam-pos_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-P5_Ekin-1-0MeV_EkinSpread-0MeV_new-beam-pos_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-P5_Ekin-1-39MeV_EkinSpread-0-122MeV_new-beam-pos_muon-beam-sim.root";
  //  std::string filename = "output/R15b_200k_Geom-P5_Ekin-1-39MeV_EkinSpread-0-122MeV_new-beam-pos_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-P5_Ekin-1-36MeV_EkinSpread-0-120MeV_new-beam-pos_muon-beam-sim.root";
  //  //  std::string filename = "output/R15b_100k_Geom-S14_Ekin-1-19MeV_EkinSpread-0-157MeV_new-beam-pos_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-S14_Ekin-1-39MeV_EkinSpread-0-122MeV_new-beam-pos_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-S14_Ekin-1-36MeV_EkinSpread-0-120MeV_new-beam-pos_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-S14_Ekin-1-40MeV_EkinSpread-0-134MeV_new-beam-pos_muon-beam-sim.root";
  std::string filename = "output/raw_g4sim.root";
  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");

  //  double min_energy = 200;
  double min_energy = 400;
  //  double max_energy = 2000;
  double max_energy = 6000;
  TH1F* hAll = new TH1F("hAllParticles", "", 160,min_energy,max_energy);
  TH1F* hNotStops = new TH1F("hNotStopMuons", "", 160,min_energy,max_energy);
  TH1F* hStops = new TH1F("hStopMuons", "", 160,min_energy,max_energy);
  TH1F* hNoSiL3 = new TH1F("hNoSiL3", "", 160,min_energy,max_energy);

  std::vector<std::string>* particleName = 0;
  std::vector<std::string>* volName = 0;
  std::vector<double>* edep = 0;
  std::vector<int>* volID = 0;
  std::vector<int>* stopped = 0;

  TBranch* br_particleName = tree->GetBranch("M_particleName");
  TBranch* br_volName = tree->GetBranch("M_volName");
  TBranch* br_edep = tree->GetBranch("M_edep");
  TBranch* br_volID = tree->GetBranch("M_volID");
  TBranch* br_stopped = tree->GetBranch("M_stopped");

  br_particleName->SetAddress(&particleName);
  br_volName->SetAddress(&volName);
  br_edep->SetAddress(&edep);
  br_volID->SetAddress(&volID);
  br_stopped->SetAddress(&stopped);

  int n_entries = tree->GetEntries();

  for (int iEntry = 0; iEntry < n_entries; ++iEntry) {
    tree->GetEvent(iEntry);

    if (iEntry % 10000 == 0) {
      std::cout << iEntry << " / " << n_entries << std::endl;
    }

    double sil3_edep = -1;
    double target_edep = -1;
    for (int iElement = 0; iElement < volName->size();  ++iElement) {
      std::string i_particleName = particleName->at(iElement);
      std::string i_volName = volName->at(iElement);
      double i_edep = edep->at(iElement)*1e6;
      int i_volID = volID->at(iElement);
      int i_stopped = stopped->at(iElement);
      //      std::cout << "Particle #" << iElement+1 << ": " << i_particleName << " in " << i_volName << " depositing " << i_edep << " keV and stopping? " << i_stopped << std::endl;

      if (i_volName == "Target" && i_volID>0 && i_volID<15) { // don't want to include SiL1-1-S and SiL1-16-S
	hAll->Fill(i_edep);

	if (i_particleName=="mu-") {
	  target_edep = i_edep;
	  if (i_stopped == 1) {
	    hStops->Fill(i_edep);
	  }
	  else if (i_stopped == 0) {
	    hNotStops->Fill(i_edep);
	  }	
	}
      }

      if (i_volName == "SiL3" && i_particleName=="mu-") {
	sil3_edep = i_edep;
      }
    }
    //    std::cout << "Target Edep = " << target_edep << ", SiL3 Edep = " << sil3_edep << std::endl;
    if (target_edep > 0 && sil3_edep <0) {
      hNoSiL3->Fill(target_edep);
    }
  }

  std::stringstream title;
  title << "Energy Deposited in SiL1 (MC Truth)";
  hAll->SetTitle(title.str().c_str());
  hAll->SetXTitle("Energy Deposited [keV]");
  hAll->SetLineColor(kBlack);
  //  hAll->Rebin(5);
  hAll->Draw("HIST E");

  hStops->SetTitle(title.str().c_str());
  hStops->SetXTitle("Energy Deposited [keV]");
  hStops->SetLineColor(kRed);
  //  hStops->Rebin(5);
  hStops->Fit("gaus", "Q0");
  hStops->GetFunction("gaus")->SetLineColor(kRed);
  hStops->GetFunction("gaus")->Draw("LSAME");
  hStops->Draw("HIST E SAMES");

  hNotStops->SetTitle(title.str().c_str());
  hNotStops->SetXTitle("Energy Deposited [keV]");
  hNotStops->SetLineColor(kBlue);
  hNotStops->Fit("gaus", "Q0", "", 600, 800);
  hNotStops->GetFunction("gaus")->SetLineColor(kBlue);
  hNotStops->GetFunction("gaus")->Draw("LSAME");
  //  hNotStops->Rebin(5);
  hNotStops->Draw("HIST E SAMES");

  hNoSiL3->SetLineColor(kMagenta);
  hNoSiL3->Draw("HIST E SAMES");
}
