#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TF1.h"

#include "RooUnfoldResponse.h"

#include <iostream>

void R15b_Si16b_ActiveTargetCorrections() {

  TFile* file = new TFile("output/R15b_10M_Geom-P5_proton-target.root", "READ");

  TTree* tree = (TTree*) file->Get("tree");

  double min_energy = 0;
  double max_energy = 30000;
  double energy_width = 100;
  int n_energy_bins = (max_energy - min_energy) / energy_width;

  TH1F* hEDep_All = new TH1F("hEDep_All", "", n_energy_bins, min_energy, max_energy);
  hEDep_All->SetXTitle("Energy Deposited in Target [keV]");
  hEDep_All->SetLineColor(kBlack);

  TH1F* hEDep_stopped = (TH1F*) hEDep_All->Clone("hEDep_stopped");
  hEDep_stopped->SetLineColor(kRed);
  TH1F* hEDep_not_stopped = (TH1F*) hEDep_All->Clone("hEDep_not_stopped");
  hEDep_not_stopped->SetLineColor(kMagenta);

  std::vector<std::string>* particleName = 0;
  TBranch* br_particleName = tree->GetBranch("M_particleName");
  br_particleName->SetAddress(&particleName);

  std::vector<std::string>* volName = 0;
  TBranch* br_volName = tree->GetBranch("M_volName");
  br_volName->SetAddress(&volName);
    
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
  double proton_mass = 938.272;

  int n_observed_energy_bins = n_energy_bins;
  int n_true_energy_bins = n_energy_bins;
  RooUnfoldResponse* response = new RooUnfoldResponse(n_observed_energy_bins,min_energy,max_energy, n_true_energy_bins,min_energy,max_energy, "Si16b_response");
  
  double max_diff = 0.1; // accept as stopped if there's less than 0.1 keV difference between initial_energy and total_edep
  for (int i_entry = 0; i_entry < tree->GetEntries(); ++i_entry) {
    tree->GetEntry(i_entry);

    double total_edep = 0;
    double initial_energy = 0;
    for (int iElement = 0; iElement < volName->size();  ++iElement) {
      std::string i_volName = volName->at(iElement);
      double i_edep = edep->at(iElement);
      int i_stopped = stopped->at(iElement);
      double i_time = t->at(iElement);

      if(i_volName=="Target") {
	total_edep += i_edep;
      }
      if (iElement == 0) {
	initial_energy = (std::sqrt(i_px*i_px + i_py*i_py + i_pz*i_pz + proton_mass*proton_mass) - proton_mass)*1e3; // convert to keV
      }
    }

    //    std::cout << std::setprecision(10) << "AE: Total EDep = " << total_edep*1e6 << ", Initial E = " << initial_energy << " ";
    hEDep_All->Fill(total_edep*1e6);
    double difference = (initial_energy - total_edep*1e6);
    if (difference > max_diff) {
      //      std::cout << "Not Stopped!" << std::endl;
      hEDep_not_stopped->Fill(total_edep*1e6);
    }
    else {
      //      std::cout << "Stopped!" << std::endl;
      hEDep_stopped->Fill(total_edep*1e6);
      //      response->Fill(total_edep*1e6, initial_energy);
    }
    response->Fill(total_edep*1e6, initial_energy);
  }
  //  tree->Draw("M_edep*1e6>>hEDep_All", "M_volName==\"Target\"", "goff");
  //  tree->Draw("M_edep*1e6>>hEDep_proton", "M_volName==\"Target\" && M_particleName==\"proton\"", "goff");
  //  tree->Draw("M_edep*1e6>>hEDep_proton_stopped", "M_volName==\"Target\" && M_particleName==\"proton\" && M_stopped==1", "goff");
  //  tree->Draw("M_edep*1e6>>hEDep_proton_not_stopped", "M_volName==\"Target\" && M_particleName==\"proton\" && M_stopped==0", "goff");

  hEDep_All->Draw();
  //  hEDep_proton->Draw("SAME");
  hEDep_stopped->Draw("SAME");
  hEDep_not_stopped->Draw("SAME");
  //  hProton_TotalEDepVsInitialEnergy->Draw("COLZ");
  TH1F* hStoppingFraction = (TH1F*) hEDep_stopped->Clone("hStoppingFraction");
  hStoppingFraction->Sumw2();
  hStoppingFraction->Divide(hEDep_All);
  hStoppingFraction->SetYTitle("Stopping Fraction");

  TFile* muplus_file = new TFile("output/R15b_1M_Geom-P5_muplus_new.root", "READ");
  TTree* muplus_tree = (TTree*) muplus_file->Get("tree");

  br_edep = muplus_tree->GetBranch("M_edep");
  br_edep->SetAddress(&edep);
  br_volName = muplus_tree->GetBranch("M_volName");
  br_volName->SetAddress(&volName);
  br_particleName = muplus_tree->GetBranch("M_particleName");
  br_particleName->SetAddress(&particleName);

  TH1F* hEDep_muplus = (TH1F*) hEDep_All->Clone("hEDep_muplus");
  hEDep_muplus->Reset();
  hEDep_muplus->Sumw2();
  TF1* sil3_res_fn = new TF1("res_SiL3", "TMath::Gaus(x, 0, 31.6)", -250, 250);
  //  muplus_tree->Draw("M_edep*1e6>>hEDep_muplus", "M_volName==\"Target\" && M_particleName==\"e+\"", "goff");
  int n_entries = muplus_tree->GetEntries();
  for (int i_entry = 0; i_entry < n_entries; ++i_entry) {
    muplus_tree->GetEntry(i_entry);

    if (i_entry % 10000 == 0) {
      std::cout << i_entry << " / " << n_entries << std::endl;
    }

    double total_edep = 0;
    for (int iElement = 0; iElement < volName->size();  ++iElement) {
      std::string i_volName = volName->at(iElement);
      std::string i_particleName = particleName->at(iElement);
      double i_edep = edep->at(iElement);

      if (i_volName=="SiL3") {
	total_edep += i_edep;
      }
    }
    if (total_edep>0) {
      double smear = sil3_res_fn->GetRandom();
      hEDep_muplus->Fill(total_edep*1e6 + smear);
    }
  }

  //  hEDep_muplus->Scale(1.0 / hEDep_muplus->Integral());
  hEDep_muplus->Scale(1.0 / 1.0e6);
  hEDep_muplus->SetYTitle("Normalised to Unit Area");

  TFile* outfile = new TFile("test.root", "RECREATE");
  //  hEDep_All->Write();
  //  hEDep_proton->Write();
  //  hEDep_stopped->Write();
  //  hEDep_not_stopped->Write();
  //  hStoppingFraction->Write();
  hEDep_muplus->Write();
  //  hProton_TotalEDepVsInitialEnergy->Write();
  //  TH2* hResponse = response->Hresponse();
  //  hResponse->SetXTitle("Observed Energy [keV]");
  //  hResponse->SetYTitle("True Energy [keV]");
  //  response->Write();
  
  outfile->Write();
  outfile->Close();
}
