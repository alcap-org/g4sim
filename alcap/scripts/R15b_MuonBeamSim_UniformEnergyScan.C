#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TF1.h"
#include "TH2.h"

void R15b_MuonBeamSim_UniformEnergyScan() {

  //  std::string filename = "output/R15b_100k_Geom-P5_Ekin-1-1MeV_EkinSpread-0.4MeVUniform_new-beam-pos_muon-beam-sim.root";
  //  std::string filename = "output/R15b_1M_Geom-P5_Ekin-1-1MeV_EkinSpread-0.6MeVUniform_new-beam-pos_muon-beam-sim.root";
  //  std::string filename = "output/R15b_100k_Geom-P5_Ekin-1-19MeV_EkinSpread-0-157MeV_new-beam-pos_muon-beam-sim.root";
  //  std::string filename = "output/raw_g4sim.root";
  std::string filename = "output/R15b_1M_Geom-P5_Ekin-1-5MeV_EkinSpread-1.2MeVUniform_new-beam-pos_muon-beam-sim.root";
  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");

  double min_stop_energy = 0.9;
  double max_stop_energy = 2.1;
  double min_energy = 0.0;
  double max_energy = 2.5;
  double bin_width = 0.01;
  int n_bins = (max_energy - min_energy) / bin_width;

  TH1F* hStops = new TH1F("hStops", "", n_bins,min_energy,max_energy);
  hStops->SetLineColor(kRed);
  TH1F* hNotStops = new TH1F("hNotStops", "", n_bins,min_energy,max_energy);
  TH1F* hAll = new TH1F("hAll", "", n_bins,min_energy,max_energy);
  TH1F* hEDepAll = new TH1F("hEDepAll", "", n_bins,min_energy,max_energy);
  TH1F* hEDepStops = new TH1F("hEDepStops", "", n_bins,min_energy,max_energy);
  TH1F* hEDepNotStops = new TH1F("hEDepNotStops", "", n_bins,min_energy,max_energy);
  TH2F* hEKinVsEDepStops = new TH2F("hEKinVsEDepStops", "", n_bins,min_energy,max_energy, n_bins,min_energy,max_energy);

  std::vector<std::string>* particleName = 0;
  TBranch* br_particleName = tree->GetBranch("M_particleName");
  br_particleName->SetAddress(&particleName);

  std::vector<std::string>* volName = 0;
  TBranch* br_volName = tree->GetBranch("M_volName");
  br_volName->SetAddress(&volName);

  std::vector<std::string>* oprocess = 0;
  TBranch* br_oprocess = tree->GetBranch("M_oprocess");
  br_oprocess->SetAddress(&oprocess);

  std::vector<double>* edep = 0;
  TBranch* br_edep = tree->GetBranch("M_edep");
  br_edep->SetAddress(&edep);
  
  std::vector<int>* stopped = 0;
  TBranch* br_stopped = tree->GetBranch("M_stopped");
  br_stopped->SetAddress(&stopped);
  
  std::vector<double>* t = 0;
  TBranch* br_t = tree->GetBranch("M_t");
  br_t->SetAddress(&t);

  double i_px = 0;
  TBranch* br_i_px = tree->GetBranch("i_px");
  br_i_px->SetAddress(&i_px);
  double i_py = 0;
  TBranch* br_i_py = tree->GetBranch("i_py");
  br_i_py->SetAddress(&i_py);
  double i_pz = 0;
  TBranch* br_i_pz = tree->GetBranch("i_pz");
  br_i_pz->SetAddress(&i_pz);

  for (int i_entry = 0; i_entry < tree->GetEntries(); ++i_entry) {
    tree->GetEntry(i_entry);

    double total_edep = 0;
    bool this_stopped = false;
    bool hit_target = false;
    double E_kin = std::sqrt(i_px*i_px + i_py*i_py + i_pz*i_pz + 105.66*105.66) - 105.66;
    double muon_time = 0;
    for (int i_element = 0; i_element < volName->size();  ++i_element) {
      std::string i_volName = volName->at(i_element);
      std::string i_particleName = particleName->at(i_element);
      double i_edep = edep->at(i_element);
      double i_stopped = stopped->at(i_element);
      double i_t = t->at(i_element);
      std::string i_oprocess = oprocess->at(i_element);
      if (i_volName == "Target") {
	if (i_particleName == "mu-") {
	  hit_target = true;
	  muon_time = i_t;
	  if (i_stopped) {
	    this_stopped = true;
	  }
	}

	if (std::fabs(muon_time - i_t) < 2) {
	  total_edep += i_edep;
	}
      }

    }
    if (hit_target) {
      total_edep *= 1e3;
      hAll->Fill(E_kin);
      hEDepAll->Fill(total_edep);
    
      if (this_stopped) {
	hStops->Fill(E_kin);
	hEDepStops->Fill(total_edep);
	hEKinVsEDepStops->Fill(E_kin, total_edep);
      }
      else {
	hNotStops->Fill(E_kin);
	hEDepNotStops->Fill(total_edep);
      }
    }
  }

  /*
  tree->Draw("sqrt(i_px^2 + i_py^2 + i_pz^2 + 105.66^2) - 105.66>>hAll", "M_volName==\"Target\" && M_particleName==\"mu-\"", "goff");
  tree->Draw("sqrt(i_px^2 + i_py^2 + i_pz^2 + 105.66^2) - 105.66>>hNotStops", "M_stopped==0 && M_volName==\"Target\" && M_particleName==\"mu-\"", "HIST E");
  tree->Draw("sqrt(i_px^2 + i_py^2 + i_pz^2 + 105.66^2) - 105.66>>hStops", "M_stopped==1 && M_volName==\"Target\" && M_particleName==\"mu-\"", "HIST E SAMES");
  tree->Draw("M_edep*1e3>>hEDepAll", "M_volName==\"Target\" && M_particleName==\"mu-\"", "goff");
  tree->Draw("M_edep*1e3>>hEDepNotStops", "M_stopped==0 && M_volName==\"Target\" && M_particleName==\"mu-\"", "HIST E");
  tree->Draw("M_edep*1e3>>hEDepStops", "M_stopped==1 && M_volName==\"Target\" && M_particleName==\"mu-\"", "HIST E SAMES");
  */

  TH1F* hStoppingFraction = (TH1F*) hStops->Clone("hStoppingFraction");
  hStoppingFraction->Divide(hAll);
  hStoppingFraction->Draw();

  TF1* beam_energy = new TF1("beam_energy", "TMath::Gaus(x, [0], [1])", min_energy, max_energy);
  //  beam_energy->SetParameters(1.19, 0.157);
  //  beam_energy->SetParameters(1.14, 0.157);
  //  beam_energy->SetParameters(1.15, 0.180); // was used before
  //  beam_energy->SetParameters(0.922, 0.154);
  //  beam_energy->SetParameters(1.7, 0.2);
  beam_energy->SetParameters(0.9, 0.3);
  //  beam_energy->Draw("LSAME");

  TH1F* hResult = new TH1F("hResult", "", n_bins,min_energy,max_energy);
  
  int n_events = 1000000;
  for (int i_event = 0; i_event < n_events; ++i_event) {
    double energy = beam_energy->GetRandom();
    double stopping_fraction = hStoppingFraction->GetBinContent(hStoppingFraction->FindBin(energy));
    if (energy < min_stop_energy) {
      stopping_fraction = 1;
    }
    else if (energy > max_stop_energy) {
      stopping_fraction = 0;
    }

    if (stopping_fraction > 0) {
      //      hResult->Fill(energy, 1.0/stopping_fraction);
      hResult->Fill(energy, stopping_fraction);
    }
  }
  hResult->Scale(1.0/hResult->GetMaximum());
  hResult->Draw();
  hStoppingFraction->Draw("SAME");
  beam_energy->Draw("LSAME");
  

  /*  TH1F* hEdep = new TH1F("hEdep", "", n_bins,min_energy,max_energy);
  tree->Draw("M_edep*1e3>>hEdep", "M_volName==\"Target\" && M_particleName==\"mu-\" && M_stopped==1", "");
  TH1F* hDecon = new TH1F("hDecon", "", n_bins,min_energy,max_energy);
  for (int i_bin = 1; i_bin <= hEdep->GetNbinsX(); ++i_bin) {
    double bin_content = hEdep->GetBinContent(i_bin);
    double stopping_fraction = hStoppingFraction->GetBinContent(i_bin);

    if (stopping_fraction > 0) {
      hDecon->SetBinContent(i_bin, bin_content / stopping_fraction); 
    }
  }
  hDecon->Draw();
  */
  //  beam_energy->Draw("LSAME");
}
