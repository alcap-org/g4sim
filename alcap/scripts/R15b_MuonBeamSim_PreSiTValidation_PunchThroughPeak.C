#include "TFile.h"
#include "TTree.h"
#include "TH2.h"
#include "TF1.h"

#include <iostream>
#include <string>
#include <sstream>

#include "Andy_Si16b_PunchThroughPeak_RooFit.h"

void R15b_MuonBeamSim_PreSiTValidation_PunchThroughPeak() {
  //  gStyle->SetOptFit(1);

  // Energy resolutions
  const int n_SiT_channels = 4;
  //  double energy_res_sigma[n_SiT_channels] = {23.7, 22.7, 23.7, 24.1};
  //  double energy_res_sigma[n_SiT_channels] = {29.9, 34.1, 31.0, 43.6};
  //  double energy_res_sigma[n_SiT_channels] = {38.1, 40.9, 39.0, 49.8};
  double energy_res_sigma[n_SiT_channels] = {20, 20, 20, 20};
  TF1* energy_res_fn = new TF1("energy_res_fn", "TMath::Gaus(x, 0, [0])", -500,500);

  //  std::string filename = "output/R15b_100k_Geom-S14_Ekin-1-40MeV_EkinSpread-0-134MeV_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/raw_g4sim.root";
  //  std::string filename = "output/R15b_100k_Geom-S14_Ekin-1-50MeV_EkinSpread-0-3MeV_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-S14_Ekin-1-50MeV_EkinSpread-0-3MeV_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-S14_Ekin-1-35MeV_EkinSpread-0-27MeV_SiL1-Ephi--45deg_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-S14_Ekin-1-35MeV_EkinSpread-0-27MeV_SiL1-Ephi--45deg_SiT-Ephi-85deg_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-S14_Ekin-1-35MeV_EkinSpread-0-27MeV_SiL1-Ephi--45deg_SiT-Ephi-80deg_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-S14_Ekin-1-54MeV_EkinSpread-0-27MeV_SiL1-Ephi--45deg_SiT-Ephi-90deg_ThetaSpread-10deg_PhiSpread-10deg_new-beam-pos_pre-sit-muon-beam-sim.root";
  std::string filename = "output/R15b_1M_Geom-S14_Ekin-1-54MeV_EkinSpread-0-27MeV_SiL1-Ephi--45deg_SiT-Ephi-90deg_BeamDivergence-R13_new-beam-pos_pre-sit-muon-beam-sim.root";

  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");

  //  double min_energy = 200;
  double min_energy = 0;
  TH1F* hSiT_All = new TH1F("hSiT_All", "", 200,min_energy,2000);
  hSiT_All->SetXTitle("SiT Edep [keV]");
  hSiT_All->SetLineColor(kBlack);
  hSiT_All->SetLineWidth(2);

  TH1F* hSiT_mu = (TH1F*) hSiT_All->Clone("hSiT_mu");
  hSiT_mu->SetLineColor(kRed);

  TH1F* hSiT_mu_stopped = (TH1F*) hSiT_All->Clone("hSiT_mu_stopped");
  hSiT_mu_stopped->SetLineColor(kBlue);

  TH1F* hSiT_mu_not_stopped = (TH1F*) hSiT_All->Clone("hSiT_mu_not_stopped");
  hSiT_mu_not_stopped->SetLineColor(kMagenta);

  TH1F* hSmears = new TH1F("hSmears", "", 200,-1000,1000);

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

    for (int iElement = 0; iElement < volName->size();  ++iElement) {
      std::string i_particleName = particleName->at(iElement);
      std::string i_volName = volName->at(iElement);
      double i_edep = edep->at(iElement)*1e6;
      int i_volID = volID->at(iElement);
      int i_stopped = stopped->at(iElement);
      //      std::cout << "Particle #" << iElement+1 << ": " << i_particleName << " in " << i_volName << " depositing " << i_edep << " keV and stopping? " << i_stopped << std::endl;

      if (i_volName == "SiT" && i_edep>0) {
	energy_res_fn->SetParameter(0, energy_res_sigma[i_volID]);
	double smear = energy_res_fn->GetRandom();
	hSmears->Fill(smear);
	i_edep += smear;
	
	hSiT_All->Fill(i_edep);
	if (i_particleName == "mu-") {
	  hSiT_mu->Fill(i_edep);
	  if (i_stopped == 1) {
	    hSiT_mu_stopped->Fill(i_edep);
	  }
	  else if (i_stopped == 0) {
	    hSiT_mu_not_stopped->Fill(i_edep);
	  }
	}
      }
    }
  }

  hSiT_All->Draw("");
  hSiT_mu->Draw("SAME");
  hSiT_mu_stopped->Draw("SAME");
  hSiT_mu_not_stopped->Draw("SAME");

  /*
  double spectrum_max = hSiT_mu->GetMaximum();
  double spectrum_max_pos = hSiT_mu->GetBinCenter(hSiT_mu->GetMaximumBin());
  TF1* gaussian = new TF1("gaussian", "[0]*TMath::Gaus(x,[1],[2])");
  gaussian->SetParameters(spectrum_max, spectrum_max_pos, 100);
  hSiT_mu->Fit(gaussian, "Q", "", spectrum_max_pos-100, spectrum_max_pos+100);
  std::cout << "Punch Through Peak is at " << gaussian->GetParameter(1) << " keV" << std::endl;
  std::cout << "Punch Through Peak width is " << gaussian->GetParameter(2) << " keV" << std::endl;
  */
  PunchThroughPeak_RooFit(hSiT_mu, "MC");
  /*  TFile* output = new TFile("Andy_Si16_MC_SiTMuons.root", "RECREATE");
  hSiT_All->Write();
  hSiT_mu->Write();
  hSiT_mu_stopped->Write();
  hSiT_mu_not_stopped->Write();
  output->Write();
  output->Close();
  */
}
