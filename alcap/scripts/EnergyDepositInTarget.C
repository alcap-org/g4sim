void EnergyDepositInTarget(std::string filename) {

  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");

  double bin_width = 0.1; // MeV
  double max_x = 30;
  double min_x = 0;
  int n_bins = (max_x - min_x) / bin_width;

  std::string histtitle = "Energy Deposited by Charged Particles in Target (" + filename + ")";
  TH1F* hist_all = new TH1F("EnergyDepositedInTarget", histtitle.c_str(), n_bins, min_x, max_x);
  hist_all->SetXTitle("Energy Deposited [MeV]");
  hist_all->SetYTitle("Counts");
  hist_all->GetYaxis()->SetTitleOffset(1.3);
  hist_all->SetLineWidth(2);

  // Currently just for a SiR2 target but can easily be extended
  tree->Draw("M_edep*1e3>>EnergyDepositedInTarget", "M_volName==\"SiR2\" && M_charge!=0 && (M_edep*1e3)>=1");

  TH1F* hist_muons = new TH1F("MuonEnergyDepositedInTarget", histtitle.c_str(), n_bins, min_x, max_x);
  hist_muons->SetXTitle("Energy Deposited [MeV]");
  hist_muons->SetYTitle("Counts");
  hist_muons->GetYaxis()->SetTitleOffset(1.3);
  hist_muons->SetLineWidth(2);
  hist_muons->SetLineColor(kRed);

  // Currently just for a SiR2 target but can easily be extended
  tree->Draw("M_edep*1e3>>MuonEnergyDepositedInTarget", "M_volName==\"SiR2\" && M_charge!=0 && (M_edep*1e3)>=1 && M_particleName==\"mu-\"", "SAMES");

  TLegend* leg = new TLegend(0.45, 0.8, 0.7, 0.6, "");
  leg->SetBorderSize(0);
  leg->SetTextSize(0.04);
  leg->SetFillColor(kWhite);
  leg->AddEntry(hist_all, "All Particles", "l");
  leg->AddEntry(hist_muons, "Muons", "l");
  leg->Draw();
}
