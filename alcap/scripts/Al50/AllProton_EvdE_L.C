#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH2.h"
#include "TCanvas.h"

#include <iostream>
#include <sstream>

void AllProton_EvdE_L(std::string filename, bool three_layer = false) {

  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");

  std::vector<std::string>* particleName = 0;
  std::vector<std::string>* volName = 0;
  std::vector<std::string>* ovolName = 0;
  std::vector<double>* edep = 0;
  std::vector<int>* stopped = 0;
  std::vector<double>* ekin = 0;
  std::vector<int>* tid = 0;
  std::vector<double>* time = 0;
  double i_px = 0;
  double i_py = 0;
  double i_pz = 0;

  TBranch* br_particleName = tree->GetBranch("M_particleName");
  TBranch* br_volName = tree->GetBranch("M_volName");
  TBranch* br_ovolName = tree->GetBranch("M_ovolName");
  TBranch* br_edep = tree->GetBranch("M_edep");
  TBranch* br_stopped = tree->GetBranch("M_stopped");
  TBranch* br_ekin = tree->GetBranch("M_ekin");
  TBranch* br_tid = tree->GetBranch("M_tid");
  TBranch* br_time = tree->GetBranch("M_Ot");
  TBranch* br_i_px = tree->GetBranch("i_px");
  TBranch* br_i_py = tree->GetBranch("i_py");
  TBranch* br_i_pz = tree->GetBranch("i_pz");

  br_particleName->SetAddress(&particleName);
  br_volName->SetAddress(&volName);
  br_ovolName->SetAddress(&ovolName);
  br_edep->SetAddress(&edep);
  br_stopped->SetAddress(&stopped);
  br_ekin->SetAddress(&ekin);
  br_tid->SetAddress(&tid);
  br_time->SetAddress(&time);
  br_i_px->SetAddress(&i_px);
  br_i_py->SetAddress(&i_py);
  br_i_pz->SetAddress(&i_pz);

  double max_true_ekin = 50000;
  double max_total_edep = 25000;
  double max_thin_edep = 3000;
  double min_edep = 0;
  double edep_width = 100;
  int n_bins_total_edep = (max_total_edep - min_edep) / edep_width;
  int n_bins_thin_edep = (max_thin_edep - min_edep) / edep_width;
  int n_bins_true_ekin = (max_true_ekin - min_edep) / edep_width;

  //  TH2F* hEvdE_SiL_stopped = new TH2F("hEvdE_SiL_stopped", "hEvdE_SiL_stopped", n_bins_total_edep,min_edep,max_total_edep, n_bins_thin_edep,min_edep,max_thin_edep);
  TH2F* hEvdE_SiL_TwoHits_stopped = new TH2F("hEvdE_SiL_TwoHits_stopped", "Two Hits", n_bins_total_edep,min_edep,max_total_edep, n_bins_thin_edep,min_edep,max_thin_edep);
  hEvdE_SiL_TwoHits_stopped->SetMarkerColor(kRed);
  hEvdE_SiL_TwoHits_stopped->SetMarkerStyle(7);
  hEvdE_SiL_TwoHits_stopped->SetXTitle("E_{1} + E_{2} [keV]");
  hEvdE_SiL_TwoHits_stopped->GetYaxis()->SetTitleOffset(1.4);
  hEvdE_SiL_TwoHits_stopped->SetYTitle("E_{1} [keV]");

  TH2F* hEvdE_SiL_TwoHits_not_stopped = new TH2F("hEvdE_SiL_TwoHits_not_stopped", "Two Hits", n_bins_total_edep,min_edep,max_total_edep, n_bins_thin_edep,min_edep,max_thin_edep);
  hEvdE_SiL_TwoHits_not_stopped->SetMarkerColor(kBlue);
  hEvdE_SiL_TwoHits_not_stopped->SetMarkerStyle(7);
  hEvdE_SiL_TwoHits_not_stopped->SetXTitle("E_{1} + E_{2} [keV]");
  hEvdE_SiL_TwoHits_not_stopped->GetYaxis()->SetTitleOffset(1.4);
  hEvdE_SiL_TwoHits_not_stopped->SetYTitle("E_{1} [keV]");


  TH2F* hEvdE_SiL_ThreeHits_stopped = new TH2F("hEvdE_SiL_ThreeHits_stopped", "Three Hits", n_bins_total_edep,min_edep,max_total_edep, n_bins_total_edep,min_edep,max_total_edep);
  hEvdE_SiL_ThreeHits_stopped->SetMarkerColor(kRed);
  hEvdE_SiL_ThreeHits_stopped->SetMarkerStyle(7);
  hEvdE_SiL_ThreeHits_stopped->SetXTitle("E_{1} + E_{2} + E_{3} [keV]");
  hEvdE_SiL_ThreeHits_stopped->GetYaxis()->SetTitleOffset(1.4);
  hEvdE_SiL_ThreeHits_stopped->SetYTitle("E_{1} + E_{2} [keV]");

  TH2F* hEvdE_SiL_ThreeHits_not_stopped = new TH2F("hEvdE_SiL_ThreeHits_not_stopped", "Three Hits", n_bins_total_edep,min_edep,max_total_edep, n_bins_total_edep,min_edep,max_total_edep);
  hEvdE_SiL_ThreeHits_not_stopped->SetMarkerColor(kBlue);
  hEvdE_SiL_ThreeHits_not_stopped->SetMarkerStyle(7);
  hEvdE_SiL_ThreeHits_not_stopped->SetXTitle("E_{1} + E_{2} + E_{3} [keV]");
  hEvdE_SiL_ThreeHits_not_stopped->GetYaxis()->SetTitleOffset(1.4);
  hEvdE_SiL_ThreeHits_not_stopped->SetYTitle("E_{1} + E_{2} [keV]");


  TH2F* hEvdE_SiL_TwoHits_not_stopped_true = new TH2F("hEvdE_SiL_TwoHits_not_stopped_true", "Two Hits", n_bins_true_ekin,min_edep,max_true_ekin, n_bins_total_edep,min_edep,max_total_edep);
  hEvdE_SiL_TwoHits_not_stopped_true->SetMarkerColor(kBlue);
  hEvdE_SiL_TwoHits_not_stopped_true->SetMarkerStyle(7);
  hEvdE_SiL_TwoHits_not_stopped_true->SetYTitle("E_{1} + E_{2} [keV]");
  hEvdE_SiL_TwoHits_not_stopped_true->GetYaxis()->SetTitleOffset(1.4);
  hEvdE_SiL_TwoHits_not_stopped_true->SetXTitle("Initial Kinetic Energy [keV]");

  TH2F* hEvdE_SiL_TwoHits_stopped_true = new TH2F("hEvdE_SiL_TwoHits_stopped_true", "Two Hits", n_bins_true_ekin,min_edep,max_true_ekin, n_bins_total_edep,min_edep,max_total_edep);
  hEvdE_SiL_TwoHits_stopped_true->SetMarkerColor(kRed);
  hEvdE_SiL_TwoHits_stopped_true->SetMarkerStyle(7);
  hEvdE_SiL_TwoHits_stopped_true->SetYTitle("E_{1} + E_{2} [keV]");
  hEvdE_SiL_TwoHits_stopped_true->GetYaxis()->SetTitleOffset(1.4);
  hEvdE_SiL_TwoHits_stopped_true->SetXTitle("Initial Kinetic Energy [keV]");

  TH2F* hEvdE_SiL_ThreeHits_not_stopped_true = new TH2F("hEvdE_SiL_ThreeHits_not_stopped_true", "Three Hits", n_bins_true_ekin,min_edep,max_true_ekin, n_bins_total_edep,min_edep,max_total_edep);
  hEvdE_SiL_ThreeHits_not_stopped_true->SetMarkerColor(kBlue);
  hEvdE_SiL_ThreeHits_not_stopped_true->SetMarkerStyle(7);
  hEvdE_SiL_ThreeHits_not_stopped_true->SetYTitle("E_{1} + E_{2} + E_{3} [keV]");
  hEvdE_SiL_ThreeHits_not_stopped_true->GetYaxis()->SetTitleOffset(1.4);
  hEvdE_SiL_ThreeHits_not_stopped_true->SetXTitle("Initial Kinetic Energy [keV]");

  TH2F* hEvdE_SiL_ThreeHits_stopped_true = new TH2F("hEvdE_SiL_ThreeHits_stopped_true", "Three Hits", n_bins_true_ekin,min_edep,max_true_ekin, n_bins_total_edep,min_edep,max_total_edep);
  hEvdE_SiL_ThreeHits_stopped_true->SetMarkerColor(kRed);
  hEvdE_SiL_ThreeHits_stopped_true->SetMarkerStyle(7);
  hEvdE_SiL_ThreeHits_stopped_true->SetYTitle("E_{1} + E_{2} + E_{3} [keV]");
  hEvdE_SiL_ThreeHits_stopped_true->GetYaxis()->SetTitleOffset(1.4);
  hEvdE_SiL_ThreeHits_stopped_true->SetXTitle("Initial Kinetic Energy [keV]");

  const int n_combinations = 64;
  TH1F* hCombinations = new TH1F("hCombinations", "Combinations of Hits and Stops in Three Layers", n_combinations,0,n_combinations);

  const int n_layers = 3;

  for (int iEntry = 0; iEntry < tree->GetEntries(); ++iEntry) {
    tree->GetEvent(iEntry);

    std::string layer_name[n_layers] = {"SiL1", "SiL3", "SiL2"};
    double edep_layer[n_layers] = {0, 0, 0};
    bool hit[n_layers] = {false, false, false};
    bool stop[n_layers] = {false, false, false};
    int track_id[n_layers] = {0, 0, 0};
    double time_layer[n_layers] = {0, 0, 0};
    double ekin_layer[n_layers] = {0, 0, 0};
    double ekin_true = std::sqrt(i_px*i_px*1e6 + i_py*i_py*1e6 + i_pz*i_pz*1e6 + 938*938*1e6) - 938*1e3;

    for (int iElement = 0; iElement < particleName->size();  ++iElement) {

      // Check if this is a proton stopped in the target
      if (particleName->at(iElement) == "proton" && ovolName->at(iElement) == "Target") {

	// Now check if this is a hit/stop in one of the silicon layers
	for (int i_layer = 0; i_layer < n_layers; ++i_layer) {
	  if (volName->at(iElement) == layer_name[i_layer]) {
	    hit[i_layer] = true;
	    edep_layer[i_layer] = edep->at(iElement)*1e6; // convert to keV
	    track_id[i_layer] = tid->at(iElement);
	    time_layer[i_layer] = time->at(iElement);
	    ekin_layer[i_layer] = ekin->at(iElement)*1e6; // convert to keV

	    if (stopped->at(iElement) == 1) {
	      stop[i_layer] = true;
	    }
	  }
	}
      }
    }

    // Check that the hit/stop layers make sense
    std::stringstream combination;
    combination << hit[0] << hit[1] << hit[2] << stop[0] << stop[1] << stop[2];
    hCombinations->Fill(combination.str().c_str(), 1);
    if ( combination.str() == "000000" || // if it doesn't hit the silicon at all
	 combination.str() == "100000" || // one-layer hit and no stop (I guess if it glances the silicon...)
	 combination.str() == "100100") { // one-layer hit and stop
      // ignore
    }
    else if (combination.str() == "110000") { // two-layer hit and no stop (for two-layer geometry I guess....)
      hEvdE_SiL_TwoHits_not_stopped->Fill(edep_layer[0]+edep_layer[1], edep_layer[0]);
      hEvdE_SiL_TwoHits_not_stopped_true->Fill(ekin_true, edep_layer[0]+edep_layer[1]);
    }
    else if (combination.str() == "110010") { // two-layer hit and stop
      hEvdE_SiL_TwoHits_stopped->Fill(edep_layer[0]+edep_layer[1], edep_layer[0]);
      hEvdE_SiL_TwoHits_stopped_true->Fill(ekin_true, edep_layer[0]+edep_layer[1]);
    }
    else if (combination.str() == "111001") { // three-layer hit and stop
      hEvdE_SiL_ThreeHits_stopped->Fill(edep_layer[0]+edep_layer[1]+edep_layer[2], edep_layer[0]+edep_layer[1]);
      hEvdE_SiL_ThreeHits_stopped_true->Fill(ekin_true, edep_layer[0]+edep_layer[1]+edep_layer[2]);
    }
    else if (combination.str() == "111000") { // three-layer hit and no stop
      hEvdE_SiL_ThreeHits_not_stopped->Fill(edep_layer[0]+edep_layer[1]+edep_layer[2], edep_layer[0]+edep_layer[1]);
      hEvdE_SiL_ThreeHits_not_stopped_true->Fill(ekin_true, edep_layer[0]+edep_layer[1]+edep_layer[2]);
    }
    else {
      std::cout << "Odd series of hits and stops: " << std::endl;
      std::cout << "Name\tHit\tStop\tedep\ttime\tID" << std::endl;
      for (int i_layer = 0; i_layer < n_layers; ++i_layer) {
	std::cout << layer_name[i_layer] << "\t" << hit[i_layer] << "\t" << stop[i_layer] << "\t" << edep_layer[i_layer] << "\t" << time_layer[i_layer] << "\t" << track_id[i_layer] << std::endl;
      }
    }
  }

  TCanvas* c1 = new TCanvas("c1", "c1");
  hEvdE_SiL_TwoHits_stopped->SetTitle("E_{1} vs E_{1} + E_{2} for hits in first two layers");
  hEvdE_SiL_TwoHits_stopped->Draw();
  hEvdE_SiL_TwoHits_not_stopped->Draw("SAMES");

  TCanvas* c2 = new TCanvas("c2", "c2");
  hEvdE_SiL_ThreeHits_stopped->SetTitle("E_{1} + E_{2} vs E_{1} + E_{2} + E_{3} for hits in all layers");
  hEvdE_SiL_ThreeHits_stopped->Draw();
  hEvdE_SiL_ThreeHits_not_stopped->Draw("SAMES");

  TCanvas* c3 = new TCanvas("c3", "c3");
  hEvdE_SiL_ThreeHits_stopped_true->SetTitle("E_{1} + E_{2} + E_{3} vs true initial kinetic energy for both two hit and three hit protons");
  hEvdE_SiL_ThreeHits_stopped_true->Draw();
  hEvdE_SiL_TwoHits_not_stopped_true->Draw("SAMES");
  hEvdE_SiL_TwoHits_stopped_true->Draw("SAMES");
  hEvdE_SiL_ThreeHits_not_stopped_true->Draw("SAMES");

}
