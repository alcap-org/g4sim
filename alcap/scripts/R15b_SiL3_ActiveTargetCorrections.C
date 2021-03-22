#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TF1.h"

#include "RooUnfoldResponse.h"

void R15b_SiL3_MuPlusEDep() {

  //  TFile* muplus_file = new TFile("output/R15b_100k_Geom-P1_muplus-UIV-target.root", "READ");
  //  TFile* muplus_file = new TFile("output/R15b_100k_Geom-P1_muplus-target.root", "READ");
  //  TFile* muplus_file = new TFile("output/R15b_100k_Geom-P1-3mm_muplus-target.root", "READ");
  //  TFile* muplus_file = new TFile("output/R15b_1M_Geom-P1_muplus.root", "READ");
  //  TFile* muplus_file = new TFile("output/R15b_10M_Geom-P1_muplus.root", "READ");
  //  TFile* muplus_file = new TFile("output/R15b_100k_Geom-P1_muplus_w1475umSiL3.root", "READ");
  //  TFile* muplus_file = new TFile("output/R15b_1M_Geom-P1_electrons-45-55MeV.root", "READ");

  //  TFile* muplus_file = new TFile("output/R15b_1M_Geom-P4_muplus_new.root", "READ");
  TFile* muplus_file = new TFile("output/R15b_1M_Geom-P4_muplus_w1475umSiL3.root", "READ");

  //  TFile* muplus_file = new TFile("output/raw_g4sim.root", "READ");
  
  TTree* muplus_tree = (TTree*) muplus_file->Get("tree");

  std::string outfilename = "dump.root";

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

  TF1* SiL3_resolution = new TF1("res_SiL3", "TMath::Gaus(x, 0, 30.9)", -250, 250);
  //  double SiL3_threshold = 35; // keV
  double SiL3_threshold = 180; // keV
  double SiL3_k_factor = 1.0;//0.9;
  
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

      
      //      if (i_volName=="Target") {
      if (i_volName=="SiL3") {
      //      if (i_volName=="SiR2") {
	//	std::cout << "Event #" << i_entry << ": " << i_volName << ": " << i_particleName << ": i_edep = " << i_edep << " keV, t = " << i_time << " ns" << std::endl;
	total_edep += i_edep;
      }
    }
    double smear = SiL3_resolution->GetRandom();
    total_edep += smear;
    total_edep *= SiL3_k_factor;
    if (total_edep>SiL3_threshold) {
      //      std::cout << "Total EDep = " << total_edep << std::endl;
      hEDep_muplus->Fill(total_edep);
    }
  }

  hEDep_muplus->Scale(1.0 / muplus_tree->GetEntries());
  hEDep_muplus->SetYTitle("Per Generated Event");

  TFile* outfile = new TFile(outfilename.c_str(), "RECREATE");
  //  hEDep_All->Write();
  //  //  hEDep_proton->Write();
  //  hEDep_stopped->Write();
  //  hEDep_not_stopped->Write();
  //  hStoppingFraction->Write();
  //  hETrue_All->Write();
  //  hETrue_stopped->Write();
  //  hETrue_not_stopped->Write();
  //  hStoppingFraction_True->Write();
  hEDep_muplus->Write();
  //  //  hProton_TotalEDepVsInitialEnergy->Write();
  //  TH2* hResponse = response->Hresponse();
  //  hResponse->SetXTitle("Observed Energy [keV]");
  //  hResponse->SetYTitle("True Energy [keV]");
  //  response->Write();
  
  outfile->Write();
  outfile->Close();

  
  /*
  //  TFile* file = new TFile("output/R15b_1M_Geom-P1_proton-UIV-target.root", "READ");
  //  TFile* file = new TFile("output/raw_g4sim.root", "READ");
  //  TFile* file = new TFile("output/R15b_10M_Geom-P1_proton-target.root", "READ");
  //  TFile* file = new TFile("output/R15b_10M_Geom-P1_proton-target-expo.root", "READ");
  TFile* file = new TFile("output/R15b_10M_Geom-P1_proton-target-large.root", "READ");
  //  TFile* file = new TFile("output/R15b_10M_Geom-P1-3mm_proton-target-large.root", "READ");
  //  TFile* file = new TFile("output/R15b_10M_Geom-P1_proton-target-large-expo.root", "READ");
  //  TFile* file = new TFile("output/R15b_10M_Geom-P1_proton-target-large-straight-in.root", "READ");

  //  std::string outfilename = "R15b_SiL3-ActiveTargetAnalysis_MC_Corrections.root";
  //  std::string outfilename = "R15b_SiL3-ActiveTargetAnalysis_MC-expo_Corrections.root";
  //  std::string outfilename = "R15b_SiL3-ActiveTargetAnalysis_MC-large_Corrections.root";
  //  std::string outfilename = "R15b_SiL3-ActiveTargetAnalysis_MC-small_Corrections.root";
  //  std::string outfilename = "R15b_SiL3-ActiveTargetAnalysis_MC-expo-small_Corrections.root";
  //  std::string outfilename = "R15b_SiL3-ActiveTargetAnalysis_MC-smaller_Corrections.root";
  //  std::string outfilename = "R15b_SiL3-ActiveTargetAnalysis_MC-smaller-rebin_Corrections.root";
  //  std::string outfilename = "R15b_SiL3-ActiveTargetAnalysis_MC-expo-smaller_Corrections.root";
  //  std::string outfilename = "R15b_SiL3-ActiveTargetAnalysis_MC-large-rebin_Corrections.root";
  //  std::string outfilename = "R15b_SiL3-ActiveTargetAnalysis_MC-smaller-100MeV_Corrections.root";
  //  std::string outfilename = "R15b_SiL3-ActiveTargetAnalysis_MC-smaller-30MeV_Corrections.root";
  //  std::string outfilename = "R15b_SiL3-ActiveTargetAnalysis_MC-Protons-100-MeV_Range-0-10-MeV_Binning-100keV.root";
  //  std::string outfilename = "R15b_SiL3-ActiveTargetAnalysis_MC-Protons-100-MeV-Large-Expo_Range-0-100-MeV_Binning-100keV.root";
  //  std::string outfilename = "R15b_SiL3-ActiveTargetAnalysis_MC-Protons-100-MeV_Range-0-20-MeV_Binning-100keV.root";
  //  std::string outfilename = "R15b_SiL3-ActiveTargetAnalysis_MC-Protons-100-MeV_Range-0-50-MeV_Binning-100keV.root";
  //  std::string outfilename = "R15b_SiL3-ActiveTargetAnalysis_MC-Protons-100-MeV-3mm_Range-0-100-MeV_Binning-100keV.root";
  //  std::string outfilename = "R15b_SiL3-ActiveTargetAnalysis_MC-Protons-100-MeV_Range-0-100-MeV_Binning-500keV.root";
  //  std::string outfilename = "R15b_SiL3-ActiveTargetAnalysis_MC-Protons-100-MeV_Range-0-30-MeV_Binning-100keV.root";
  //  std::string outfilename = "R15b_SiL3-ActiveTargetAnalysis_MC-Protons-100-MeV-Expo_Range-0-50-MeV_Binning-100keV.root";
  std::string outfilename = "test.root";
  TTree* tree = (TTree*) file->Get("tree");

  double min_energy = 0;
  double max_energy = 50000;
  double energy_width = 500;
  int n_energy_bins = (max_energy - min_energy) / energy_width;

  TH1F* hEDep_All = new TH1F("hEDep_All", "", n_energy_bins, min_energy, max_energy);
  hEDep_All->SetXTitle("Energy Deposited in Target [keV]");
  hEDep_All->SetLineColor(kBlack);

  TH1F* hEDep_stopped = (TH1F*) hEDep_All->Clone("hEDep_stopped");
  hEDep_stopped->SetLineColor(kRed);
  TH1F* hEDep_not_stopped = (TH1F*) hEDep_All->Clone("hEDep_not_stopped");
  hEDep_not_stopped->SetLineColor(kMagenta);

  TH1F* hETrue_All = new TH1F("hETrue_All", "", n_energy_bins, min_energy, max_energy);
  hETrue_All->SetXTitle("True Energy [keV]");
  hETrue_All->SetLineColor(kBlack);

  TH1F* hETrue_stopped = (TH1F*) hETrue_All->Clone("hETrue_stopped");
  hETrue_stopped->SetLineColor(kRed);
  TH1F* hETrue_not_stopped = (TH1F*) hETrue_All->Clone("hETrue_not_stopped");
  hETrue_not_stopped->SetLineColor(kMagenta);

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
  RooUnfoldResponse* response = new RooUnfoldResponse(n_observed_energy_bins,min_energy,max_energy, n_true_energy_bins,min_energy,max_energy, "SiL3_response");

  TF1* SiL3_resolution = new TF1("res_SiL3", "TMath::Gaus(x, 0, 30.9)", -250, 250);
  double SiL3_threshold = 35; // keV
  double SiL3_k_factor = 1.0;//0.9;
  
  double max_diff = 100; // accept as stopped if there's less than 1 keV difference between initial_energy and total_edep
  for (int i_entry = 0; i_entry < tree->GetEntries(); ++i_entry) {
    tree->GetEntry(i_entry);

    if (i_entry % 10000 == 0) {
      std::cout << i_entry << " / " << tree->GetEntries() << std::endl;
    }


    double total_edep = 0;
    double initial_energy = 0;
    for (int iElement = 0; iElement < volName->size();  ++iElement) {
      std::string i_volName = volName->at(iElement);
      double i_edep = edep->at(iElement)*1e6;
      int i_stopped = stopped->at(iElement);
      double i_time = t->at(iElement);
      
      if(i_volName=="Target") {
	total_edep += i_edep;
	      
	if (iElement == 0) {
	  initial_energy = (std::sqrt(i_px*i_px + i_py*i_py + i_pz*i_pz + proton_mass*proton_mass) - proton_mass)*1e3; // convert to keV
	  //	  std::cout << initial_energy << std::endl;
	}
	//	std::cout << i_volName << " " << i_edep << " " << total_edep << std::endl;
      }
    }
    if (initial_energy>max_energy || initial_energy<min_energy) {
      //      response->Fake(total_edep);
      continue;
    }

    //    std::cout << std::setprecision(10) << "AE: Total EDep = " << total_edep << ", Initial E = " << initial_energy << " ";
    double smear = SiL3_resolution->GetRandom();
    total_edep += smear;
    total_edep *= SiL3_k_factor;
    if (total_edep < SiL3_threshold) {
      continue;
    }
    
    hEDep_All->Fill(total_edep);
    hETrue_All->Fill(initial_energy);
    double difference = (initial_energy - total_edep);
    if (difference > max_diff) {
      //      std::cout << "Not Stopped!" << std::endl;
      hEDep_not_stopped->Fill(total_edep);
      hETrue_not_stopped->Fill(initial_energy);
      response->Fill(total_edep, initial_energy);
      //      response->Fake(total_edep);
    }
    else {
      //      std::cout << "Stopped!" << std::endl;
      hEDep_stopped->Fill(total_edep);
      hETrue_stopped->Fill(initial_energy);
      response->Fill(total_edep, initial_energy);
    }
    //    if (initial_energy > max_energy) {
    //      response->Fake(total_edep);
    //    }
    //    else {
    //    response->Fill(total_edep, initial_energy);
      //    }
    //    response->Fill(total_edep, initial_energy);
  }
  //  tree->Draw("M_edep*1e6>>hEDep_All", "M_volName==\"Target\"", "goff");
  //  tree->Draw("M_edep*1e6>>hEDep_proton", "M_volName==\"Target\" && M_particleName==\"proton\"", "goff");
  //  tree->Draw("M_edep*1e6>>hEDep_proton_stopped", "M_volName==\"Target\" && M_particleName==\"proton\" && M_stopped==1", "goff");
  //  tree->Draw("M_edep*1e6>>hEDep_proton_not_stopped", "M_volName==\"Target\" && M_particleName==\"proton\" && M_stopped==0", "goff");

  //  hEDep_All->Draw();
  //  //  hEDep_proton->Draw("SAME");
  //  hEDep_stopped->Draw("SAME");
  //  hEDep_not_stopped->Draw("SAME");
  //  hProton_TotalEDepVsInitialEnergy->Draw("COLZ");
  TH1F* hStoppingFraction = (TH1F*) hEDep_stopped->Clone("hStoppingFraction");
  hStoppingFraction->Sumw2();
  hStoppingFraction->Divide(hEDep_All);
  hStoppingFraction->SetYTitle("Stopping Fraction");

  TH1F* hStoppingFraction_True = (TH1F*) hETrue_stopped->Clone("hStoppingFraction_True");
  hStoppingFraction_True->Sumw2();
  hStoppingFraction_True->Divide(hETrue_All);
  hStoppingFraction_True->SetYTitle("Stopping Fraction True");
  */  
}
