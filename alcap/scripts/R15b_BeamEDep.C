#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TF1.h"

#include "RooUnfoldResponse.h"

void R15b_BeamEDep(std::string filename, std::string outfilename, std::string detname, double det_resolution, double det_threshold, double det_k_factor) {

  TFile* file = new TFile(filename.c_str(), "READ");  
  TTree* tree = (TTree*) file->Get("tree");

  std::vector<double>* edep = 0;
  TBranch* br_edep = tree->GetBranch("M_edep");
  br_edep->SetAddress(&edep);
  std::vector<std::string>* volName = 0;
  TBranch* br_volName = tree->GetBranch("M_volName");
  br_volName->SetAddress(&volName);
  std::vector<double>* t = 0;
  TBranch* br_t = tree->GetBranch("M_t");
  br_t->SetAddress(&t);
  std::vector<std::string>* particleName = 0;
  TBranch* br_particleName = tree->GetBranch("M_particleName");
  br_particleName->SetAddress(&particleName);
  std::vector<std::string>* ovolName = 0;
  TBranch* br_ovolName = tree->GetBranch("M_ovolName");
  br_ovolName->SetAddress(&ovolName);
  std::vector<int>* tid = 0;
  TBranch* br_tid = tree->GetBranch("M_tid");
  br_tid->SetAddress(&tid);
  std::vector<bool>* stopped = 0;
  TBranch* br_stopped = tree->GetBranch("M_stopped");
  br_stopped->SetAddress(&stopped);

  double i_px = 0;
  TBranch* br_i_px = tree->GetBranch("i_px");
  br_i_px->SetAddress(&i_px);
  double i_py = 0;
  TBranch* br_i_py = tree->GetBranch("i_py");
  br_i_py->SetAddress(&i_py);
  double i_pz = 0;
  TBranch* br_i_pz = tree->GetBranch("i_pz");
  br_i_pz->SetAddress(&i_pz);

  double u = 931.494*1000; // convert to keV
  double muon_mass = 0.1134289267*u;
  double mass = muon_mass;

  double min_energy = 0;
  double max_energy = 50000;
  double energy_width = 50;
  int n_energy_bins = (max_energy - min_energy) / energy_width;

  TH1F* hEDep_beam = new TH1F("hEDep_beam", "", n_energy_bins, min_energy, max_energy);
  hEDep_beam->Reset();
  hEDep_beam->Sumw2();

  double min_true_kinetic_energy = 0;
  double max_true_kinetic_energy = 6000;
  double true_kinetic_energy_width = 50;
  int n_true_kinetic_energy_bins = (max_true_kinetic_energy - min_true_kinetic_energy) / true_kinetic_energy_width;
  TH1F* hInputEnergy = new TH1F("hInputEnergy", "", n_true_kinetic_energy_bins,min_true_kinetic_energy,max_true_kinetic_energy);
  TH1F* hInputEnergy_Stop = new TH1F("hInputEnergy_Stop", "", n_true_kinetic_energy_bins,min_true_kinetic_energy,max_true_kinetic_energy);
  

  TF1* resolution = new TF1("res", "TMath::Gaus(x, 0, [0])", -250, 250);
  resolution->SetParameter(0, det_resolution);
  
  //  tree->Draw("M_edep*1e6>>hEDep", "M_volName==\"Target\" && M_particleName==\"e+\"", "goff");
  int n_entries = tree->GetEntries();
  for (int i_entry = 0; i_entry < n_entries; ++i_entry) {
    tree->GetEntry(i_entry);

    if (i_entry % 10000 == 0) {
      std::cout << i_entry << " / " << n_entries << std::endl;
    }

    double true_px = i_px*1e3;
    double true_py = i_py*1e3;
    double true_pz = i_pz*1e3;
    double squared_mom = true_px*true_px + true_py*true_py + true_pz*true_pz;
    double total_energy = std::sqrt(squared_mom + mass*mass);
    double true_kinetic_energy = total_energy - mass;
    //    std::cout << "True Kinetic Energy = " << true_kinetic_energy << " keV" << std::endl;

    double total_edep = 0;
    bool mu_stop = false;
    for (int iElement = 0; iElement < volName->size();  ++iElement) {
      std::string i_volName = volName->at(iElement);
      std::string i_ovolName = ovolName->at(iElement);
      std::string i_particleName = particleName->at(iElement);
      double i_edep = edep->at(iElement)*1e6;
      double i_time = t->at(iElement);
      int i_tid = tid->at(iElement)*1e6;
      bool i_stopped = stopped->at(iElement);
      
      if (i_volName==detname){// && i_time<100) {
	//	if (i_edep>0) {
	//	  std::cout << "Event #" << i_entry << ": " << i_volName << ": " << i_particleName << ": i_edep = " << i_edep << " keV, t = " << i_time << " ns" << std::endl;
	//	}
	if (i_time<100) {
	  total_edep += i_edep;
	}
	if (i_particleName=="mu-" && i_stopped) {
	  mu_stop = true;
	}
      }
    }
    //    std::cout << "Total EDep = " << total_edep << std::endl;
    double smear = resolution->GetRandom();
    total_edep += smear;
    total_edep *= det_k_factor;
    if (total_edep>det_threshold) {
      //      std::cout << "Total EDep = " << total_edep << std::endl;
      hEDep_beam->Fill(total_edep);

      hInputEnergy->Fill(true_kinetic_energy);
      if (mu_stop) {
	hInputEnergy_Stop->Fill(true_kinetic_energy);
      }
    }
  }

  hEDep_beam->Scale(1.0 / tree->GetEntries());
  hEDep_beam->SetYTitle("Per Generated Event");

  TFile* outfile = new TFile(outfilename.c_str(), "RECREATE");
  hEDep_beam->Write();
  hInputEnergy->Write();
  hInputEnergy_Stop->Write();
  outfile->Write();
  outfile->Close();
}
