#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TCanvas.h"

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

void R15b_MuonBeamSim_AllPlots() {

  std::string filename = "output/raw_g4sim.root";
  std::string outfilename = "MuonBeamSim_AllPlots.root";
  
  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");

  double min_energy = 200;
  double max_energy = 2000;
  double energy_width = 10;
  int n_energy_bins = (max_energy - min_energy) / energy_width;
  TH1F* hSiL1All = new TH1F("hSiL1All", "", n_energy_bins,min_energy,max_energy);
  TH1F* hSiL1Stopped = new TH1F("hSiL1Stopped", "", n_energy_bins,min_energy,max_energy);
  TH1F* hSiL1NotStopped = new TH1F("hSiL1NotStopped", "", n_energy_bins,min_energy,max_energy);
  TH1F* hSiTAll = new TH1F("hSiTAll", "", n_energy_bins,min_energy,max_energy);
  TH1F* hSiTStopped = new TH1F("hSiTStopped", "", n_energy_bins,min_energy,max_energy);
  TH1F* hSiTNotStopped = new TH1F("hSiTNotStopped", "", n_energy_bins,min_energy,max_energy);

  const int n_channels = 16;
  TH1F* hSiL1Position = new TH1F("hSiL1Position", "", n_channels,0.5,n_channels+0.5);

  std::vector<std::string>* volName = 0;
  std::vector<std::string>* particleName = 0;
  std::vector<std::string>* oprocess = 0;
  std::vector<double>* edep = 0;
  std::vector<int>* volID = 0;
  std::vector<double>* time = 0;
  std::vector<int>* stopped = 0;

  TBranch* br_volName = tree->GetBranch("M_volName");
  TBranch* br_particleName = tree->GetBranch("M_particleName");
  TBranch* br_oprocess = tree->GetBranch("M_oprocess");
  TBranch* br_edep = tree->GetBranch("M_edep");
  TBranch* br_volID = tree->GetBranch("M_volID");
  TBranch* br_time = tree->GetBranch("M_t");
  TBranch* br_stopped = tree->GetBranch("M_stopped");

  br_volName->SetAddress(&volName);
  br_particleName->SetAddress(&particleName);
  br_oprocess->SetAddress(&oprocess);
  br_edep->SetAddress(&edep);
  br_volID->SetAddress(&volID);
  br_time->SetAddress(&time);
  br_stopped->SetAddress(&stopped);
  

  const int n_SiL1_channels = 1;
  double SiL1_energy_resolutions[n_SiL1_channels] = {30.0};
  TF1* SiL1_energy_res_fn[n_SiL1_channels];
  for (int i_SiL1_channel = 0; i_SiL1_channel < n_SiL1_channels; ++i_SiL1_channel) {
    SiL1_energy_res_fn[i_SiL1_channel] = new TF1("SiL1_energy_res_fn", "TMath::Gaus(x, 0, [0])", -250, 250);
    SiL1_energy_res_fn[i_SiL1_channel]->SetParameter(0, SiL1_energy_resolutions[i_SiL1_channel]);
  }
  const int n_SiT_channels = 1;
  double SiT_energy_resolutions[n_SiT_channels] = {41.0};
  TF1* SiT_energy_res_fn[n_SiT_channels];
  for (int i_SiT_channel = 0; i_SiT_channel < n_SiT_channels; ++i_SiT_channel) {
    SiT_energy_res_fn[i_SiT_channel] = new TF1("SiT_energy_res_fn", "TMath::Gaus(x, 0, [0])", -250, 250);
    SiT_energy_res_fn[i_SiT_channel]->SetParameter(0, SiT_energy_resolutions[i_SiT_channel]);
  }

  int n_entries = tree->GetEntries();
  for (int i_entry = 0; i_entry < n_entries; ++i_entry) {
    tree->GetEvent(i_entry);

    if (i_entry % 10000 == 0) {
      std::cout << i_entry << " / " << n_entries << std::endl;
    }

    bool SiL1_hit = false;
    bool SiL1_stopped = false;
    std::vector<double> SiL1_edeps_to_fill;
    double SiL1_edep_to_fill = 0;
    double SiL1_central_time = 0;
    bool SiT_hit = false;
    bool SiT_stopped = false;
    std::vector<double> SiT_edeps_to_fill;
    double SiT_edep_to_fill = 0;
    double SiT_central_time = 0;
    double tpi_length = 5000;
    double merge_time = 100; // peaks will merge together within this and we will take the largest of two peaks within 5000 ns
    for (int i_element = 0; i_element < volName->size();  ++i_element) {

      std::string i_volName = volName->at(i_element);
      std::string i_particleName = particleName->at(i_element);
      std::string i_oprocess = oprocess->at(i_element);
      double i_edep = edep->at(i_element)*1e6;
      int i_volID = volID->at(i_element);
      double i_time = time->at(i_element);
      int i_stopped = stopped->at(i_element);

      if (i_volName == "SiL1") {
	if (!SiL1_hit) { // if we haven't seen a SiL1 hit before
	  SiL1_central_time = i_time;
	}
	SiL1_hit = true;
	if (std::fabs(SiL1_central_time - i_time) < merge_time) { // add energy deposits that are close in time
	  SiL1_edep_to_fill += i_edep;
	}
	else if (std::fabs(SiL1_central_time - i_time) > tpi_length) { // add another peak
	  SiL1_edeps_to_fill.push_back(SiL1_edep_to_fill);
	  SiL1_central_time = i_time;
	  SiL1_edep_to_fill = i_edep;
	}
	if (i_particleName == "mu-" && i_stopped == 1) {
	  SiL1_stopped = true;
	}
	hSiL1Position->Fill(i_volID);
      }
      else if (i_volName == "SiT") {
	if (!SiT_hit) { // if we haven't seen a SiT hit before
	  SiT_central_time = i_time;
	}
	SiT_hit = true;
	if (std::fabs(SiT_central_time - i_time) < merge_time) { // add energy deposits that are close in time
	  SiT_edep_to_fill += i_edep;	  
	}
	else if (std::fabs(SiT_central_time - i_time) > tpi_length) { // add another peak
	  SiT_edeps_to_fill.push_back(SiT_edep_to_fill);
	  SiT_central_time = i_time;
	  SiT_edep_to_fill = i_edep;
	}
	if (i_particleName == "mu-" && i_stopped == 1) {
	  SiT_stopped = true;
	}
      }
    }
    SiL1_edeps_to_fill.push_back(SiL1_edep_to_fill); // put the last edep into the vector
    SiT_edeps_to_fill.push_back(SiT_edep_to_fill); // put the last edep into the vector
    // pick an energy
    //    SiL1_edep_to_fill = *(std::max_element(SiL1_edeps_to_fill.begin(), SiL1_edeps_to_fill.end()));
    for (std::vector<double>::const_iterator i_edep = SiL1_edeps_to_fill.begin(); i_edep != SiL1_edeps_to_fill.end(); ++i_edep) {
      SiL1_edep_to_fill = *i_edep;
      double delta_edep = SiL1_energy_res_fn[0]->GetRandom();
      SiL1_edep_to_fill += delta_edep;  
      if (SiL1_hit && SiL1_edep_to_fill>200) {
	hSiL1All->Fill(SiL1_edep_to_fill);
	if (SiL1_stopped) {
	  hSiL1Stopped->Fill(SiL1_edep_to_fill);
	}
	else {
	  hSiL1NotStopped->Fill(SiL1_edep_to_fill);
	}
      }
    }
    for (std::vector<double>::const_iterator i_edep = SiT_edeps_to_fill.begin(); i_edep != SiT_edeps_to_fill.end(); ++i_edep) {
      SiT_edep_to_fill = *i_edep;
      double delta_edep = SiT_energy_res_fn[0]->GetRandom();
      SiT_edep_to_fill += delta_edep;  
      if (SiT_hit && SiT_edep_to_fill>200) {
	hSiTAll->Fill(SiT_edep_to_fill);
	if (SiT_stopped) {
	  hSiTStopped->Fill(SiT_edep_to_fill);
	}
	else {
	  hSiTNotStopped->Fill(SiT_edep_to_fill);
	}
      }
    }
  }


  TFile* outfile = new TFile(outfilename.c_str(), "RECREATE");
  hSiL1All->Write();
  hSiL1Stopped->Write();
  hSiL1NotStopped->Write();
  hSiL1Position->Write();
  hSiTAll->Write();
  hSiTStopped->Write();
  hSiTNotStopped->Write();
  outfile->Write();
  outfile->Close();
}
