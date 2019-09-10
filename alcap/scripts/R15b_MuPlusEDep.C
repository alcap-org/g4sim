#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TF1.h"

void R15b_MuPlusEDep(std::string filename, std::string outfilename, std::string detname, double det_resolution, double det_threshold, double det_k_factor) {

  TFile* muplus_file = new TFile(filename.c_str(), "READ");  
  TTree* muplus_tree = (TTree*) muplus_file->Get("tree");

  std::vector<double>* edep = 0;
  TBranch* br_edep = muplus_tree->GetBranch("M_edep");
  br_edep->SetAddress(&edep);
  std::vector<std::string>* volName = 0;
  TBranch* br_volName = muplus_tree->GetBranch("M_volName");
  br_volName->SetAddress(&volName);
  std::vector<double>* t = 0;
  TBranch* br_t = muplus_tree->GetBranch("M_t");
  br_t->SetAddress(&t);
  std::vector<std::string>* particleName = 0;
  TBranch* br_particleName = muplus_tree->GetBranch("M_particleName");
  br_particleName->SetAddress(&particleName);
  std::vector<std::string>* ovolName = 0;
  TBranch* br_ovolName = muplus_tree->GetBranch("M_ovolName");
  br_ovolName->SetAddress(&ovolName);
  std::vector<int>* tid = 0;
  TBranch* br_tid = muplus_tree->GetBranch("M_tid");
  br_tid->SetAddress(&tid);

  double min_energy = 0;
  double max_energy = 50000;
  double energy_width = 50;
  int n_energy_bins = (max_energy - min_energy) / energy_width;

  TH1F* hEDep_muplus = new TH1F("hEDep_muplus", "", n_energy_bins, min_energy, max_energy);
  hEDep_muplus->Reset();
  hEDep_muplus->Sumw2();

  TF1* resolution = new TF1("res", "TMath::Gaus(x, 0, [0])", -250, 250);
  resolution->SetParameter(0, det_resolution);
  
  //  muplus_tree->Draw("M_edep*1e6>>hEDep_muplus", "M_volName==\"Target\" && M_particleName==\"e+\"", "goff");
  int n_entries = muplus_tree->GetEntries();
  for (int i_entry = 0; i_entry < n_entries; ++i_entry) {
    muplus_tree->GetEntry(i_entry);

    if (i_entry % 10000 == 0) {
      std::cout << i_entry << " / " << n_entries << std::endl;
    }


    double total_edep = 0;
    bool didnt_start_in_target = false;
    for (int iElement = 0; iElement < volName->size();  ++iElement) {
      std::string i_volName = volName->at(iElement);
      std::string i_ovolName = ovolName->at(iElement);
      std::string i_particleName = particleName->at(iElement);
      double i_edep = edep->at(iElement)*1e6;
      double i_time = t->at(iElement);
      int i_tid = tid->at(iElement)*1e6;
      
      if (i_tid == 1 && i_ovolName.find("Target")==std::string::npos) { // only want events that started in the target
	//	std::cout << "Actually started in " << i_ovolName << std::endl;
	didnt_start_in_target = true;
	break;
      }

      
      if (i_volName==detname) {
	//       	std::cout << "Event #" << i_entry << ": " << i_volName << ": " << i_particleName << ": i_edep = " << i_edep << " keV, t = " << i_time << " ns" << std::endl;
	total_edep += i_edep;
      }
    }
    double smear = resolution->GetRandom();
    total_edep += smear;
    total_edep *= det_k_factor;
    if (total_edep>det_threshold) {
      //      std::cout << "Total EDep = " << total_edep << std::endl;
      hEDep_muplus->Fill(total_edep);
    }
  }

  //  hEDep_muplus->Scale(1.0 / muplus_tree->GetEntries());
  //  hEDep_muplus->SetYTitle("Per Generated Event");
  hEDep_muplus->Scale(1.0 / hEDep_muplus->Integral());
  hEDep_muplus->SetYTitle("Probability");

  std::cout << "Integral = " << hEDep_muplus->Integral() << std::endl;
  
  TFile* outfile = new TFile(outfilename.c_str(), "RECREATE");
  hEDep_muplus->Write();
  
  outfile->Write();
  outfile->Close();
}
