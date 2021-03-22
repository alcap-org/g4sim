#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TF1.h"
#include "TH2.h"

#include "RooUnfoldResponse.h"

void R15b_Michels_SiRs() {

  std::string dataset = "Si16b";
  //  std::string dataset = "Ti50";

  std::string filename = "output/R15b_1M_Geom-P";
  if (dataset == "Si16b") {
    filename += "5";
  }
  else if (dataset == "Ti50") {
    filename += "4";
  }
  filename += "_muplus_new.root";
  TFile* file = new TFile(filename.c_str(), "READ");
  //  //  TFile* file = new TFile("output/R15b_1M_Geom-P5_electron_new.root", "READ");
  //  //  TFile* file = new TFile("output/R15b_10M_Geom-P5_electron_new_RestrictedDir.root", "READ");
  //  TFile* file = new TFile("output/raw_g4sim.root", "READ");
  TTree* tree = (TTree*) file->Get("tree");

  double min_energy = 0;
  double max_energy = 1500;
  double energy_width = 50;
  int n_energy_bins = (max_energy - min_energy) / energy_width;

  std::string histtitle = dataset + " (MC)";
  
  TH1F* hEDep_SiR1 = new TH1F("hEDep_SiR1", histtitle.c_str(), n_energy_bins, min_energy, max_energy);
  TH1F* hEDep_SiR2_all = new TH1F("hEDep_SiR2_all", histtitle.c_str(), n_energy_bins, min_energy, max_energy);
  TH1F* hEDep_SiR2 = new TH1F("hEDep_SiR2", histtitle.c_str(), n_energy_bins, min_energy, max_energy);
  TH1F* hEDep_SiR2_two_layer = new TH1F("hEDep_SiR2_two_layer", histtitle.c_str(), n_energy_bins, min_energy, max_energy);
  TH1F* hEDep_SiR3_all = new TH1F("hEDep_SiR3_all", histtitle.c_str(), n_energy_bins, min_energy, max_energy);
  TH1F* hEDep_SiR3 = new TH1F("hEDep_SiR3", histtitle.c_str(), n_energy_bins, min_energy, max_energy);
  TH1F* hEDep_Target = new TH1F("hEDep_Target", histtitle.c_str(), n_energy_bins, min_energy, max_energy);
  TH2F* hEDep2D = new TH2F("hEDep2D", histtitle.c_str(), n_energy_bins, min_energy, max_energy, n_energy_bins, min_energy, max_energy);
  hEDep2D->SetYTitle("SiR2 Energy [keV]");
  hEDep2D->SetXTitle("SiR3 Energy [keV]");
  hEDep2D->SetLineColor(kBlack);

  double min_ekin = 0;
  double max_ekin = 60;
  double ekin_width = 1;
  double n_ekin_bins = (max_ekin - min_ekin) / ekin_width;
  double min_stop_power = 0;
  double max_stop_power = 3;
  double stop_power_width = 0.01;
  int n_stop_power_bins = (max_stop_power - min_stop_power) / stop_power_width;
  TH2F* hEKinVsStopPower_SiR2 = new TH2F("hEKinVsStopPower_SiR2", histtitle.c_str(), n_ekin_bins,min_ekin,max_ekin, n_stop_power_bins,min_stop_power,max_stop_power);
  TH2F* hEKinVsStopPower_SiR3 = new TH2F("hEKinVsStopPower_SiR3", histtitle.c_str(), n_ekin_bins,min_ekin,max_ekin, n_stop_power_bins,min_stop_power,max_stop_power);

  TH2F* hEDep2D_2 = new TH2F("hEDep2D_2", histtitle.c_str(), n_energy_bins, min_energy, max_energy, n_energy_bins, min_energy, max_energy);
  hEDep2D_2->SetXTitle("SiR2 Energy [keV]");
  hEDep2D_2->SetYTitle("SiR1 Energy [keV]");
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

  double SiR1_threshold = 100;
  double SiR2_threshold = 200;
  double SiR3_threshold = 300;

  TF1* SiR1_resolution = new TF1("res_SiR1_1", "TMath::Gaus(x, 0, 42.7)", -250, 250); // SiR1-1
  //  right_arm.thin_energy_resolutions.push_back(new TF1("res_SiR1_2", "TMath::Gaus(x, 0, 37.7)", -250, 250));
  //  right_arm.thin_energy_resolutions.push_back(new TF1("res_SiR1_3", "TMath::Gaus(x, 0, 40.7)", -250, 250));
  //  right_arm.thin_energy_resolutions.push_back(new TF1("res_SiR1_4", "TMath::Gaus(x, 0, 43.0)", -250, 250));
  //  TF1* SiR2_resolution = new TF1("res_SiR2", "TMath::Gaus(x, 0, 31.8)", -250, 250);
  TF1* SiR2_resolution = new TF1("res_SiR2", "TMath::Gaus(x, 0, 32.7)", -250, 250);
  //  TF1* SiR3_resolution = new TF1("res_SiR3", "TMath::Gaus(x, 0, 62.2)", -250, 250);
  //  TF1* SiR3_resolution = new TF1("res_SiR3", "TMath::Gaus(x, 0, 62.2, 1)", -1000, 1000);
  TF1* SiR3_resolution = new TF1("res_SiR3", "TMath::Gaus(x, 0, 63.8, 1)", -1000, 1000);
  //  TF1* SiR3_resolution = new TF1("res_SiR3", "TMath::Gaus(x, 0, 150, 1)", -1000, 1000);
  TH1F* hSiR3_resolution = new TH1F("hSiR3_resolution", "", 100,-250,250);

  double SiR2_k_factor = 1.2;//1.0;//1.46;
  double SiR3_k_factor = 0.8;//1.0;//0.85;//1.0;//1.46;//0.8;
  
  int n_entries = tree->GetEntries();
  for (int i_entry = 0; i_entry < n_entries; ++i_entry) {
    tree->GetEntry(i_entry);

    if (i_entry % 10000 == 0) {
      std::cout << i_entry << " / " << n_entries << std::endl;
    }


    bool SiR1_hit = false, SiR2_hit = false, SiR3_hit = false;
    double total_SiR1_edep = 0;

    double total_SiR2_edep = 0;
    double SiR2_ekin = 0;
    double total_SiR2_stepL = 0;
    
    double total_SiR3_edep = 0;
    double SiR3_ekin = 0;
    double total_SiR3_stepL = 0;
    
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

      if(i_volName=="SiR1") {
	total_SiR1_edep += i_edep*1e6;
      }
      else if (i_volName=="SiR2") {
	total_SiR2_edep += i_edep*1e6;
	total_SiR2_stepL += i_stepL;
	if (i_tid == 1) {
	  SiR2_ekin = i_ekin*1e3; // convert to MeV
	}
	//	std::cout << "Event #" << i_entry << ": SiR2: " << i_edep*1e6 << ", " << i_particleName << ", total = " << total_SiR2_edep << std::endl;
      }
      else if (i_volName=="SiR3") {
	total_SiR3_edep += i_edep*1e6;
	total_SiR3_stepL += i_stepL;
	if (i_tid == 1) {
	  SiR3_ekin = i_ekin*1e3; // convert to MeV
	}
	//	std::cout << "SiR3: " << i_edep << ", " << i_particleName << std::endl;
      }
      else if (i_volName=="Target") {
	total_Target_edep += i_edep*1e6;
	//	std::cout << "SiR3: " << i_edep << ", " << i_particleName << std::endl;
      }
    }

    if (total_SiR1_edep > 0) {
      total_SiR1_edep += SiR1_resolution->GetRandom();
      if (total_SiR1_edep > SiR1_threshold) {
	SiR1_hit = true;
	hEDep_SiR1->Fill(total_SiR1_edep);
      }
    }
    if (total_SiR2_edep > 0) {
      total_SiR2_edep += SiR2_resolution->GetRandom();
      total_SiR2_edep *= SiR2_k_factor;
      if (total_SiR2_edep > SiR2_threshold) {
	SiR2_hit = true;
	hEDep_SiR2_all->Fill(total_SiR2_edep);
      }
    }
    if (total_SiR3_edep > 0) {

      total_SiR3_edep += SiR3_resolution->GetRandom();
      total_SiR3_edep *= SiR3_k_factor;
      if (total_SiR3_edep > SiR3_threshold) {
	SiR3_hit = true;
	hEDep_SiR3_all->Fill(total_SiR3_edep);
      }
    }
    hEDep_Target->Fill(total_Target_edep);

    double density = 2.328; // g/cm^3
    double SiR2_stopping_power = (total_SiR2_edep*1e-3) / total_SiR2_stepL / density;
    hEKinVsStopPower_SiR2->Fill(SiR2_ekin, SiR2_stopping_power);
    double SiR3_stopping_power = (total_SiR3_edep*1e-3) / total_SiR3_stepL / density;
    hEKinVsStopPower_SiR3->Fill(SiR3_ekin, SiR3_stopping_power);

    if (!SiR1_hit && SiR2_hit) {
      hEDep_SiR2_two_layer->Fill(total_SiR2_edep);
    }
    if (!SiR1_hit && SiR2_hit && SiR3_hit) {
      //	std::cout << total_SiR2_edep << ", " << total_SiR3_edep << std::endl;
      hEDep2D->Fill(total_SiR3_edep, total_SiR2_edep);
      hEDep_SiR2->Fill(total_SiR2_edep);
      hEDep_SiR3->Fill(total_SiR3_edep);
    }
    if (SiR1_hit && SiR2_hit && SiR3_hit) {
    	hEDep2D_2->Fill(total_SiR2_edep, total_SiR1_edep);
    }
  }

        
  double min_smear = -250;
  double max_smear = 250;
  double smear_step = 1;
  TH1F* hEDep_SiR3_smeared = (TH1F*) hEDep_SiR3->Clone("hEDep_SiR3_smeared");
  hEDep_SiR3_smeared->Reset();
  for (double smear = min_smear; smear <= max_smear; smear += smear_step) {
    double prob = SiR3_resolution->Eval(smear);
    hSiR3_resolution->Fill(smear, prob);

    for (int i_bin = 1; i_bin <= hEDep_SiR3->GetNbinsX(); ++i_bin) {
      double edep = hEDep_SiR3->GetBinCenter(i_bin);
      double content = hEDep_SiR3->GetBinContent(i_bin);
      
      double edep_to_fill = edep + smear;
      //      edep_to_fill *= SiR3_k_factor;
      hEDep_SiR3_smeared->Fill(edep_to_fill, prob);
    }
  }

  
  hEDep2D->Draw("COLZ");
  hEDep_SiR3->Draw("HIST E");

  std::string outfilename = "MC_" + dataset + "Dataset_Michels_new.root";
  TFile* out = new TFile(outfilename.c_str(), "RECREATE");
  hEDep_SiR2->Write();
  hEDep_SiR3->Write();
  hEDep_SiR2_two_layer->Write();
  out->Write();
  out->Close();
}
