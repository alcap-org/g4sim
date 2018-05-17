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

void R15b_Si16a_MuonBeamSim_SiTEDepValidation() {
  //  gStyle->SetOptFit(1);

  /////////////////////////////
  // Input Parameters
  std::string filename = "output/raw_g4sim.root";
  //  std::string filename = "output/R15b_100k_Geom-S17_muon-beam-sim.root";
  //  std::string datafilename = "plots09736.root";
  //  std::string datahistname = "hSiT_All";
  std::string datafilename = "plotsSi16b.root";
  std::string datahistname = "hSiTEnergy";
  ///////////////////////////////////


  
  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");

  TH1F* hSiTAll = new TH1F("hSiTAll", "", 60,0,1500);
  TH1F* hSiTStopped = new TH1F("hSiTStopped", "", 60,0,1500);

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
  const int n_si_channels = 1;
  double energy_resolutions[n_si_channels] = {41.0}; // using the mean of these {37.2, 39.9, 38.1, 48.6};
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

    bool SiL1_hit = false;
    bool SiT_hit = false;
    bool SiT_stopped = false;
    std::vector<double> SiT_edeps_to_fill;
    double SiT_edep_to_fill = 0;
    double SiT_central_time = 0;
    double tpi_length = 5000;
    double merge_time = 100; // peaks will merge together within this and we will take the largest of two peaks within 5000 ns
    for (int iElement = 0; iElement < volName->size();  ++iElement) {

      std::string i_volName = volName->at(iElement);
      std::string i_particleName = particleName->at(iElement);
      std::string i_oprocess = oprocess->at(iElement);
      double i_edep = edep->at(iElement)*1e6;
      int i_volID = volID->at(iElement);
      double i_time = time->at(iElement);
      int i_stopped = stopped->at(iElement);
      /*
      if (i_volName == "SiT" && i_particleName=="mu-") {
	SiT_hit = true;
	SiT_edep_to_fill += i_edep;
	if (i_stopped == 1) {
	  SiT_stopped = true;
	}
      }
      */
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
	  SiT_central_time = i_time;
	  SiT_edep_to_fill = i_edep;
	}
	if (i_particleName == "mu-" && i_stopped == 1) {
	  SiT_stopped = true;
	}
      }

      if (i_volName=="SiL1" && i_particleName=="mu-") {
	SiL1_hit = true;
      }
    }
    SiT_edeps_to_fill.push_back(SiT_edep_to_fill); // oput the last edep into the vector
    // pick an energy
    SiT_edep_to_fill = *(std::max_element(SiT_edeps_to_fill.begin(), SiT_edeps_to_fill.end()));
    double delta_edep = energy_res_fn[0]->GetRandom();
    SiT_edep_to_fill += delta_edep;  
    if (SiT_hit && SiT_edep_to_fill>200) {
      hSiTAll->Fill(SiT_edep_to_fill);
      if (SiT_stopped) {
	hSiTStopped->Fill(SiT_edep_to_fill);
      }
    }
  }

  TCanvas* c_SiT = new TCanvas("c_SiT", "c_SiT");
  std::stringstream title;
  title << "Energy Deposited in SiT (MC Truth)";
  hSiTAll->SetTitle(title.str().c_str());
  hSiTAll->SetXTitle("Energy Deposited [keV]");
  hSiTAll->SetLineColor(kRed);
  hSiTAll->Sumw2();
  double scale_factor = 1.0/hSiTAll->GetMaximum();
  hSiTAll->Scale(scale_factor);
  hSiTAll->Draw("HIST E");

  hSiTStopped->Scale(scale_factor);
  hSiTStopped->Draw("SAME");

  TFile* datafile = new TFile(datafilename.c_str(), "READ");
  //  TH1D* hSiTData = ((TH2F*)datafile->Get(datahistname.c_str()))->ProjectionY();
  TH1F* hSiTData = (TH1F*)datafile->Get(datahistname.c_str());
  hSiTData->Scale(1.0/hSiTData->GetMaximum());
  hSiTData->SetLineColor(kBlack);
  hSiTData->Draw("SAMES");
  
  std::cout << "AE: " << hSiTAll->GetBinCenter(hSiTAll->GetMaximumBin()) << " " << hSiTAll->GetMean() << " " << hSiTAll->GetRMS() << std::endl;

  /*
  //    TFile* data10477 = new TFile("AlCapData_Run10477_SiTEDep.root", "READ");
  //  TFile* data10477 = new TFile("AlCapData_Run10477_SiTEDep-wNewSiTCalib.root", "READ");
  TFile* data10477 = new TFile("AlCapData_Run10477_SiTEDep-wNewSiTCalib_SameBinning.root", "READ");
  //  TFile* data10477 = new TFile("AlCapData_Run10477_SiTEDep_SameBinning.root", "READ");
  TCanvas* hDataCanvas = ((TCanvas*)data10477->Get("c1"));
  TH1D* hSiTData10477 = (TH1D*) hDataCanvas->GetPrimitive("sum_hist");
  c_SiT->cd();
  hSiTData10477->Scale(1.0/hSiTData10477->GetMaximum());
  hSiTData10477->SetLineColor(kBlack);
  hSiTData10477->Draw("SAMES");
  */
}
