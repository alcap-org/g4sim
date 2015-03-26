// Plots a 2D histogram of energy deposited by muons in all the volumes in the output file

void EnergyDepositInAllVolumes(std::string filename) {

  TFile* file = new TFile(filename.c_str(), "READ");
  TTree* tree = (TTree*) file->Get("tree");
  TCanvas* c1 = new TCanvas("c1", "c1");

  double bin_width = 0.01; // MeV
  double max_x = 5;
  double min_x = 0;
  int n_bins = (max_x - min_x) / bin_width;

  std::string histtitle = "Energy Deposited by Muons in All Volumes (" + filename + ")";
  TH2F* hist_all = new TH2F("EnergyDepositedInAllVolumes", histtitle.c_str(), n_bins, min_x, max_x, 20,0,20);
  hist_all->SetXTitle("Energy Deposited [MeV]");
  hist_all->SetYTitle("Volume Name");
  hist_all->GetYaxis()->SetTitleOffset(1.4);

  tree->Draw("M_volName:M_edep*1e3>>EnergyDepositedInAllVolumes", "M_particleName==\"mu-\" && M_edep>0", "COLZ");

  c1->SetLogz();
}
