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

void R15b_Si16a_MuonBeamSim_SiL1EDepValidation() {
  //  gStyle->SetOptFit(1);

  /////////////////////////////
  // Input Parameters
  std::string filename = "output/raw_g4sim.root";
  //  std::string filename = "output/R15b_100k_Geom-S17_muon-beam-sim.root";
  //  std::string datafilename = "plots09740.root";
  //  std::string datahistname = "hTarget_All";
  std::string datafilename = "raw_spectra09740.root";
  std::string datahistname = "RawSpectrum_fromEnergyTime_Thin_All_TimeSlice-200_200/hRawSpectrum";
  ///////////////////////////////////

  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");

  TH1F* hSiL1All = new TH1F("hSiL1All", "", 60,0,1500);
  TH1F* hSiL1Stopped = new TH1F("hSiL1Stopped", "", 60,0,1500);

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
  double energy_resolutions[n_si_channels] = {30.1};
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
    bool SiL1_stopped = false;
    std::vector<double> SiL1_edeps_to_fill;
    double SiL1_edep_to_fill = 0;
    double SiL1_central_time = 0;
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

      /*      if (i_volName == "SiL1" && i_particleName=="mu-") {
	SiL1_hit = true;
	SiL1_edep_to_fill += i_edep;
	if (i_stopped == 1) {
	  SiL1_stopped = true;
	}
      }
      */
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
      }
    }
    SiL1_edeps_to_fill.push_back(SiL1_edep_to_fill); // oput the last edep into the vector
    // pick an energy
    //    SiL1_edep_to_fill = *(std::max_element(SiL1_edeps_to_fill.begin(), SiL1_edeps_to_fill.end()));
    for (std::vector<double>::const_iterator i_edep = SiL1_edeps_to_fill.begin(); i_edep != SiL1_edeps_to_fill.end(); ++i_edep) {
      SiL1_edep_to_fill = *i_edep;
      double delta_edep = energy_res_fn[0]->GetRandom();
      //    SiL1_edep_to_fill += delta_edep;  
      if (SiL1_hit && SiL1_edep_to_fill>200) {
	hSiL1All->Fill(SiL1_edep_to_fill);
	if (SiL1_stopped) {
	  hSiL1Stopped->Fill(SiL1_edep_to_fill);
	}
      }
    }
  }

  TCanvas* c_SiL1 = new TCanvas("c_SiL1", "c_SiL1");
  std::stringstream title;
  title << "Energy Deposited in SiL1 (MC Truth)";
  hSiL1All->SetTitle(title.str().c_str());
  hSiL1All->SetXTitle("Energy Deposited [keV]");
  hSiL1All->SetLineColor(kRed);
  hSiL1All->Sumw2();
  double scale_factor = 1.0/hSiL1All->GetMaximum();
  hSiL1All->Scale(scale_factor);
  hSiL1All->Draw("HIST E");

  hSiL1Stopped->Scale(scale_factor);
  hSiL1Stopped->Draw("SAME");

  TFile* datafile = new TFile(datafilename.c_str(), "READ");
  //  TH1D* hSiL1Data = ((TH2F*)datafile->Get(datahistname.c_str()))->ProjectionY();
  TH1D* hSiL1Data = (TH1D*)datafile->Get(datahistname.c_str());
  hSiL1Data->Rebin(2);
  hSiL1Data->Scale(1.0/hSiL1Data->GetMaximum());
  hSiL1Data->SetLineColor(kBlack);
  hSiL1Data->Draw("SAMES");

  
  /*  //  TFile* data = new TFile("AlCapData_Run10477_SiL1EDep.root", "READ");
  TFile* data = new TFile("AlCapData_Run10477_SiL1EDep-wNewSiTCalib_SameBinning.root", "READ");
  TCanvas* hDataCanvas = ((TCanvas*)data->Get("c1"));
  TH1D* hSiL1Data = (TH1D*) hDataCanvas->GetPrimitive("sum_hist");
  c_SiL1->cd();
  hSiL1Data->Scale(1.0/hSiL1Data->GetMaximum());
  hSiL1Data->SetLineColor(kBlack);
  hSiL1Data->Draw("SAMES");
  */
  std::cout << "AE: " << hSiL1All->GetBinCenter(hSiL1All->GetMaximumBin()) << " " << hSiL1All->GetMean() << " " << hSiL1All->GetRMS() << std::endl;
}
