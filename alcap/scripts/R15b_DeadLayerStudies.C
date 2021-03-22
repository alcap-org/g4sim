#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TF1.h"

#include <iostream>
#include <string>
#include <sstream>

void R15b_DeadLayerStudies() {
  //  gStyle->SetOptFit(1);

  //  std::string filename = "output/R15b_1M_proton_wVacDeadLayers.root";
  std::string filename = "output/R15b_1M_proton_wNoDeadLayersAndProperSiR1Thickness.root";
  TFile* noDL_file = new TFile(filename.c_str(), "READ");
  TTree* noDL_tree = (TTree*) noDL_file->Get("tree");

  filename = "output/R15b_1M_proton_wDeadLayers.root";
  TFile* DL_file = new TFile(filename.c_str(), "READ");
  TTree* DL_tree = (TTree*) DL_file->Get("tree");

  //  double min_energy = 200;
  double min_energy = 0;
  double max_energy = 1500;
  double energy_width = 20;
  int n_energy_bins = (max_energy - min_energy) / energy_width;

  TH1F* hNoDL = new TH1F("hNoDL", "", n_energy_bins,min_energy,max_energy);
  hNoDL->SetTitle("Energy Deposited in SiR1");
  hNoDL->SetXTitle("EDep [MeV]");
  hNoDL->SetYTitle("Counts");
  TH1F* hDL = new TH1F("hDL", "", n_energy_bins,min_energy,max_energy);
  hDL->SetLineColor(kRed);

  noDL_tree->Draw("M_edep*1e6>>hNoDL", "M_volName==\"SiR1\" && M_particleName==\"proton\"", "goff");
  DL_tree->Draw("M_edep*1e6>>hDL", "M_volName==\"SiR1\" && M_particleName==\"proton\"", "goff");

  hNoDL->Draw();
  hDL->Draw("SAMES");

  TLegend *leg = new TLegend(0.59,0.64,0.79,0.88,NULL,"brNDC");
  leg->SetBorderSize(0);
  leg->SetTextSize(0.03);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);
  leg->AddEntry(hNoDL, "Dead Layers Vacuum", "l");
  leg->AddEntry(hDL, "Dead Layers", "l");
  leg->Draw();
}
