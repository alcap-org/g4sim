#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TF1.h"

#include <iostream>
#include <string>
#include <sstream>

void R15b_MuonBeamSim_EDepValidation() {
  //  gStyle->SetOptFit(1);

  //  std::string filename = "output/R15b_100k_Geom-P5_Mom-14-79MeV_MomSpread-0-188MeV_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-P5_Mom-10-32MeV_MomSpread-0-131MeV_muon-beam-sim.root";
  std::string filename = "output/R15b_100k_Geom-P5_Mom-14-79MeV_MomSpread-0-367MeV_muon-beam-sim.root";
  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");

  TH1F* hist = new TH1F( "hist", "", 1000,0,10000);

  std::vector<std::string>* volName = 0;
  std::vector<double>* edep = 0;
  std::vector<int>* volID = 0;

  TBranch* br_volName = tree->GetBranch("M_volName");
  TBranch* br_edep = tree->GetBranch("M_edep");
  TBranch* br_volID = tree->GetBranch("M_volID");

  br_volName->SetAddress(&volName);
  br_edep->SetAddress(&edep);
  br_volID->SetAddress(&volID);

  int n_entries = tree->GetEntries();
  const int n_si_channels = 16;
  // from SiL1 energy calibration (Am241 fit sigma, NB SiL1-1-S and SiL1-16S are miscalibrated)
  double energy_resolutions[n_si_channels] = {1031.69, 84.4072, 86.4528, 69.5987, 65.1556, 57.732, 69.3947, 50.0809, 64.0503, 50.8744, 67.9137, 62.3101, 76.8118, 55.7161, 59.978, 1037.23};
  TF1* energy_res_fn[n_si_channels];
  for (int i_si_channel = 0; i_si_channel < n_si_channels; ++i_si_channel) {
    energy_res_fn[i_si_channel] = new TF1("energy_res_fn", "TMath::Gaus(x, 0, [0])", -250, 250);
    energy_res_fn[i_si_channel]->SetParameter(0, energy_resolutions[i_si_channel]);
  }

  for (int iEntry = 0; iEntry < n_entries; ++iEntry) {
    tree->GetEvent(iEntry);

    if (iEntry % 10000 == 0) {
      std::cout << iEntry << " / " << n_entries << std::endl;
    }

    for (int iElement = 0; iElement < volName->size();  ++iElement) {

      std::string i_volName = volName->at(iElement);
      double i_edep = edep->at(iElement)*1e6;
      int i_volID = volID->at(iElement);
      // smear the energy
      double delta_edep = energy_res_fn[i_volID]->GetRandom();

      if (i_volName == "Target" && i_volID>0 && i_volID<15) { // don't want to include SiL1-1-S and SiL1-16-S
	hist->Fill(i_edep+delta_edep);
      }
    }
  }

  std::stringstream title;
  title << "Energy Deposited in SiL1 (MC Truth with #sigma_{E} from SiL1 calibrations)";
  hist->SetTitle(title.str().c_str());
  hist->SetXTitle("Energy Deposited [keV]");
  hist->SetLineColor(kRed);
  hist->Rebin(5);
  hist->Draw("HIST SAMES");
}
