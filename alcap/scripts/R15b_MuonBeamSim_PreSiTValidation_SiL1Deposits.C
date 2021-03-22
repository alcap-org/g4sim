#include "TFile.h"
#include "TTree.h"
#include "TH2.h"
#include "TF1.h"

#include <iostream>
#include <string>
#include <sstream>

#include "Andy_Si16b_PunchThroughPeak_RooFit.h"

void R15b_MuonBeamSim_PreSiTValidation_SiL1Deposits() {
  //  gStyle->SetOptFit(1);

  //  std::string filename = "output/R15b_100k_Geom-S14_Ekin-1-40MeV_EkinSpread-0-134MeV_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/raw_g4sim.root";
  //  std::string filename = "output/R15b_100k_Geom-S14_Ekin-1-50MeV_EkinSpread-0-3MeV_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-S14_Ekin-1-50MeV_EkinSpread-0-3MeV_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-S14_Ekin-1-54MeV_EkinSpread-0-27MeV_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-S14_Ekin-1-54MeV_EkinSpread-0-27MeV_SiL1-Ephi--40deg_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-S14_Ekin-1-54MeV_EkinSpread-0-27MeV_SiL1-Ephi--35deg_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-S14_Ekin-1-54MeV_EkinSpread-0-27MeV_SiL1-Ephi--30deg_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-S14_Ekin-1-54MeV_EkinSpread-0-27MeV_SiL1-Ephi--41deg_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-S14_Ekin-1-54MeV_EkinSpread-0-27MeV_SiL1-Ephi--39deg_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-S14_Ekin-1-53MeV_EkinSpread-0-27MeV_SiL1-Ephi--40deg_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-S14_Ekin-1-50MeV_EkinSpread-0-27MeV_SiL1-Ephi--40deg_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-S14_Ekin-1-45MeV_EkinSpread-0-27MeV_SiL1-Ephi--45deg_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-S14_Ekin-1-35MeV_EkinSpread-0-27MeV_SiL1-Ephi--45deg_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-S14_Ekin-1-54MeV_EkinSpread-0-35MeV_SiL1-Ephi--45deg_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-S14_Ekin-1-35MeV_EkinSpread-0-27MeV_SiL1-Ephi--45deg_SiT-Ephi-80deg_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-S14_Ekin-1-54MeV_EkinSpread-0-27MeV_SiL1-Ephi--45deg_SiT-Ephi-90deg_ThetaSpread-10deg_PhiSpread-10deg_new-beam-pos_pre-sit-muon-beam-sim.root";
  std::string filename = "output/R15b_1M_Geom-S14_Ekin-1-54MeV_EkinSpread-0-27MeV_SiL1-Ephi--45deg_SiT-Ephi-90deg_BeamDivergence-R13_new-beam-pos_pre-sit-muon-beam-sim.root";
  
  const int n_si_channels = 16;
  // from SiL1 energy calibration (Am241 fit sigma, NB SiL1-1-S and SiL1-16S are miscalibrated)
  //  double energy_resolutions[n_si_channels] = {1031.69, 84.4072, 86.4528, 69.5987, 65.1556, 57.732, 69.3947, 50.0809, 64.0503, 50.8744, 67.9137, 62.3101, 76.8118, 55.7161, 59.978, 1037.23};
  double energy_resolutions[n_si_channels] = {80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80};
  TF1* energy_res_fn[n_si_channels];
  for (int i_si_channel = 0; i_si_channel < n_si_channels; ++i_si_channel) {
    energy_res_fn[i_si_channel] = new TF1("energy_res_fn", "TMath::Gaus(x, 0, [0])", -500, 500);
    energy_res_fn[i_si_channel]->SetParameter(0, energy_resolutions[i_si_channel]);
  }


  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");

  //  double min_energy = 200;
  double min_energy = 0;
  double max_energy = 10000;
  double energy_width = 1;
  int n_energy_bins = (max_energy - min_energy) / energy_width;
  TH1F* hSiL1_All = new TH1F("hSiL1_All", "", n_energy_bins,min_energy,max_energy);
  hSiL1_All->SetXTitle("SiL1 Edep [keV]");
  hSiL1_All->SetLineColor(kBlack);
  hSiL1_All->SetLineWidth(2);

  TH1F* hSiL1_mu = (TH1F*) hSiL1_All->Clone("hSiL1_mu");
  hSiL1_mu->SetLineColor(kRed);

  TH1F* hSiL1_mu_stopped = (TH1F*) hSiL1_All->Clone("hSiL1_mu_stopped");
  hSiL1_mu_stopped->SetLineColor(kBlue);

  TH1F* hSiL1_mu_not_stopped = (TH1F*) hSiL1_All->Clone("hSiL1_mu_not_stopped");
  hSiL1_mu_not_stopped->SetLineColor(kMagenta);

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

      if (i_volName == "Target"){// && i_edep>0 && i_volID>0 && i_volID<15) {
	i_edep += energy_res_fn[i_volID]->GetRandom();
	
	hSiL1_All->Fill(i_edep);
	if (i_particleName == "mu-") {
	  hSiL1_mu->Fill(i_edep);
	  if (i_stopped == 1) {
	    hSiL1_mu_stopped->Fill(i_edep);
	  }
	  else if (i_stopped == 0) {
	    hSiL1_mu_not_stopped->Fill(i_edep);
	  }
	}
      }
    }
  }

  //  hSiL1_All->Draw("");
  hSiL1_mu->Draw("");
  hSiL1_mu_stopped->Draw("SAME");
  hSiL1_mu_not_stopped->Draw("SAME");

  /*  double spectrum_max = hSiL1_mu->GetMaximum();
  double spectrum_max_pos = hSiL1_mu->GetBinCenter(hSiL1_mu->GetMaximumBin());
  TF1* gaussian = new TF1("gaussian", "[0]*TMath::Gaus(x,[1],[2])");
  gaussian->SetParameters(spectrum_max, spectrum_max_pos, 100);
  hSiL1_mu->Fit(gaussian, "Q", "", spectrum_max_pos-300, spectrum_max_pos+300);
  std::cout << "Punch Through Peak is at " << gaussian->GetParameter(1) << " keV" << std::endl;
  std::cout << "Punch Through Peak width is " << gaussian->GetParameter(2) << " keV" << std::endl;
  */
  //  PunchThroughPeak_RooFit(hSiL1_mu, "MC");

  TFile* output = new TFile("Andy_Si16_MC_Ekin-1-54MeV_EkinSpread-0-27MeV_SiL1-Ephi--45deg_SiT-Ephi-90deg_BeamDivergence-R13_SiL1Muons.root", "RECREATE");
  hSiL1_All->Write();
  hSiL1_mu->Write();
  hSiL1_mu_stopped->Write();
  hSiL1_mu_not_stopped->Write();
  output->Write();
  output->Close();
}
