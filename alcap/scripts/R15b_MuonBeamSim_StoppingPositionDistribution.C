#include "TFile.h"
#include "TTree.h"
#include "TH3F.h"
#include "THnSparse.h"
#include "TBranch.h"

#include <iostream>

void R15b_MuonBeamSim_StoppingPositionDistribution() {

  //  std::string filename = "output/R15b_1M_Geom-S14_Ekin-1-54MeV_EkinSpread-0-27MeV_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-S14_Ekin-1-54MeV_EkinSpread-0-27MeV_SiL1-Ephi--40deg_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-S14_Ekin-1-54MeV_EkinSpread-0-27MeV_SiL1-Ephi--35deg_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-S14_Ekin-1-54MeV_EkinSpread-0-27MeV_SiL1-Ephi--30deg_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-P5_Ekin-1-54MeV_EkinSpread-0-27MeV_SiL1-Ephi--45deg_SiT-Ephi-90deg_BeamDivergence-R13_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-P5_Ekin-1-34MeV_EkinSpread-0-27MeV_SiL1-Ephi--45deg_SiT-Ephi-90deg_BeamDivergence-R13_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-P5_Ekin-1-04MeV_EkinSpread-0-27MeV_SiL1-Ephi--45deg_SiT-Ephi-90deg_BeamDivergence-R13_new-beam-pos_pre-sit-muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-P1_muon-beam-sim_Ekin-3-94MeV_EkinSpread-0-26MeV.root";
  //  std::string filename = "output/R15b_1M_Geom-P5_muon-beam-sim_Ekin-0-84MeV_EkinSpread-0-18MeV.root";
  //  std::string filename = "output/R15b_500k_Geom-P4_muon-beam-sim.root";
  //  std::string filename = "output/R15b_200k_Geom-S14_muon-beam-sim_SF1-05.root";
  //  std::string filename = "output/raw_g4sim.root";
  //  std::string filename = "output/R15b_200k_Geom-P1_muon-beam-sim_SF1-12-nominal.root";
  //  std::string filename = "output/R15b_200k_Geom-P1_muon-beam-sim_SF1-12-lowerE.root";
  //  std::string filename = "output/R15b_200k_Geom-P1_muon-beam-sim_SF1-12-higherE.root";
  
  //  std::string filename = "output/R15b_100k_Geom-S17_muon-beam-sim.root"; // volName = SiL1
  //  std::string outfilename ="data/R15b_output_Geom-S17_muon-stop-positions.root";
  //  std::string filename = "output/R15b_100k_Geom-S14_muon-beam-sim.root";
  //  std::string outfilename ="data/R15b_output_Geom-S14_muon-stop-positions.root";
  //  std::string filename = "output/R15b_1M_Geom-P5_muon-beam-sim.root";
  //  std::string outfilename ="data/R15b_output_Geom-P5_muon-stop-positions_new.root";
  //  std::string filename = "output/R15b_1M_Geom-P1_muon-beam-sim.root";
  //  std::string outfilename ="data/R15b_output_Geom-P5_check.root";
  //  std::string outfilename ="data/R15b_output_Geom-P4_muon-stop-positions.root";
  //  std::string outfilename ="data/R15b_output_Geom-P4_muon-stop-positions.root";

  //  std::string filename = "output/R15b_2M_Geom-S14_muon-beam-sim_SF1-02-midHighE.root";
  //  std::string outfilename = "data/R15b_Geom-P5-midHighE_muon-stop-positions.root";
  //  std::string filename = "output/R15b_2M_Geom-S14_muon-beam-sim_SF1-02-midLowE.root";
  //  std::string outfilename = "data/R15b_Geom-P5-midLowE_muon-stop-positions.root";

  //  std::string filename = "output/R15b_2M_Geom-P5_muon-beam-sim_SF1-02_new.root";
  //  std::string outfilename = "data/R15b_Geom-P5_muon-stop-positions_new.root";
  //  std::string filename = "output/R15b_2M_Geom-P5_muon-beam-sim_SF1-02-lowerE_new.root";
  //  std::string outfilename = "data/R15b_Geom-P5-lowerE_muon-stop-positions_new.root";
  // std::string filename = "output/R15b_2M_Geom-P5_muon-beam-sim_SF1-02-higherE_new.root";
  // std::string outfilename = "data/R15b_Geom-P5-higherE_muon-stop-positions_new.root";
  // std::string filename = "output/R15b_2M_Geom-P5_muon-beam-sim_SF1-02-midLowE_new.root";
  // std::string outfilename = "data/R15b_Geom-P5-midLowE_muon-stop-positions_new.root";
  // std::string filename = "output/R15b_2M_Geom-P5_muon-beam-sim_SF1-02-midHighE_new.root";
  // std::string outfilename = "data/R15b_Geom-P5-midHighE_muon-stop-positions_new.root";
  // std::string filename = "output/R15b_2M_Geom-P5_muon-beam-sim_SF1-02-lowestE_new.root";
  // std::string outfilename = "data/R15b_Geom-P5-lowestE_muon-stop-positions_new.root";
  std::string filename = "output/R15b_2M_Geom-P5_muon-beam-sim_SF1-02-highestE_new.root";
  std::string outfilename = "data/R15b_Geom-P5-highestE_muon-stop-positions_new.root";




  //  std::string tgtVolName = "Target";
  std::string tgtVolName = "SiL1";
  //  std::string tgtVolName = "SiL3";
  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");

  TH3F* hLocalStoppingPositions = 0;
  double bin_width = 0.0001; // mm
  const int n_dimensions = 3;
  double x_low[n_dimensions] = {0};
  double x_high[n_dimensions] = {0};
  if (tgtVolName=="SiL3") {
    hLocalStoppingPositions = new TH3F("hLocalStoppingPositions", "Positions of Stopped Muons in Target (local coords)", 10,-5,5, 10,-5,5, 120,-1000,1000); // Geom-P1
    x_low[0] = -30; x_low[1] = -30; x_low[2] = -5;
    x_high[0] = 30; x_high[1] = 30; x_high[2] = 5;
  }
  else if (tgtVolName=="SiL1") {
    hLocalStoppingPositions = new TH3F("hLocalStoppingPositions", "Positions of Stopped Muons in Target (local coords)", 10,-5,5, 10,-5,5, 100,-50,50); // Geom-P5 and Geom-P4
    x_low[0] = -30; x_low[1] = -30; x_low[2] = -30;
    x_high[0] = 30; x_high[1] = 30; x_high[2] = 30;
  }
  else {
    std::cout << "Error: Unsupported target volume " << tgtVolName << std::endl;
  }
  hLocalStoppingPositions->SetXTitle("Local X [cm]");
  hLocalStoppingPositions->SetYTitle("Local Y [cm]");
  hLocalStoppingPositions->SetZTitle("Local Z [#mum]");
  std::string cutcmd = "M_particleName==\"mu-\" && M_volName==\"" + tgtVolName + "\" && M_stopped==1";
  tree->Draw("M_local_Oz*10000:M_local_Oy:M_local_Ox>>hLocalStoppingPositions", cutcmd.c_str(), "");

  int n_bins_one_axis = (x_high[0] - x_low[0]) / bin_width;
  int n_bins[n_dimensions] = {n_bins_one_axis, n_bins_one_axis, n_bins_one_axis};
  std::cout << "AE: n_bins_one_axis = " << n_bins_one_axis << std::endl;

  THnSparseF* hGlobalStoppingPositions = new THnSparseF("hGlobalStoppingPositions", "Positions of Stopped Muons in Target (global coords)", n_dimensions, n_bins, x_low, x_high);
  hGlobalStoppingPositions->GetAxis(0)->SetTitle("Global X [mm]");
  hGlobalStoppingPositions->GetAxis(1)->SetTitle("Global Y [mm]");
  hGlobalStoppingPositions->GetAxis(2)->SetTitle("Global Z [mm]");

  std::vector<std::string>* particleName = 0;
  std::vector<std::string>* volName = 0;
  std::vector<int>* stopped = 0;
  std::vector<double>* Ox = 0;
  std::vector<double>* Oy = 0;
  std::vector<double>* Oz = 0;
  
  TBranch* br_particleName = tree->GetBranch("M_particleName");
  TBranch* br_volName = tree->GetBranch("M_volName");
  TBranch* br_stopped = tree->GetBranch("M_stopped");
  TBranch* br_Ox = tree->GetBranch("M_Ox");
  TBranch* br_Oy = tree->GetBranch("M_Oy");
  TBranch* br_Oz = tree->GetBranch("M_Oz");
  
  br_particleName->SetAddress(&particleName);
  br_volName->SetAddress(&volName);
  br_stopped->SetAddress(&stopped);
  br_Ox->SetAddress(&Ox);
  br_Oy->SetAddress(&Oy);
  br_Oz->SetAddress(&Oz);

  int n_entries = tree->GetEntries();
  for (int iEntry = 0; iEntry < n_entries; ++iEntry) {
      tree->GetEvent(iEntry);
      
      if (iEntry % 10000 == 0) {
	std::cout << iEntry << " / " << n_entries << std::endl;
      }

      for (unsigned int iElement = 0; iElement < particleName->size();  ++iElement) {

	std::string i_particleName = particleName->at(iElement);
	std::string i_volName = volName->at(iElement);

	if (i_particleName == "mu-" && i_volName == tgtVolName && stopped->at(iElement) == 1) {
	  double coord[n_dimensions] = {Ox->at(iElement)*10, Oy->at(iElement)*10, Oz->at(iElement)*10}; // convert to mm
	  hGlobalStoppingPositions->Fill(coord);
	}
      }
  }

  //    tree->Draw("M_Oz:M_Oy:M_Ox>>hGlobalStoppingPositions", "M_particleName==\"mu-\" && M_volName==\"Target\" && M_stopped==1", "", 10000);
  std::cout << "NStops = " << hLocalStoppingPositions->GetEntries() << std::endl;
  TFile* out_file = new TFile(outfilename.c_str(), "RECREATE");
  hLocalStoppingPositions->Write();
  hGlobalStoppingPositions->Write();
  out_file->Close();
}
