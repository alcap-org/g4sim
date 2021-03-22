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

void R15b_Si16a_MuonBeamSim_EDepValidation() {
  //  gStyle->SetOptFit(1);

  //  std::string filename = "output/R15b_1M_Geom-S14_Run9737_muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-S14_Run9743_muon-beam-sim.root";
  std::string filename = "output/raw_g4sim.root";
  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");

  TH1F* hSiL1All = new TH1F("hSiL1All", "", 100,0,5000);
  TH1F* hSiL1wSiL3Hit = new TH1F("hSiL1wSiL3Hit", "", 100,0,5000);
  TH1F* hSiL1woutSiL3Hit = new TH1F("hSiL1woutSiL3Hit", "", 100,0,5000);
  TH1F* hSiL3All = new TH1F("hSiL3All", "", 100,0,5000);
  TH1F* hSiL3wSiL1Hit = new TH1F("hSiL3wSiL1Hit", "", 100,0,5000);
  TH1F* hSiL3woutSiL1Hit = new TH1F("hSiL3woutSiL1Hit", "", 100,0,5000);

  std::vector<std::string>* volName = 0;
  std::vector<std::string>* particleName = 0;
  std::vector<std::string>* oprocess = 0;
  std::vector<double>* edep = 0;
  std::vector<int>* volID = 0;
  std::vector<double>* time = 0;

  TBranch* br_volName = tree->GetBranch("M_volName");
  TBranch* br_particleName = tree->GetBranch("M_particleName");
  TBranch* br_oprocess = tree->GetBranch("M_oprocess");
  TBranch* br_edep = tree->GetBranch("M_edep");
  TBranch* br_volID = tree->GetBranch("M_volID");
  TBranch* br_time = tree->GetBranch("M_t");

  br_volName->SetAddress(&volName);
  br_particleName->SetAddress(&particleName);
  br_oprocess->SetAddress(&oprocess);
  br_edep->SetAddress(&edep);
  br_volID->SetAddress(&volID);
  br_time->SetAddress(&time);

  int n_entries = tree->GetEntries();
  //  const int n_si_channels = 16;
  // from SiL1 energy calibration (Am241 fit sigma, NB SiL1-1-S and SiL1-16S are miscalibrated)
  //  double energy_resolutions[n_si_channels] = {1031.69, 84.4072, 86.4528, 69.5987, 65.1556, 57.732, 69.3947, 50.0809, 64.0503, 50.8744, 67.9137, 62.3101, 76.8118, 55.7161, 59.978, 1037.23};
  const int n_si_channels = 2; // SiL1 and SiL3
  double energy_resolutions[n_si_channels] = {30.1, 30.9}; // 30.1 is average of the SiL1 resolutions
  TF1* energy_res_fn[n_si_channels];
  for (int i_si_channel = 0; i_si_channel < n_si_channels; ++i_si_channel) {
    energy_res_fn[i_si_channel] = new TF1("energy_res_fn", "TMath::Gaus(x, 0, [0])", -250, 250);
    energy_res_fn[i_si_channel]->SetParameter(0, energy_resolutions[i_si_channel]);
  }

  for (int iEntry = 0; iEntry < n_entries; ++iEntry) {
    tree->GetEvent(iEntry);
    //    std::cout << std::endl << std::endl;
    if (iEntry % 10000 == 0) {
      std::cout << iEntry << " / " << n_entries << std::endl;
    }

    bool SiL1_hit = false;
    bool SiL3_hit = false;
    double SiL1_central_time = 0;
    double SiL3_central_time = 0;
    double SiL1_edep_to_fill = 0;
    std::vector<double> SiL1_edeps_to_fill;
    double SiL3_edep_to_fill = 0;
    std::vector<double> SiL3_edeps_to_fill;
    double tpi_length = 5000;
    double merge_time = 100; // peaks will merge together within this and we will take the largest of two peaks within 5000 ns
    for (int iElement = 0; iElement < volName->size();  ++iElement) {

      std::string i_volName = volName->at(iElement);
      std::string i_particleName = particleName->at(iElement);
      std::string i_oprocess = oprocess->at(iElement);
      double i_edep = edep->at(iElement)*1e6;
      int i_volID = volID->at(iElement);
      double i_time = time->at(iElement);

      if (i_volName == "Target" && i_volID>0 && i_volID<15 && i_edep>100) { // don't want to include SiL1-1-S and SiL1-16-S
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
      }
      if (i_volName == "SiL3" && i_edep>100) {
	if (!SiL3_hit) { // if we haven't seen a SiL3 hit before
	  SiL3_central_time = i_time;
	}
	SiL3_hit = true;
	if (std::fabs(SiL3_central_time - i_time) < merge_time) { // add energy deposits that are close in time
	  SiL3_edep_to_fill += i_edep;
	  //	  std::cout << "AE: " << i_edep << " keV (" << i_particleName << ", " << std::fabs(SiL3_central_time - i_time) << " ns)" << std::endl;
	}
	else if (std::fabs(SiL3_central_time - i_time) > tpi_length) { // add another peak
	  //	  std::cout << "AE: " << i_edep << " keV (" << i_particleName << ", " << std::fabs(SiL3_central_time - i_time) << " ns)" << std::endl;
	  SiL3_edeps_to_fill.push_back(SiL3_edep_to_fill);
	  SiL3_central_time = i_time;
	  SiL3_edep_to_fill = i_edep;
	}
      }
    }
    SiL1_edeps_to_fill.push_back(SiL1_edep_to_fill); // oput the last edep into the vector
    SiL3_edeps_to_fill.push_back(SiL3_edep_to_fill); // oput the last edep into the vector
    //    std::cout << "SiL3 EDeps: " << std::endl;
    //    for (int i = 0; i < SiL3_edeps_to_fill.size(); ++i) {
    //      std::cout << SiL3_edeps_to_fill.at(i) << " keV" << " ";
    //    }
    SiL1_edep_to_fill = *(std::max_element(SiL1_edeps_to_fill.begin(), SiL1_edeps_to_fill.end()));
    SiL3_edep_to_fill = *(std::max_element(SiL3_edeps_to_fill.begin(), SiL3_edeps_to_fill.end()));

    double delta_edep = energy_res_fn[0]->GetRandom();
    SiL1_edep_to_fill += delta_edep;
    delta_edep = energy_res_fn[1]->GetRandom();
    SiL3_edep_to_fill += delta_edep;
    if (SiL1_hit) {
      // smear the energy
      hSiL1All->Fill(SiL1_edep_to_fill);
    }
    if (SiL3_hit) {
      hSiL3All->Fill(SiL3_edep_to_fill);
    }
    if (SiL1_hit && SiL3_hit) {
      hSiL1wSiL3Hit->Fill(SiL1_edep_to_fill);
      hSiL3wSiL1Hit->Fill(SiL3_edep_to_fill);
    }
    if (SiL1_hit && !SiL3_hit) {
      hSiL1woutSiL3Hit->Fill(SiL1_edep_to_fill);
    }
    if (!SiL1_hit && SiL3_hit) {
      hSiL3woutSiL1Hit->Fill(SiL3_edep_to_fill);
    }
  }

  TCanvas* c_SiL1 = new TCanvas("c_SiL1", "c_SiL1");
  std::stringstream title;
  title << "Energy Deposited in SiL1 (MC Truth with #sigma_{E} from SiL1 calibrations)";
  hSiL1All->SetTitle(title.str().c_str());
  hSiL1All->SetXTitle("Energy Deposited [keV]");
  hSiL1All->SetLineColor(kRed);
  hSiL1All->Sumw2();
  double sil1_scale_factor = hSiL1All->GetMaximum();
  hSiL1All->Scale(1.0/sil1_scale_factor);
  hSiL1All->Draw("HIST E");
  hSiL1wSiL3Hit->SetLineColor(kBlue);
  hSiL1wSiL3Hit->Scale(1.0/sil1_scale_factor);
  hSiL1wSiL3Hit->Draw("HIST SAME");
  hSiL1woutSiL3Hit->SetLineColor(kCyan);
  hSiL1woutSiL3Hit->Scale(1.0/sil1_scale_factor);
  hSiL1woutSiL3Hit->Draw("HIST SAME");

  TCanvas* c_SiL3 = new TCanvas("c_SiL3", "c_SiL3");
  title.str("");
  title << "Energy Deposited in SiL3 (MC Truth)";
  hSiL3All->SetTitle(title.str().c_str());
  hSiL3All->SetXTitle("Energy Deposited [keV]");
  hSiL3All->SetLineColor(kRed);
  hSiL3All->Sumw2();
  double sil3_scale_factor = hSiL3All->GetMaximum();
  hSiL3All->Scale(1.0/sil3_scale_factor);
  hSiL3All->Draw("HIST E");
  hSiL3wSiL1Hit->SetLineColor(kBlue);
  hSiL3wSiL1Hit->Scale(1.0/sil3_scale_factor);
  //  hSiL3wSiL1Hit->Draw("HIST SAME");
  hSiL3woutSiL1Hit->SetLineColor(kRed);
  hSiL3woutSiL1Hit->Scale(1.0/sil3_scale_factor);
  //  hSiL3woutSiL1Hit->Draw("HIST SAME");

  TFile* data = new TFile("plots09740.root", "READ");
  c_SiL1->cd();
  TH1D* hSiL1Data = ((TH2F*)data->Get("hTarget_All"))->ProjectionY();
  hSiL1Data->Scale(1.0/hSiL1Data->GetMaximum());
  hSiL1Data->SetLineColor(kBlack);
  hSiL1Data->Draw("HIST SAMES");

  c_SiL3->cd();
  TH1D* hSiL3Data = ((TH2F*)data->Get("hThick_All"))->ProjectionY();
  hSiL3Data->Scale(1.0/hSiL3Data->GetMaximum());
  hSiL3Data->SetLineColor(kBlack);
  hSiL3Data->Draw("HIST SAMES");
}
