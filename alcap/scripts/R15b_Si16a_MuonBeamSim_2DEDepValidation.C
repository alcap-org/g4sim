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

void R15b_Si16a_MuonBeamSim_2DEDepValidation() {
  //  gStyle->SetOptFit(1);

  std::string filename = "output/raw_g4sim.root";
  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");

  TH2F* hTwoLayer = new TH2F("hTwoLayer", "", 500,0,5000, 200,0,2000);

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
  
  int n_entries = tree->GetEntries();
  const int n_si_channels = 2; // SiT, SiL1
  double energy_resolutions[n_si_channels] = {41.0, 30.1}; // using the mean of these {37.2, 39.9, 38.1, 48.6};
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

    bool SiT_hit = false;
    std::vector<double> SiT_edeps_to_fill;
    std::vector<double> SiT_times_to_fill;
    double SiT_edep_to_fill = 0;
    double SiT_central_time = 0;

    bool SiL1_hit = false;
    std::vector<double> SiL1_edeps_to_fill;
    std::vector<double> SiL1_times_to_fill;
    double SiL1_edep_to_fill = 0;
    double SiL1_central_time = 0;

    double tpi_length = 5000;
    double merge_time = 100; // peaks will merge together within this and we will take the largest of two peaks within 5000 ns
    double layer_coinc_time = 200;
    for (int iElement = 0; iElement < volName->size();  ++iElement) {

      std::string i_volName = volName->at(iElement);
      std::string i_particleName = particleName->at(iElement);
      std::string i_oprocess = oprocess->at(iElement);
      double i_edep = edep->at(iElement)*1e6;
      int i_volID = volID->at(iElement);
      double i_time = time->at(iElement);
      int i_stopped = stopped->at(iElement);

      if (i_volName == "SiT") {
	if (!SiT_hit) { // if we haven't seen a SiT hit before
	  SiT_central_time = i_time;
	}
	SiT_hit = true;
	if (std::fabs(SiT_central_time - i_time) < merge_time) { // add energy deposits that are close in time
	  SiT_edep_to_fill += i_edep;	  
	}
	else if (std::fabs(SiT_central_time - i_time) > tpi_length) { // add another peak
	  SiT_edeps_to_fill.push_back(SiT_edep_to_fill);
	  SiT_times_to_fill.push_back(SiT_central_time);
	  SiT_central_time = i_time;
	  SiT_edep_to_fill = i_edep;
	}
      }

      if (i_volName == "Target") {
	if (!SiL1_hit) { // if we haven't seen a SiL1 hit before
	  SiL1_central_time = i_time;
	}
	SiL1_hit = true;
	if (std::fabs(SiL1_central_time - i_time) < merge_time) { // add energy deposits that are close in time
	  SiL1_edep_to_fill += i_edep;	  
	}
	else if (std::fabs(SiL1_central_time - i_time) > tpi_length) { // add another peak
	  SiL1_edeps_to_fill.push_back(SiL1_edep_to_fill);
	  SiL1_times_to_fill.push_back(SiL1_central_time);
	  SiL1_central_time = i_time;
	  SiL1_edep_to_fill = i_edep;
	}
      }

    }
    SiT_edeps_to_fill.push_back(SiT_edep_to_fill); // oput the last edep into the vector
    SiT_times_to_fill.push_back(SiT_central_time);
    SiL1_edeps_to_fill.push_back(SiL1_edep_to_fill); // oput the last edep into the vector
    SiL1_times_to_fill.push_back(SiL1_central_time);

    // pick an energy
    std::vector<double>::const_iterator SiT_element = std::max_element(SiT_edeps_to_fill.begin(), SiT_edeps_to_fill.end());
    std::vector<double>::const_iterator SiL1_element = std::max_element(SiL1_edeps_to_fill.begin(), SiL1_edeps_to_fill.end());
    SiT_edep_to_fill = *(SiT_element);
    SiT_central_time = SiT_times_to_fill.at(SiT_element - SiT_edeps_to_fill.begin());
    SiL1_edep_to_fill = *(SiL1_element);
    SiL1_central_time = SiL1_times_to_fill.at(SiL1_element - SiL1_edeps_to_fill.begin());
    
    double delta_edep = energy_res_fn[0]->GetRandom();
    SiT_edep_to_fill += delta_edep;
    delta_edep = energy_res_fn[1]->GetRandom();
    SiL1_edep_to_fill += delta_edep;

    if (SiT_hit && SiT_edep_to_fill>200 && std::fabs(SiT_central_time - SiL1_central_time) < layer_coinc_time && SiL1_hit && SiL1_edep_to_fill>200) {
      hTwoLayer->Fill(SiT_edep_to_fill+SiL1_edep_to_fill, SiT_edep_to_fill);
    }
  }

  TCanvas* c_SiT = new TCanvas("c_SiT", "c_SiT");
  std::stringstream title;
  title << "Energy Deposited in SiT (MC Truth)";
  hTwoLayer->SetTitle(title.str().c_str());
  hTwoLayer->SetXTitle("E_{SiT} + E_{SiL1} [keV]");
  hTwoLayer->SetYTitle("E_{SiT} [keV]");
  hTwoLayer->Draw("COLZ");

  /*  TCanvas* hDataCanvas = ((TCanvas*)data10477->Get("c1"));
  TH1D* hSiTData10477 = (TH1D*) hDataCanvas->GetPrimitive("sum_hist");
  c_SiT->cd();
  hSiTData10477->Scale(1.0/hSiTData10477->GetMaximum());
  hSiTData10477->SetLineColor(kBlack);
  hSiTData10477->Draw("SAMES");
  */
}
