#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH2.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFitResult.h"
#include "TLatex.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

void FractionProtonsStop(std::string filename) {

  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");

  std::vector<std::string>* particleName = 0;
  std::vector<std::string>* volName = 0;
  std::vector<double>* px = 0;
  std::vector<double>* py = 0;
  std::vector<double>* pz = 0;
  double proton_mass = 938272; // keV
  std::vector<int>* stopped = 0;
  
  TBranch* br_particleName = tree->GetBranch("M_particleName");
  TBranch* br_volName = tree->GetBranch("M_volName");
  TBranch* br_px = tree->GetBranch("M_px");
  TBranch* br_py = tree->GetBranch("M_py");
  TBranch* br_pz = tree->GetBranch("M_pz");
  TBranch* br_stopped = tree->GetBranch("M_stopped");
  
  br_particleName->SetAddress(&particleName);
  br_volName->SetAddress(&volName);
  br_px->SetAddress(&px);
  br_py->SetAddress(&py);
  br_pz->SetAddress(&pz);
  br_stopped->SetAddress(&stopped);

  const double bin_width = 200;
  const double x_lower_limit = 0;
  const double x_upper_limit = 25000;
  int n_bins_x = (x_upper_limit - x_lower_limit) / bin_width;
  const double y_lower_limit = 0;
  const double y_upper_limit = 10000;
  int n_bins_y = (y_upper_limit - y_lower_limit) / bin_width;

  TH1F* hStoppedProtons;
  TH1F* hAllProtons;

  std::string histname = "hProtonsStopped";
  std::string histtitle = "";
  hStoppedProtons = new TH1F(histname.c_str(), histtitle.c_str(), n_bins_x,x_lower_limit,x_upper_limit);
  hStoppedProtons->SetXTitle("Kinetic Energy [keV]");
  hStoppedProtons->GetYaxis()->SetTitleOffset(1.4);
  hStoppedProtons->SetYTitle("Fraction of Protons that Stop");

  histname = "hAllProtons";
  histtitle = "";
  hAllProtons = new TH1F(histname.c_str(), histtitle.c_str(), n_bins_x,x_lower_limit,x_upper_limit);
  hAllProtons->SetXTitle("E_{1} + E_{2} [keV]");
  hAllProtons->GetYaxis()->SetTitleOffset(1.4);
  hAllProtons->SetYTitle("Number of Protons");

  for (int i_entry = 0; i_entry < tree->GetEntries(); ++i_entry) {
    tree->GetEvent(i_entry);
     
    if (i_entry % 10000 == 0) {
      std::cout << i_entry << " / " << tree->GetEntries() << std::endl;
    }
    double E = 0;

    for (int iElement = 0; iElement < particleName->size();  ++iElement) {
      std::string i_particleName = particleName->at(iElement);
      std::string i_volName = volName->at(iElement);
      double i_px = px->at(iElement)*1e6; // convert to keV
      double i_py = py->at(iElement)*1e6; // convert to keV
      double i_pz = pz->at(iElement)*1e6; // convert to keV
      int i_stopped = stopped->at(iElement);

      double kinetic_energy = std::sqrt(i_px*i_px + i_py*i_py + i_pz*i_pz + proton_mass*proton_mass) - proton_mass;
      if ( (i_volName == "ESi1" || i_volName == "ESi2") && i_particleName == "proton") {
	hAllProtons->Fill(kinetic_energy); // fill in the "all" plot
	
	if (i_stopped == 1) {
	  hStoppedProtons->Fill(kinetic_energy); // fill in the "all" plot
	}
      }
    }
  }

  // Divide stopped protons by all protons
  hStoppedProtons->Divide(hAllProtons);
  hStoppedProtons->SetStats(false);
  hStoppedProtons->Draw();
}
