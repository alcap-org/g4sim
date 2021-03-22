#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TF1.h"
#include "TH2.h"

#include "RooUnfoldResponse.h"

//void R15b_Ti50_Michels() {
void R15b_Michels_SiLs() {

  //  std::string dataset = "Si16b";
  std::string dataset = "Ti50";

  std::string filename = "output/R15b_1M_Geom-P";
  if (dataset == "Si16b") {
    filename += "5";
  }
  else if (dataset == "Ti50") {
    filename += "4";
  }
  filename += "_muplus_new.root";
  TFile* file = new TFile(filename.c_str(), "READ");
  //  TFile* file = new TFile("output/raw_g4sim.root", "READ");
  TTree* tree = (TTree*) file->Get("tree");

  double min_energy = 0;
  double max_energy = 1500;
  double energy_width = 50;
  int n_energy_bins = (max_energy - min_energy) / energy_width;

  std::string histtitle = dataset + " (MC)";
  
  TH1F* hEDep_SiL1 = new TH1F("hEDep_SiL1", histtitle.c_str(), n_energy_bins, min_energy, max_energy);
  TH1F* hEDep_SiL3 = new TH1F("hEDep_SiL3", histtitle.c_str(), n_energy_bins, min_energy, max_energy);

  TH2F* hEDep2D_2 = new TH2F("hEDep2D_2", histtitle.c_str(), n_energy_bins, min_energy, max_energy, n_energy_bins, min_energy, max_energy);
  hEDep2D_2->SetXTitle("SiL3 Energy [keV]");
  hEDep2D_2->SetYTitle("SiL1 Energy [keV]");
  hEDep2D_2->SetLineColor(kBlack);


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

  std::vector<double>* ekin = 0;
  TBranch* br_ekin = tree->GetBranch("M_ekin");
  br_ekin->SetAddress(&ekin);

  std::vector<int>* tid = 0;
  TBranch* br_tid = tree->GetBranch("M_tid");
  br_tid->SetAddress(&tid);

  std::vector<double>* stepL = 0;
  TBranch* br_stepL = tree->GetBranch("M_stepL");
  br_stepL->SetAddress(&stepL);

  double SiL1_threshold = 100;
  double SiL3_threshold = 35;
  double SiR3_threshold = 250;

  TF1* SiL1_resolution = new TF1("res_SiL1_1", "TMath::Gaus(x, 0, 30.1)", -250, 250); // mean of all channels
  TF1* SiL3_resolution = new TF1("res_SiL3", "TMath::Gaus(x, 0, 30.9)", -250, 250);
  //  TF1* SiL3_resolution = new TF1("res_SiL3", "TMath::Gaus(x, 0, 150)", -250, 250);

  double SiL3_k_factor = 1.0;//1.46;
  
  int n_entries = tree->GetEntries();
  for (int i_entry = 0; i_entry < n_entries; ++i_entry) {
    tree->GetEntry(i_entry);

    if (i_entry % 10000 == 0) {
      std::cout << i_entry << " / " << n_entries << std::endl;
    }


    bool SiL1_hit = false, SiL3_hit = false;
    double total_SiL1_edep = 0;

    double total_SiL3_edep = 0;
    double SiL3_ekin = 0;
    double total_SiL3_stepL = 0;
        
    double total_Target_edep = 0;
    for (int iElement = 0; iElement < volName->size();  ++iElement) {
      std::string i_volName = volName->at(iElement);
      std::string i_particleName = particleName->at(iElement);
      double i_edep = edep->at(iElement);
      int i_stopped = stopped->at(iElement);
      double i_time = t->at(iElement);
      double i_ekin = ekin->at(iElement);
      int i_tid = tid->at(iElement);
      double i_stepL = stepL->at(iElement);

      if(i_volName=="SiL1") {
	total_SiL1_edep += i_edep*1e6;
      }
      else if (i_volName=="SiL3") {
	total_SiL3_edep += i_edep*1e6;
	total_SiL3_stepL += i_stepL;
	if (i_tid == 1) {
	  SiL3_ekin = i_ekin*1e3; // convert to MeV
	}
	//	std::cout << "Event #" << i_entry << ": SiL3: " << i_edep*1e6 << ", " << i_particleName << ", total = " << total_SiL3_edep << std::endl;
      }
      else if (i_volName=="Target") {
	total_Target_edep += i_edep*1e6;
	//	std::cout << "SiR3: " << i_edep << ", " << i_particleName << std::endl;
      }
    }

    if (total_SiL1_edep > 0) {
      total_SiL1_edep += SiL1_resolution->GetRandom();
      if (total_SiL1_edep > SiL1_threshold) {
	SiL1_hit = true;
	hEDep_SiL1->Fill(total_SiL1_edep);
      }
    }
    if (total_SiL3_edep > 0) {
      //      std::cout << "Resolution: " << total_SiL3_edep << " --> ";
      total_SiL3_edep += SiL3_resolution->GetRandom();
      //      std::cout << total_SiL3_edep << " keV" << std::endl;
      total_SiL3_edep *= SiL3_k_factor;
      
      if (total_SiL3_edep > SiL3_threshold) {
	SiL3_hit = true;
      }
    }

    if (!SiL1_hit && SiL3_hit) {
      //	std::cout << total_SiL3_edep << ", " << total_SiR3_edep << std::endl;
      hEDep_SiL3->Fill(total_SiL3_edep);
    }
    if (SiL1_hit && SiL3_hit) {
    	hEDep2D_2->Fill(total_SiL3_edep, total_SiL1_edep);
    }
  }

  hEDep_SiL3->Draw("COLZ");
  
  //  std::string outfilename = "MC_" + dataset + "Dataset_Michels.root";
  std::string outfilename = "MC_" + dataset + "Dataset_Michels_new.root";
  TFile* out = new TFile(outfilename.c_str(), "UPDATE");
  hEDep_SiL3->Write();
  out->Write();
  out->Close();

}
