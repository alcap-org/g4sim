#include "TFile.h"
#include "TTree.h"
#include "TH2.h"
#include "TF1.h"

#include <iostream>
#include <string>
#include <sstream>

void R15b_MuonBeamSim_PreSiTValidation() {
  //  gStyle->SetOptFit(1);

  std::string filename = "output/R15b_100k_Geom-S14_Ekin-1-40MeV_EkinSpread-0-134MeV_new-beam-pos_pre-sit-muon-beam-sim.root";

  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");

  //  double min_energy = 200;
  double min_energy = 0;
  TH2F* hSiTvsSiL1_All = new TH2F("hSiTvsSiL1_All", "", 200,min_energy,2000, 200,min_energy,2000);
  hSiTvsSiL1_All->SetXTitle("SiT Edep");
  hSiTvsSiL1_All->SetYTitle("SiL1 Edep");

  TH2F* hSiTvsSiL1_SiL1Stops = (TH2F*) hSiTvsSiL1_All->Clone("hSiTvsSiL1_SiL1Stops");
  TH2F* hSiTvsSiL1_SiL1NotStops = (TH2F*) hSiTvsSiL1_All->Clone("hSiTvsSiL1_SiL1NotStops");
  TH2F* hSiTvsSiL1_noSiL3 = (TH2F*) hSiTvsSiL1_All->Clone("hSiTvsSiL1_noSiL3");
  TH2F* hSiTvsSiL1_wSiL3 = (TH2F*) hSiTvsSiL1_All->Clone("hSiTvsSiL1_wSiL3");

  TH1F* hTotalSiEDep = new TH1F("hTotalSiEDep", "", 300,min_energy,3000);
  hTotalSiEDep->SetXTitle("SiTEdep + SiL1 Edep + SiL3Edep [keV]");


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
    double sit_edep = -1;
    double target_edep = -1;
    bool target_stop = false;
    bool sil3_hit = false;
    for (int iElement = 0; iElement < volName->size();  ++iElement) {
      std::string i_particleName = particleName->at(iElement);
      std::string i_volName = volName->at(iElement);
      double i_edep = edep->at(iElement)*1e6;
      int i_volID = volID->at(iElement);
      int i_stopped = stopped->at(iElement);
      //      std::cout << "Particle #" << iElement+1 << ": " << i_particleName << " in " << i_volName << " depositing " << i_edep << " keV and stopping? " << i_stopped << std::endl;

      if (i_volName == "Target") { // include SiL1-1-S and SiL1-16-S for the time begin
	if (i_particleName=="mu-") {
	  target_edep = i_edep;
	  if (i_stopped == 1) {
	    target_stop = true;
	  }
	  else if (i_stopped == 0) {
	    target_stop = false;
	  }	
	}
      }

      if (i_volName == "SiT" && i_particleName=="mu-") {
	sit_edep = i_edep;
      }

      if (i_volName== "SiL3" && i_particleName=="mu-" && i_edep>200) {
	sil3_hit = true;
	sil3_edep = i_edep;
      }
    }
    //    std::cout << "Target Edep = " << target_edep << ", Sit Edep = " << sit_edep << std::endl;
    if (target_edep > 0 && sit_edep > 0) {
      hSiTvsSiL1_All->Fill(sit_edep, target_edep);
      if (target_stop == true) {
 	hSiTvsSiL1_SiL1Stops->Fill(sit_edep, target_edep);
      }
      else {
	hSiTvsSiL1_SiL1NotStops->Fill(sit_edep, target_edep);
      }

      if (sil3_hit == true) {
 	hSiTvsSiL1_wSiL3->Fill(sit_edep, target_edep);
	hTotalSiEDep->Fill(sit_edep+target_edep+sil3_edep);
      }
      else {
 	hSiTvsSiL1_noSiL3->Fill(sit_edep, target_edep);
      }
    }
  }

  hSiTvsSiL1_All->Draw("COLZ");

  hSiTvsSiL1_SiL1Stops->Draw("COLZ");

  hSiTvsSiL1_SiL1NotStops->Draw("COLZ");

  hSiTvsSiL1_wSiL3->Draw("COLZ");

  hSiTvsSiL1_noSiL3->Draw("COLZ");

  hTotalSiEDep->Draw();
}
