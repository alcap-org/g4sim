#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TLegend.h"

#include <iostream>
#include <cmath>

void InitialProtonLocation(std::string filename) {

  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");

  std::vector<std::string>* particleName = 0;
  TBranch* br_particleName = tree->GetBranch("M_particleName");
  br_particleName->SetAddress(&particleName);

  std::vector<std::string>* volName = 0;
  TBranch* br_volName = tree->GetBranch("M_volName");
  br_volName->SetAddress(&volName);

  double initial_z = 0;
  TBranch* br_initial_z = tree->GetBranch("i_z");
  br_initial_z->SetAddress(&initial_z);
  double initial_x = 0;
  TBranch* br_initial_x = tree->GetBranch("i_x");
  br_initial_x->SetAddress(&initial_x);

  std::vector<std::string>* oprocess = 0;
  TBranch* br_oprocess = tree->GetBranch("M_oprocess");
  br_oprocess->SetAddress(&oprocess);

  std::vector<int>* stopped = 0;
  TBranch* br_stopped = tree->GetBranch("M_stopped");
  br_stopped->SetAddress(&stopped);

  std::vector<double>* px = 0;
  TBranch* br_px = tree->GetBranch("M_px");
  br_px->SetAddress(&px);
  std::vector<double>* py = 0;
  TBranch* br_py = tree->GetBranch("M_py");
  br_py->SetAddress(&py);
  std::vector<double>* pz = 0;
  TBranch* br_pz = tree->GetBranch("M_pz");
  br_pz->SetAddress(&pz);

  double proton_mass = 938.272 * 1000; // convert to keV


  double target_depth = 100e3; // microns
  double min_depth = -target_depth/2; double max_depth = target_depth/2; double bin_width = 1e3;
  int n_bins = (max_depth - min_depth) / bin_width;
  TH1F* hSiLProtons = new TH1F("hSiLProtons", "hSiLProtons", n_bins,min_depth,max_depth);
  hSiLProtons->SetLineColor(kBlue);
  hSiLProtons->SetLineWidth(2);
  TH1F* hSiRProtons = new TH1F("hSiRProtons", "hSiRProtons", n_bins,min_depth,max_depth);
  hSiRProtons->SetLineColor(kRed);
  hSiRProtons->SetLineWidth(2);

  double min_energy = 0; double max_energy = 20000; bin_width = 500;
  n_bins = (max_energy - min_energy) / bin_width;
  TH1F* hSiLProtonsEnergy = new TH1F("hSiLProtonsEnergy", "hSiLProtonsEnergy", n_bins,min_energy,max_energy);
  hSiLProtonsEnergy->SetLineColor(kBlue);
  hSiLProtonsEnergy->SetLineWidth(2);
  TH1F* hSiRProtonsEnergy = new TH1F("hSiRProtonsEnergy", "hSiRProtonsEnergy", n_bins,min_energy,max_energy);
  hSiRProtonsEnergy->SetLineColor(kRed);
  hSiRProtonsEnergy->SetLineWidth(2);

  int n_entries = tree->GetEntries();
  for (int i_entry = 0; i_entry < n_entries; ++i_entry) {
    tree->GetEvent(i_entry);
     
    if (i_entry % 10000 == 0) {
      std::cout << i_entry << " / " << n_entries << std::endl;
    }

    double i_initial_z = initial_z * 10000; // convert to microns
    double i_initial_x = initial_x * 10000; // convert to microns
    double local_stopping_depth = (1/sqrt(2))*i_initial_x + (1/sqrt(2))*i_initial_z;
    for (int iElement = 0; iElement < particleName->size();  ++iElement) {
      std::string i_particleName = particleName->at(iElement);
      std::string i_volName = volName->at(iElement);
      std::string i_oprocess = oprocess->at(iElement);
      int i_stopped = stopped->at(iElement);

      double i_px = px->at(iElement)*1e6; // convert to keV
      double i_py = py->at(iElement)*1e6;
      double i_pz = pz->at(iElement)*1e6;
      double squared_mom = i_px*i_px + i_py*i_py + i_pz*i_pz;
      double total_energy = std::sqrt(squared_mom + proton_mass*proton_mass);
      double kinetic_energy = total_energy - proton_mass;

      
      if (i_particleName=="proton" && i_volName=="ESi2" && i_oprocess=="NULL" && i_stopped==1) {
	hSiLProtons->Fill(local_stopping_depth);
	hSiLProtonsEnergy->Fill(kinetic_energy);
      }
      if (i_particleName=="proton" && i_volName=="ESi1" && i_oprocess=="NULL" && i_stopped==1) {
	hSiRProtons->Fill(local_stopping_depth);
	hSiRProtonsEnergy->Fill(kinetic_energy);
      }
    }
  }
  hSiLProtons->Draw("");
  hSiRProtons->Draw("SAMES");
  //  hSiLProtonsEnergy->Draw("");
  //  hSiRProtonsEnergy->Draw("SAMES");

  TLegend *leg = new TLegend(0.41,0.68,0.61,0.78);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.04);
  leg->SetFillColor(kWhite);
  leg->AddEntry(hSiLProtons, "SiL", "l");
  leg->AddEntry(hSiRProtons, "SiR", "l");
  leg->Draw();

  // Find the fractions in the front and back halfs
  double SiL_full_error;
  double SiL_full = hSiLProtons->IntegralAndError(1, hSiLProtons->GetNbinsX(), SiL_full_error);
  double SiR_full_error;
  double SiR_full = hSiRProtons->IntegralAndError(1, hSiRProtons->GetNbinsX(), SiR_full_error);

  int middle_bin = hSiLProtons->FindBin(0);
  double SiL_back_error;
  double SiL_back = hSiLProtons->IntegralAndError(1, middle_bin, SiL_back_error);
  double SiR_back_error;
  double SiR_back = hSiRProtons->IntegralAndError(1, middle_bin, SiR_back_error);

  double SiL_front_error;
  double SiL_front = hSiLProtons->IntegralAndError(middle_bin+1, hSiLProtons->GetNbinsX(), SiL_front_error);
  double SiR_front_error;
  double SiR_front = hSiRProtons->IntegralAndError(middle_bin+1, hSiRProtons->GetNbinsX(), SiR_front_error);

  std::cout << "SiL full: " << SiL_full << " +- " << SiL_full_error << std::endl;
  std::cout << "SiR full: " << SiR_full << " +- " << SiR_full_error << std::endl;
  std::cout << "SiL back: " << SiL_back << " +- " << SiL_back_error << std::endl;
  std::cout << "SiR back: " << SiR_back << " +- " << SiR_back_error << std::endl;
  std::cout << "SiL front: " << SiL_front << " +- " << SiL_front_error << std::endl;
  std::cout << "SiR front: " << SiR_front << " +- " << SiR_front_error << std::endl;

  // Check that front and back add up to the value for full
  std::cout << SiL_back << " + " << SiL_front << " = " << SiL_back+SiL_front << " (" << SiL_full << "?)" << std::endl;
  std::cout << SiR_back << " + " << SiR_front << " = " << SiR_back+SiR_front << " (" << SiR_full << "?)" << std::endl;

  // Fractions
  double SiL_full_fractional_error = SiL_full_error / SiL_full;
  double SiR_full_fractional_error = SiR_full_error / SiR_full;

  double SiL_front_fractional_error = SiL_front_error / SiL_front;
  double SiR_front_fractional_error = SiR_front_error / SiR_front;

  double SiL_front_fraction = SiL_front / SiL_full;
  double SiR_front_fraction = SiR_front / SiR_full;
  std::cout << "Fraction Front: SiL = " << SiL_front_fraction << " +- " << SiL_front_fraction*sqrt(SiL_front_fractional_error*SiL_front_fractional_error + SiL_full_fractional_error*SiL_full_fractional_error) << std::endl;
  std::cout << "\t\tSiR = " << SiR_front_fraction << " +- " << SiR_front_fraction*sqrt(SiR_front_fractional_error*SiR_front_fractional_error + SiR_full_fractional_error*SiR_full_fractional_error) << std::endl;

  double SiL_back_fractional_error = SiL_back_error / SiL_back;
  double SiR_back_fractional_error = SiR_back_error / SiR_back;

  double SiL_back_fraction = SiL_back / SiL_full;
  double SiR_back_fraction = SiR_back / SiR_full;
  std::cout << "Fraction Back: SiL = " << SiL_back_fraction << " +- " << SiL_back_fraction*sqrt(SiL_back_fractional_error*SiL_back_fractional_error + SiL_full_fractional_error*SiL_full_fractional_error) << std::endl;
  std::cout << "\t\tSiR = " << SiR_back_fraction << " +- " << SiR_back_fraction*sqrt(SiR_back_fractional_error*SiR_back_fractional_error + SiR_full_fractional_error*SiR_full_fractional_error) << std::endl;

}
